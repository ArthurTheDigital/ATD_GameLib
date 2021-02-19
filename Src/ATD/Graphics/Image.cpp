/**
 * @file      
 * @brief     Image class implementation.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Graphics/Image.hpp>

#include <ATD/Core/ImageImpl.hpp>
#include <ATD/Core/Printf.hpp>
#include <ATD/Graphics/AnimatedGif.hpp>

#include <errno.h>
#include <jpeglib.h>
#include <png.h>
#include <setjmp.h>
#include <stdio.h>
#include <string.h>

#include <stdexcept>


#define IGNORE_UNUSED(x) (void)(x)


/* ATD::Image auxiliary: */

struct CustomJpegErrorMgr
{
	struct jpeg_error_mgr pub;
	std::string str;
	jmp_buf jmp;
};

const std::string _JPEG_ERROR_DELIMITER = "\n";

static void _customJpegOutputMessage(j_common_ptr info)
{
	char buffer[JMSG_LENGTH_MAX];
	(*info->err->format_message) (info, buffer);

	CustomJpegErrorMgr *err = reinterpret_cast<CustomJpegErrorMgr *>(info->err);
	err->str += 
			(err->str.size() ? _JPEG_ERROR_DELIMITER : std::string()) + 
			std::string(buffer);
}

static void _customJpegErrorExit(j_common_ptr info)
{
	CustomJpegErrorMgr *err = reinterpret_cast<CustomJpegErrorMgr *>(info->err);
	(*info->err->output_message) (info);
	longjmp(err->jmp, 1);
}


/* ATD::Image::Drawable */

ATD::Image::Drawable::Drawable()
{}

ATD::Image::Drawable::~Drawable()
{}


/* ATD::Image constants */

const std::vector<std::string> ATD::Image::EXTENSIONS = std::vector<std::string>({
	"png", 
	"jpeg", 
	"jpg", 
	"gif"
});

const size_t ATD::Image::EXT_PNG =  0;
const size_t ATD::Image::EXT_JPEG = 1;
const size_t ATD::Image::EXT_JPG =  2;
const size_t ATD::Image::EXT_GIF =  3;


/* ATD::Image */

ATD::Image::Image()
	: Image(Vector2S(0, 0))
{}

ATD::Image::Image(const ATD::Vector2S &size, 
		const ATD::Pixel &pixel)
	: Loadable()
	, m_size(size)
	, m_pixels((m_size.x * m_size.y != 0) ? new Pixel[m_size.x * m_size.y] : nullptr)
{
	for (size_t iP = 0; iP < m_size.x * m_size.y; iP++) {
		m_pixels[iP] = pixel;
	}
}

ATD::Image::Image(const ATD::Image &other)
	: Loadable()
	, m_size(other.m_size)
	, m_pixels((m_size.x * m_size.y != 0) ? new Pixel[m_size.x * m_size.y] : nullptr)
{
	::memcpy(static_cast<void *>(m_pixels), 
			static_cast<const void *>(other.m_pixels), 
			m_size.x * m_size.y * sizeof(Pixel));
}

ATD::Image::~Image()
{
	delete [] m_pixels;
}

ATD::Image &ATD::Image::operator=(const ATD::Image &other)
{
	size_t newSize = other.m_size.x * other.m_size.y;
	Pixel *newPixels = newSize ? new Pixel[newSize] : nullptr;

	delete [] m_pixels;
	m_pixels = newPixels;
	m_size = other.m_size;

	return *this;
}

ATD::Pixel ATD::Image::getPixel(const ATD::Vector2L &position, 
		bool repeat) const
{
	if (repeat) {
		if (m_size.x * m_size.y != 0) {
			Vector2L p(position);

			/* Both non-zero, division acceptable */
			p.x %= m_size.x;
			p.x += p.x < 0 ? m_size.x : 0;

			p.y %= m_size.y;
			p.y += p.y < 0 ? m_size.y : 0;

			return m_pixels[p.y * m_size.x + p.x];
		} else {
			return Pixel();
		}
	} else {
		return RectL(m_size).contains(position) ? 
			m_pixels[position.y * m_size.x + position.x] : Pixel();
	}
}

bool ATD::Image::operator==(const ATD::Image &other) const
{
	if (m_size != other.m_size) { return false; }
	for (size_t iP = 0; iP < m_size.x * m_size.y; iP++) {
		if (m_pixels[iP] != other.m_pixels[iP]) { return false; }
	}
	return true;
}

bool ATD::Image::operator!=(const ATD::Image &other) const
{
	return !(operator==(other));
}

void ATD::Image::clear(const ATD::Pixel &pixel)
{
	for (size_t iP = 0; iP < m_size.x * m_size.y; iP++) {
		m_pixels[iP] = pixel;
	}
}

void ATD::Image::draw(const ATD::Vector2L &position, 
		const ATD::Pixel &pixel)
{
	ImageImpl::drawCell<Pixel>(
			m_size, m_pixels, 
			position, 
			pixel
			);
}

void ATD::Image::draw(const ATD::Vector2L &position, 
			const ATD::Image &texture, 
			ATD::Image::MixerFunc mixer)
{
	ImageImpl::drawImage<Pixel>(
			m_size, m_pixels, 
			position, 
			texture.m_size, texture.m_pixels, 
			mixer ? mixer : 
				[this](const Pixel &dst, const Pixel &src) {
					return this->mixOpacity(dst, src);
				}
			);
}

void ATD::Image::draw(const ATD::Vector2L &position, 
			const ATD::Image &texture, 
			const ATD::RectL &bounds, 
			bool repeat, 
			ATD::Image::MixerFunc mixer)
{
	ImageImpl::drawBounded<Pixel>(
			m_size, m_pixels, 
			position, 
			texture.m_size, texture.m_pixels, 
			bounds, 
			repeat, 
			mixer ? mixer : 
				[this](const Pixel &dst, const Pixel &src) {
					return this->mixOpacity(dst, src);
				}
			);
}

void ATD::Image::draw(const ATD::Image::Drawable &drawable)
{
	drawable.drawSelf(*this);
}

void ATD::Image::onLoad(const ATD::Fs::Path &path)
{
	/* Open file for reading */
	std::string pathStr = path.native();
	FILE *file = ::fopen(pathStr.c_str(), "rb");
	if (!file) {
		int errnoVal = errno;
		throw std::runtime_error(Aux::printf("'fopen(%s, \"r\")' failure: %d %s", 
					pathStr.c_str(), errnoVal, ::strerror(errnoVal)));
	}

	/* PNG */
	if (isPng(file)) {
		try {
			loadAsPng(file);
			::fclose(file);
			return;
		} catch (const std::exception &e) {
			::fclose(file);
			throw std::runtime_error(Aux::printf("file '%s' as png: %s", 
						pathStr.c_str(), e.what()));
		}
	}

	/* JPEG */
	if (isJpeg(file)) {
		try {
			loadAsJpeg(file);
			::fclose(file);
			return;
		} catch (const std::exception &e) {
			::fclose(file);
			throw std::runtime_error(Aux::printf("file '%s' as jpeg: %s", 
						pathStr.c_str(), e.what()));
		}
	}

	if (isGif(file)) {
		::fclose(file);
		try {
			AnimatedGif gif;
			gif.load(path);
			operator=(*gif.framePtr(0));

			/* File already closed */
			return;
		} catch (const std::exception &e) {
			throw std::runtime_error(Aux::printf("file '%s' as gif: %s", 
						pathStr.c_str(), e.what()));
		}

		/* Open file again */
		file = ::fopen(pathStr.c_str(), "rb");
		if (!file) {
			int errnoVal = errno;
			throw std::runtime_error(Aux::printf(
						"'fopen(%s, \"r\")' failure: %d %s", 
						pathStr.c_str(), errnoVal, ::strerror(errnoVal)));
		}
	}

	/* FIXME: Shall I add more formats? Webp? Bmp? */

	::fclose(file);
	throw std::runtime_error(Aux::printf("file '%s' has unknown format", 
				pathStr.c_str()));
}

void ATD::Image::onSave(const ATD::Fs::Path &path) const
{
	/* Open file for writing */
	std::string pathStr = path.native();
	FILE *file = ::fopen(pathStr.c_str(), "wb");
	if (!file) {
		int errnoVal = errno;
		throw std::runtime_error(Aux::printf("'fopen(%s, \"w\")' failure: %d %s", 
					pathStr.c_str(), errnoVal, ::strerror(errnoVal)));
	}

	size_t extension = path.extensionFromList(EXTENSIONS);
	switch (extension) {
		case EXT_GIF:
			{
				/* GIF */
				::fclose(file);
				try {
					AnimatedGif gif(*this, m_size, 
							std::vector<Vector2L>(1, Vector2L(0, 0)));
					gif.save(path);
				} catch (const std::exception &e) {
					throw std::runtime_error(Aux::printf("file '%s' as gif: %s", 
								pathStr.c_str(), e.what()));
				}
			}
			break;

		case EXT_JPEG:
		case EXT_JPG:
			{
				/* JPEG */
				try {
					saveAsJpeg(file);
					::fclose(file);
					return;
				} catch (const std::exception &e) {
					::fclose(file);
					throw std::runtime_error(Aux::printf("file '%s' as jpeg: %s", 
								pathStr.c_str(), e.what()));
				}
			}
			break;

		case EXT_PNG:
		default:
			{
				/* PNG as default */
				try {
					saveAsPng(file);
					::fclose(file);
					return;
				} catch (const std::exception &e) {
					::fclose(file);
					throw std::runtime_error(Aux::printf("file '%s' as png: %s", 
								pathStr.c_str(), e.what()));
				}
			}
	}
}

ATD::Pixel ATD::Image::mixOpacity(const ATD::Pixel &dst, 
		const ATD::Pixel &src) const
{
	return Pixel(
			dst.r * (0xFF - src.a) + src.r * src.a, 
			dst.g * (0xFF - src.a) + src.g * src.a, 
			dst.b * (0xFF - src.a) + src.b * src.a, 
			(static_cast<unsigned>(dst.a) + 
				static_cast<unsigned>(src.a) > 0xFF) ?
			0xFF : dst.a + src.a
			);
}

/* Png check/load/save implementation */

bool ATD::Image::isPng(FILE *file) const
{
	/* Remember the file offset */
	long offset = ::ftell(file);
	if (offset == -1) {
		int errnoVal = errno;
		throw std::runtime_error(Aux::printf("'ftell(..)' failure: %d %s", 
					errnoVal, ::strerror(errnoVal)));
	}

	/* Read file header and check it with 'png_sig_cmp()' */
	const size_t pngHeaderSize = 8;
	std::string header(pngHeaderSize + 1, '\0');
	size_t readResult = ::fread(&header[0], 1, pngHeaderSize, file);
	IGNORE_UNUSED(readResult);
	if (::png_sig_cmp(reinterpret_cast<png_bytep>(&header[0]), 0, 
				pngHeaderSize)) {
		/* Rewind the file as if there was no read */
		::fseek(file, offset, SEEK_SET);
		return false;
	}

	/* Rewind the file as if there was no read */
	::fseek(file, offset, SEEK_SET);
	return true;
}

void ATD::Image::loadAsPng(FILE *file)
{
	/* Init png read structures */
	png_structp pngPtr = ::png_create_read_struct(PNG_LIBPNG_VER_STRING, 
			nullptr, nullptr, nullptr);
	if (!pngPtr) {
		int errnoVal = errno;
		throw std::runtime_error(Aux::printf("'%s(..)' failure: %d %s", 
					"png_create_read_struct", errnoVal, 
					::strerror(errnoVal)));
	}
	png_infop infoPtr = ::png_create_info_struct(pngPtr);
	if (!infoPtr) {
		int errnoVal = errno;
		::png_destroy_read_struct(&pngPtr, nullptr, nullptr);
		throw std::runtime_error(Aux::printf("'%s(..)' failure: %d %s", 
					"png_create_info_struct", errnoVal, 
					::strerror(errnoVal)));
	}
	if (::setjmp(png_jmpbuf(pngPtr))) {
		int errnoVal = errno;
		::png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
		throw std::runtime_error(Aux::printf("libpng internal error: %d %s", 
					errnoVal, ::strerror(errnoVal)));
	}
	::png_init_io(pngPtr, file);

	/* Read png info */
	::png_read_info(pngPtr, infoPtr);

	Vector2S newSize(
			static_cast<size_t>(::png_get_image_width(pngPtr, infoPtr)), 
			static_cast<size_t>(::png_get_image_height(pngPtr, infoPtr))
			);

	png_byte colorType = ::png_get_color_type(pngPtr, infoPtr);
	png_byte bitDepth = ::png_get_bit_depth(pngPtr, infoPtr);

	if (bitDepth == 16) {
		::png_set_strip_16(pngPtr);
	}
	if (colorType == PNG_COLOR_TYPE_PALETTE) {
		::png_set_palette_to_rgb(pngPtr);
	}
	if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8) {
		::png_set_expand_gray_1_2_4_to_8(pngPtr);
	}
	if (::png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS)) {
		::png_set_tRNS_to_alpha(pngPtr);
	}
	if (colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_GRAY || 
			colorType == PNG_COLOR_TYPE_PALETTE) {
		::png_set_filler(pngPtr, 0xFF, PNG_FILLER_AFTER);
	}
	if (colorType == PNG_COLOR_TYPE_GRAY || 
			colorType == PNG_COLOR_TYPE_GRAY_ALPHA) {
		::png_set_gray_to_rgb(pngPtr);
	}

	::png_read_update_info(pngPtr, infoPtr);

	/* Read the image */
	Pixel *newPixels = nullptr;
	if (newSize.x * newSize.y != 0) {
		newPixels = new Pixel[newSize.x * newSize.y];

		png_bytep *rowPtrs = new png_bytep[newSize.y];
		for (size_t iY = 0; iY < newSize.y; iY++) {
			rowPtrs[iY] = new png_byte[::png_get_rowbytes(pngPtr, infoPtr)];
		}

		::png_read_image(pngPtr, rowPtrs);

		for (size_t iY = 0; iY < newSize.y; iY++) {
			for (size_t iX = 0; iX < newSize.x; iX++) {
				png_bytep px = &(rowPtrs[iY][iX * 4]);
				newPixels[iY * newSize.x + iX] = 
					Pixel(px[0], px[1], px[2], px[3]);
			}
			delete [] rowPtrs[iY];
		}

		delete [] rowPtrs;
	}

	/* Cleanup png structures */
	::png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);

	/* Replace the current image with new one */
	delete [] m_pixels;
	m_size = newSize;
	m_pixels = newPixels;
}

void ATD::Image::saveAsPng(FILE *file) const
{
	/* Init png write structures */
	png_structp pngPtr = ::png_create_write_struct(PNG_LIBPNG_VER_STRING, 
			nullptr, nullptr, nullptr);
	if (!pngPtr) {
		int errnoVal = errno;
		throw std::runtime_error(Aux::printf("'%s(..)' failure: %d %s", 
					"png_create_write_struct", errnoVal, 
					::strerror(errnoVal)));
	}
	png_infop infoPtr = ::png_create_info_struct(pngPtr);
	if (!infoPtr) {
		int errnoVal = errno;
		::png_destroy_write_struct(&pngPtr, nullptr);
		throw std::runtime_error(Aux::printf("'%s(..)' failure: %d %s", 
					"png_create_info_struct", errnoVal, 
					::strerror(errnoVal)));
	}
	if (::setjmp(png_jmpbuf(pngPtr))) {
		int errnoVal = errno;
		::png_destroy_write_struct(&pngPtr, &infoPtr);
		throw std::runtime_error(Aux::printf("libpng internal error: %d %s", 
					errnoVal, ::strerror(errnoVal)));
	}
	::png_init_io(pngPtr, file);

	/* Set png info */
	::png_set_IHDR(pngPtr, infoPtr, 
			static_cast<png_uint_32>(m_size.x), 
			static_cast<png_uint_32>(m_size.y), 
			8, PNG_COLOR_TYPE_RGBA, 
			PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, 
			PNG_FILTER_TYPE_DEFAULT);

	/* Write the image */
	png_bytep data = new png_byte[m_size.x * m_size.y * 4];
	for (size_t iP = 0; iP < m_size.x * m_size.y; iP++) {
		data[iP * 4 + 0] = static_cast<png_byte>(m_pixels[iP].r);
		data[iP * 4 + 1] = static_cast<png_byte>(m_pixels[iP].g);
		data[iP * 4 + 2] = static_cast<png_byte>(m_pixels[iP].b);
		data[iP * 4 + 3] = static_cast<png_byte>(m_pixels[iP].a);
	}
	png_bytep *rowPtrs = new png_bytep[m_size.y];
	for (size_t iY = 0; iY < m_size.y; iY++) {
		rowPtrs[iY] = &data[iY * m_size.x * 4];
	}

	::png_set_rows(pngPtr, infoPtr, rowPtrs);
	::png_write_png(pngPtr, infoPtr, PNG_TRANSFORM_IDENTITY, nullptr);
	::png_write_end(pngPtr, infoPtr);

	delete [] rowPtrs;
	delete [] data;

	/* Cleanup png structures */
	::png_destroy_write_struct(&pngPtr, &infoPtr);
}

bool ATD::Image::isJpeg(FILE *file) const
{
	/* Remember the file offset */
	long offset = ::ftell(file);
	if (offset == -1) {
		int errnoVal = errno;
		throw std::runtime_error(Aux::printf("'ftell(..)' failure: %d %s", 
					errnoVal, ::strerror(errnoVal)));
	}

	/* Init jpeg read structures */
	jpeg_decompress_struct info;
	CustomJpegErrorMgr error;
	info.err = ::jpeg_std_error(&error.pub);
	error.pub.output_message = _customJpegOutputMessage;
	error.pub.error_exit = _customJpegErrorExit;
	if (::setjmp(error.jmp)) {
		/* Not a valid jpeg header. */
		::jpeg_destroy_decompress(&info);

		/* Rewind the file as if there was no read */
		::fseek(file, offset, SEEK_SET);
		return false;
	}

	/* Read jpeg header */
	::jpeg_create_decompress(&info);
	::jpeg_stdio_src(&info, file);
	::jpeg_read_header(&info, true);
	::jpeg_destroy_decompress(&info);

	/* Rewind the file as if there was no read */
	::fseek(file, offset, SEEK_SET);
	return true;
}

void ATD::Image::loadAsJpeg(FILE *file)
{
	/* Init jpeg read structures */
	jpeg_decompress_struct info;
	CustomJpegErrorMgr error;
	info.err = ::jpeg_std_error(&error.pub);
	error.pub.output_message = _customJpegOutputMessage;
	error.pub.error_exit = _customJpegErrorExit;
	if (::setjmp(error.jmp)) {
		/* Here the decompress was already tried, an error happened 
		 * and jump to this place occured. Just a reminder, that C 
		 * is a macroassembler indeed. */
		::jpeg_destroy_decompress(&info);
		throw std::runtime_error(Aux::printf("failed to load: %s", 
					error.str.c_str()));
	}

	::jpeg_create_decompress(&info);

	/* Read jpeg info */

	/* Note:
	 * 'file' must be open in binary ("b") mode. Otherwise will be 
	 * problems on non-Unix systems.
	 * http:uw714doc.sco.com/en/jpeg/libjpeg.txt */

	::jpeg_stdio_src(&info, file);
	::jpeg_read_header(&info, true);

	info.out_color_components = 4;
	info.out_color_space = JCS_EXT_RGBA;

	/* Read the image */
	::jpeg_start_decompress(&info);
	{
		Vector2S newSize = Vector2S(info.output_width, info.output_height);
		Pixel *newPixels = new Pixel[newSize.x * newSize.y];

		JSAMPROW rowPtr;
		while (info.output_scanline < info.output_height) {
			rowPtr = reinterpret_cast<JSAMPLE *>(
					&newPixels[newSize.x * info.output_scanline]);
			::jpeg_read_scanlines(&info, &rowPtr, 
					info.output_height - info.output_scanline);
		}

		delete [] m_pixels;
		m_size = newSize;
		m_pixels = newPixels;
	}
	::jpeg_finish_decompress(&info);

	/* Cleanup jpeg structures */
	::jpeg_destroy_decompress(&info);
}

void ATD::Image::saveAsJpeg(FILE *file) const
{
	/* Init jpeg write structures */
	jpeg_compress_struct info;
	CustomJpegErrorMgr error;
	info.err = ::jpeg_std_error(&error.pub);
	error.pub.output_message = _customJpegOutputMessage;
	error.pub.error_exit = _customJpegErrorExit;
	if (::setjmp(error.jmp)) {
		/* Here the compress was already tried, an error happened 
		 * and jump to this place occured. */
		::jpeg_destroy_compress(&info);
		throw std::runtime_error(Aux::printf("failed to save: %s", 
					error.str.c_str()));
	}

	::jpeg_create_compress(&info);

	/* Set jpeg info */
	::jpeg_stdio_dest(&info, file);

	info.image_width = m_size.x;
	info.image_height = m_size.y;
	info.input_components = 4;
	info.in_color_space = JCS_EXT_RGBX;

	::jpeg_set_defaults(&info);
	::jpeg_set_quality(&info, 100, true);

	/* Write the image */
	::jpeg_start_compress(&info, true);
	{
		JSAMPROW rowPtr;
		while (info.next_scanline < info.image_height) {
			rowPtr = reinterpret_cast<JSAMPLE *>(
					&m_pixels[m_size.x * info.next_scanline]);
			::jpeg_write_scanlines(&info, &rowPtr, 1);
		}
	}
	::jpeg_finish_compress(&info);

	/* Cleanup jpeg structures */
	::jpeg_destroy_compress(&info);
}

bool ATD::Image::isGif(FILE *file) const
{
	size_t headerLen = 6;
	const std::string gif87 = "GIF87A";
	const std::string gif89 = "GIF89A";

	/* Remember the file offset */
	long offset = ::ftell(file);
	if (offset == -1) {
		int errnoVal = errno;
		throw std::runtime_error(Aux::printf("'ftell(..)' failure: %d %s", 
					errnoVal, ::strerror(errnoVal)));
	}

	/* Read GIF header */
	std::string header(headerLen + 1, '\0');
	size_t readResult = ::fread(&header[0], 1, headerLen, file);
	header.resize(headerLen);

	/* Rewind the file as if there was no read */
	::fseek(file, offset, SEEK_SET);

	if (readResult < headerLen) { return false; }
	return (header == gif87 || header == gif89);
}


