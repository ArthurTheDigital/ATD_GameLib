/**
 * @file      
 * @brief     Animated gif handling.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Graphics/AnimatedGif.hpp>

#include <ATD/Core/Printf.hpp>

#include <errno.h>
#include <gif_lib.h>
#include <stdio.h>
#include <string.h>

#include <map>
#include <stdexcept>


/* ATD::AnimatedGif auxiliary: */

static void _fillPixel(ATD::Pixel *dstData, const ATD::Vector2S &dstSize, 
		const ATD::Pixel &pixel, const ATD::RectL &area)
{
	for (long y = area.y; y < area.y + area.h; y++) {
		for (long x = area.x; x < area.x + area.w; x++) {
			dstData[y * dstSize.x + x] = pixel;
		}
	}
}

static int _interlacedToNormal(int pxId, int width, int height)
{
	int pass1Border = (height + 7) / 8 * width;
	int pass2Border = pass1Border + (height + 7 - 4) / 8 * width;
	int pass3Border = pass2Border + (height + 3 - 2) / 4 * width;
	/* int pass4Border = pass3Border + (height + 1 - 1) / 2 * width; */

	int pxResId;
	if (pxId < pass1Border) {
		pxResId = (pxId - 0) / width * (8 * width) + 
			(pxId - 0) % width;
	} else if (pxId < pass2Border) {
		pxResId = (pxId - pass1Border) / width * (8 * width) + 
			4 * width + (pxId - pass1Border) % width;
	} else if (pxId < pass3Border) {
		pxResId = (pxId - pass2Border) / width * (4 * width) + 
			2 * width + (pxId - pass2Border) % width;
	} else { /* pxId < pass4Border */
		pxResId = (pxId - pass3Border) / width * (2 * width) + 
			1 * width + (pxId - pass3Border) % width;
	}
	return pxResId;
}

static uint16_t _colorDistanceRGB(const ATD::Pixel &c1, const ATD::Pixel &c2)
{
	/* Manhettan distance between pixel values (transparency ignored) */
	return (c1.r < c2.r ? c2.r - c1.r : c1.r - c2.r) + 
			(c1.g < c2.g ? c2.g - c1.g : c1.g - c2.g) + 
			(c1.b < c2.b ? c2.b - c1.b : c1.b - c2.b);
}

static const uint16_t MAX_COLOR_DISTANCE = 0x00FF * 3;
static const uint16_t DFT_COLOR_DISTANCE = 0xFFFF; /* Impossible value */

static std::vector<ATD::Pixel> _extractColormap(const ATD::Image &image, 
		size_t maxColormapSize = ATD::AnimatedGif::MAX_COLORMAP_SIZE)
{
	/* Trivial case */
	if (!maxColormapSize) { return std::vector<ATD::Pixel>(); }

	/* This algorithm needs a color descriptor (used to decide, which colors 
	 * are more important) */
	struct CDesc
	{
		CDesc()
			: weight(0), nnDistance(DFT_COLOR_DISTANCE), nnValue(0x0)
		{}

		size_t weight;			/* Number of pixels of this color */
		uint16_t nnDistance;	/* RGB distance to the nearest neighbour */
		uint32_t nnValue;		/* Nearest neighbour color value */
	};

	std::map<uint32_t, CDesc> rawColormap;

	/* Obtain color values and descriptors */
	for (size_t pxIter = 0; pxIter < image.size().x * image.size().y; 
			pxIter++) {
		ATD::Pixel nextPixel = image.data()[pxIter];
		nextPixel.a = 0xFF;

		auto rcmIter = rawColormap.find(nextPixel.value);
		if (rcmIter != rawColormap.end()) {
			/* Color already included */
			rcmIter->second.weight += 1;
		} else {
			/* New color value */
			CDesc nextDesc;
			nextDesc.weight = 1;

			/* Must calculate nearest neighbour params */
			for (auto rcmIter2 = rawColormap.begin(); 
					rcmIter2 != rawColormap.end(); rcmIter2++) {
				/* No self-comparison possible, because, color vales are 
				 * unique as std::map keys and the next one is not yet 
				 * inserted. */
				uint16_t nextDistance = _colorDistanceRGB(nextPixel, 
						ATD::Pixel(rcmIter2->first));
				if (nextDistance < rcmIter2->second.nnDistance) {
					rcmIter2->second.nnDistance = nextDistance;
					rcmIter2->second.nnValue = nextPixel.value;
				}
				if (nextDistance < nextDesc.nnDistance) {
					nextDesc.nnDistance = nextDistance;
					nextDesc.nnValue = rcmIter2->first;
				}
			}

			rawColormap.insert(std::pair<uint32_t, CDesc>(nextPixel.value, 
						nextDesc));
		}
	}

	/* Reduce the colormap color by color */ /* FIXME: Can be optimized? */
	while (rawColormap.size() > maxColormapSize) {
		/* Function, used to decide, how important the color is */
		std::function<float(const CDesc &)> importanceFunc = 
			[image](const CDesc &desc)->float {
				return static_cast<float>(desc.weight) / 
						static_cast<float>(image.size().x * image.size().y) + 
					static_cast<float>(desc.nnDistance) / 
						static_cast<float>(MAX_COLOR_DISTANCE);
			};

		/* Find the cut candidate - color value with the least importance */
		auto rcmIter = rawColormap.begin(); /* != .end(), because 
											 * .size() > maxColormapSize >= 
											 * 0 */
		uint32_t ccValue = rcmIter->first; /* cc for cut candidate */
		float ccImportance = importanceFunc(rcmIter->second);
		rcmIter++;

		for (;rcmIter != rawColormap.end(); rcmIter++) {
			float nextImportance = importanceFunc(rcmIter->second);
			if (nextImportance < ccImportance) {
				ccImportance = nextImportance;
				ccValue = rcmIter->first;
			}
		}

		auto ccRcmIter = rawColormap.find(ccValue); /* != .end()*/

		/* Give the cut candidate's weight to it's nearest neighbour */
		if (ccRcmIter->second.nnDistance != DFT_COLOR_DISTANCE) {
			rawColormap[ccRcmIter->second.nnValue].weight += 
				ccRcmIter->second.weight;
		}

		/* Erase the cut candidate */
		rawColormap.erase(ccRcmIter);

		/* Recalculate nearest neighbours */
		for (rcmIter = rawColormap.begin(); rcmIter != rawColormap.end(); 
				rcmIter++) {
			if (rcmIter->second.nnValue == ccValue) {
				rcmIter->second.nnDistance = DFT_COLOR_DISTANCE;
				for (auto rcmIter2 = rawColormap.begin(); 
						rcmIter2 != rawColormap.end(); rcmIter2++) {
					if (rcmIter2->first != rcmIter->first) {
						uint16_t distance = _colorDistanceRGB(
								ATD::Pixel(rcmIter->first), 
								ATD::Pixel(rcmIter2->first));
						if (distance < rcmIter->second.nnDistance) {
							rcmIter->second.nnDistance = distance;
							rcmIter->second.nnValue = rcmIter2->first;
							break;
						}
					}
				}
			}
		}
	}

	/* Export the colormap */
	std::vector<ATD::Pixel> colormap;
	for (auto &color : rawColormap) {
		colormap.push_back(ATD::Pixel(color.first));
	}
	return colormap;
}

static uint8_t _indexInColormap(const ATD::Pixel &pixel, 
		const std::vector<ATD::Pixel> &colorMap)
{
	uint8_t index = 0;
	uint16_t pxDistance = 0xFFFF; 
	for (size_t pxIndex = 0; pxIndex < colorMap.size(); pxIndex++) {
		uint16_t curPxDistance = _colorDistanceRGB(pixel, colorMap[pxIndex]);
		if (curPxDistance < pxDistance) {
			index = pxIndex;
			pxDistance = curPxDistance;
		}
	}
	return index;
}

/* ATD::AnimatedGif::Player */

ATD::AnimatedGif::Player::Player(ATD::AnimatedGif::CPtr gifPtr)
	: m_gifPtr(gifPtr)
	, m_curFrameIndex(0)
	, m_curFramePlayedMs(0)
	, m_playedMs(0)
{}

void ATD::AnimatedGif::Player::reset()
{
	m_curFrameIndex = 0;
	m_curFramePlayedMs = 0;
	m_playedMs = 0;
}

void ATD::AnimatedGif::Player::update(long elapsedMs)
{
	if (m_gifPtr->framesCount()) {
		elapsedMs %= m_gifPtr->durationMs();
		if (elapsedMs < 0) { elapsedMs += m_gifPtr->durationMs(); }

		long curFramePlayedMs = m_curFramePlayedMs + elapsedMs;
		size_t prevFrameIndex = static_cast<long>(m_curFrameIndex) - 1 < 0 ? 
			m_gifPtr->framesCount() - 1 : m_curFrameIndex - 1;
		while (curFramePlayedMs >= 0) {
			prevFrameIndex = m_curFrameIndex;
			m_curFrameIndex += 1;
			m_curFrameIndex %= m_gifPtr->framesCount();
			curFramePlayedMs -= m_gifPtr->frameDelayMs(prevFrameIndex);
		}
		m_curFrameIndex = prevFrameIndex;
		m_curFramePlayedMs = curFramePlayedMs + 
			m_gifPtr->frameDelayMs(prevFrameIndex);

		m_playedMs += elapsedMs;
		m_playedMs %= m_gifPtr->durationMs();
	}
}

void ATD::AnimatedGif::Player::setPlayedMs(size_t playedMs)
{
	playedMs %= m_gifPtr->durationMs();

	m_curFrameIndex = 0;
	m_curFramePlayedMs = 0;
	m_playedMs = 0;
	update(static_cast<long>(playedMs));
}


/* ATD::AnimatedGif constants: */

const size_t ATD::AnimatedGif::DFT_DELAY_MS = 100;
const size_t ATD::AnimatedGif::MAX_DELAY_MS = (size_t)0xFFFF * 10;

const size_t ATD::AnimatedGif::MAX_COLORMAP_SIZE = 256;


/* ATD::AnimatedGif */

ATD::AnimatedGif::AnimatedGif()
	: Loadable()
	, m_size()
	, m_frames()
	, m_duration(0)
{}

ATD::AnimatedGif::AnimatedGif(const ATD::Image &spriteSheet, 
		const ATD::Vector2S &frameSize, 
		const std::vector<ATD::Vector2L> &framePositions, 
		ATD::Pixel background, 
		size_t delayMs)
	: Loadable()
	, m_size(frameSize)
	, m_frames()
{
	for (auto &framePos : framePositions) {
		Frame nextFrame;
		nextFrame.imgPtr = Image::Ptr(new Image(m_size, Pixel(background.r, 
						background.g, background.b)));
		nextFrame.imgPtr->draw(Vector2L(), spriteSheet, RectL(framePos, 
					m_size));
		nextFrame.delayMs = delayMs;

		m_frames.push_back(nextFrame);
	}
}

ATD::AnimatedGif::AnimatedGif(const std::vector<ATD::Image::CPtr> &frames, 
		ATD::Align alignX, ATD::Align alignY, 
		ATD::Pixel background, 
		size_t delayMs)
	: Loadable()
	, m_size() /* (0, 0) */
	, m_frames()
{
	for (auto &frame : frames) {
		if (frame->size().x > m_size.x) { m_size.x = frame->size().x; }
		if (frame->size().y > m_size.y) { m_size.y = frame->size().y; }
	}

	for (auto &frame : frames) {
		Frame nextFrame;
		nextFrame.imgPtr = Image::Ptr(new Image(m_size, Pixel(background.r, 
						background.g, background.b)));
		RectL bounds = RectL(frame->size()).alignClamped(RectL(m_size), 
					alignX, alignY);
		nextFrame.imgPtr->draw(bounds.pos(), *frame);
		nextFrame.delayMs = delayMs;

		m_frames.push_back(nextFrame);
	}
}

ATD::AnimatedGif::AnimatedGif(const ATD::AnimatedGif &other)
	: Loadable()
	, m_size(other.m_size)
	, m_frames()
{
	for (auto &frame : other.m_frames) {
		Frame nextFrame;
		nextFrame.imgPtr = Image::Ptr(new Image(*frame.imgPtr));
		nextFrame.delayMs = frame.delayMs;
		m_frames.push_back(nextFrame);
	}
}

ATD::AnimatedGif::~AnimatedGif()
{}

void ATD::AnimatedGif::setFrameDelayMs(size_t frameIndex, 
		size_t delayMs)
{
	checkFrameIndex(frameIndex);
	if (delayMs > MAX_DELAY_MS) {
		throw std::runtime_error(Aux::printf("too large delay time (%lu/%lu)", 
					delayMs, MAX_DELAY_MS));
	}
	m_frames[frameIndex].delayMs = delayMs;
}

size_t ATD::AnimatedGif::durationMs() const
{
	if (!m_duration) {
		for (auto &frame : m_frames) { m_duration += frame.delayMs; }
	}
	return m_duration;
}

void ATD::AnimatedGif::onLoad(const ATD::Fs::Path &filename)
{
	/* Read GIF data from file */
	FILE *file = ::fopen(filename.native().c_str(), "rb");
	if (!file) {
		throw std::runtime_error(Aux::printf(
					"'fopen(%s, ..)' failure: %d %s", 
					filename.native().c_str(), errno, 
					::strerror(errno)
					));
	}

	int error;
	GifFileType *info = nullptr;
	info = ::DGifOpenFileHandle(::fileno(file), &error);
	if (!info || error) {
		throw std::runtime_error(Aux::printf(
					"'DGifOpenFileHandle(..)' failure: %d %s", 
					error, ::GifErrorString(error)
					));
	}

	if (::DGifSlurp(info) == GIF_ERROR) {
		int slurpErr = info->Error;
		::DGifCloseFile(info, &error);
		throw std::runtime_error(Aux::printf(
					"'DGifSlurp(..)' failure: %d %s", 
					slurpErr, ::GifErrorString(slurpErr)
					));
	}

	/* Decode GIF */
	m_size = Vector2S(info->SWidth, info->SHeight);
	Pixel background; /* By default, black non-transparent */
	if (info->SColorMap) {
		background.r = info->SColorMap->Colors[info->SBackGroundColor].Red;
		background.g = info->SColorMap->Colors[info->SBackGroundColor].Green;
		background.b = info->SColorMap->Colors[info->SBackGroundColor].Blue;
	}

	for (int imgIter = 0; imgIter < info->ImageCount; imgIter++) {
		SavedImage &curImage(info->SavedImages[imgIter]);
		GifImageDesc &curIDesc(curImage.ImageDesc);

		/* Collect graphics control */
		uint8_t eflags = 0x00;
		uint16_t delay = 0;
		uint8_t transparent = 0;
		for (int extIter = 0; extIter < curImage.ExtensionBlockCount; 
				extIter++) {
			ExtensionBlock &curExt(curImage.ExtensionBlocks[extIter]);
			if (curExt.Function == 0xF9 && curExt.ByteCount == 4) {
				/* This is the graphics control, other extensions are not
				 * interesting. */
				::memcpy(&eflags, &curExt.Bytes[0], 1);
				::memcpy(&delay, &curExt.Bytes[1], 2);
				::memcpy(&transparent, &curExt.Bytes[3], 1);
			}
		}

		/* Apply graphics control */
		Frame nextFrame;
		/* Delay in GIF is stored in 1/100 of seconds */
		nextFrame.delayMs = delay * 10; 
		nextFrame.imgPtr = static_cast<Image::Ptr>(new Image(m_size));

		/* Prepare frame "canvas" */
		switch ((eflags & 0x1C) >> 2) { /* 0x1C == 0b00011100 */
			/* Disposal method flags occupy 3 bits, values from 0x4 to 0x7 
			 * are yet to be defined (reserved by GIF89A standard). */
			case 0x0: /* Fill background (the only option for 1st frame) */
				{
					_fillPixel(nextFrame.imgPtr->data(), m_size, 
							background, ATD::RectL(m_size));
				}
				break;

			case 0x1: /* Do not dispose */
				{
					if (m_frames.size()) {
						::memcpy(static_cast<void *>(nextFrame.imgPtr->data()), 
								static_cast<const void *>(m_frames.back().imgPtr->data()), 
								m_size.x * m_size.y * sizeof(Pixel));
					}
				}
				break;

			case 0x2: /* Dispose to background */
				{
					if (m_frames.size()) {
						::memcpy(static_cast<void *>(nextFrame.imgPtr->data()), 
								static_cast<const void *>(m_frames.back().imgPtr->data()), 
								m_size.x * m_size.y * sizeof(Pixel));
					}
					_fillPixel(nextFrame.imgPtr->data(), m_size, 
							background, 
							ATD::RectL(curIDesc.Left, curIDesc.Top, 
								curIDesc.Width, curIDesc.Height
								)
							);
				}
				break;

			case 0x3: /* Dispose to previous */
				{
					if (m_frames.size()) {
						::memcpy(static_cast<void *>(nextFrame.imgPtr->data()), 
								static_cast<const void *>(m_frames.back().imgPtr->data()), 
								m_size.x * m_size.y * sizeof(Pixel));
					}

					if (m_frames.size() > 1) {
						Frame &prevFrame(m_frames[m_frames.size() - 2]);
						for (int dY = 0; dY < curIDesc.Height; dY++) {
							size_t i = 
								(curIDesc.Top + dY) * m_size.x + 
								curIDesc.Left;

							::memcpy(static_cast<void *>(&nextFrame.imgPtr->data()[i]), 
									static_cast<const void *>(&prevFrame.imgPtr->data()[i]), 
									curIDesc.Width * sizeof(Pixel));
						}
					}
				}
				break;

			default:
				{}
				break;
		}

		/* Apply image */
		ColorMapObject *curColorMap = curIDesc.ColorMap ? curIDesc.ColorMap : 
			info->SColorMap;

		if (!curColorMap) {
			throw std::runtime_error(Aux::printf(
						"no color map for frame %u", 
						m_frames.size()
						));
		} else {
			int transparentId = eflags & 0x01 ? 
				static_cast<int>(transparent) : -1;

			for (int pxIter = 0; pxIter < curIDesc.Width * curIDesc.Height; 
					pxIter++) {

				int pxId = curIDesc.Interlace ? 
					_interlacedToNormal(pxIter, curIDesc.Width, 
							curIDesc.Height) : 
					pxIter;

				int pxDstIter = 
					(curIDesc.Top + pxIter / curIDesc.Width) * m_size.x + 
					curIDesc.Left + pxIter % curIDesc.Width;

				int colorId = static_cast<int>(curImage.RasterBits[pxId]);

				nextFrame.imgPtr->data()[pxDstIter] = Pixel(
						curColorMap->Colors[colorId].Red, 
						curColorMap->Colors[colorId].Green, 
						curColorMap->Colors[colorId].Blue, 
						colorId == transparentId ? 0x00 : 0xFF);
			}
		}

		m_frames.push_back(nextFrame);
	}

	/* Close GIF file */
	::DGifCloseFile(info, &error);
	if (error) {
		throw std::runtime_error(Aux::printf(
					"'DGifCloseFile(..)' failure: %d %s", 
					error, ::GifErrorString(error)
					));
	}
}

void ATD::AnimatedGif::onSave(const ATD::Fs::Path &filename) const
{
	/* Open file for writing and init encoder */
	FILE *file = ::fopen(filename.native().c_str(), "wb");
	if (!file) {
		throw std::runtime_error(Aux::printf(
					"'fopen(%s, ..)' failure: %d %s", 
					filename.native().c_str(), errno, 
					::strerror(errno)
					));
	}

	int error;
	GifFileType *info = nullptr;
	info = ::EGifOpenFileHandle(::fileno(file), &error);
	if (!info || error) {
		::fclose(file);
		throw std::runtime_error(Aux::printf(
					"'EGifOpenFileHandle(..)' failure: %d %s", 
					error, ::GifErrorString(error)
					));
	}

	/* Fill gif structure fields */
	info->SWidth = static_cast<GifWord>(m_size.x);
	info->SHeight = static_cast<GifWord>(m_size.y);

	info->SColorResolution = 8;
	info->AspectByte = 0;

	{
		GifColorType black;
		black.Red =   0x00;
		black.Green = 0x00;
		black.Blue =  0x00;
		/* Global colormap shall contain single color - black. */
		/* Gif color map shall have power-of-2 colors! */
		info->SColorMap = GifMakeMapObject(1, &black);
		if (!info->SColorMap) {
			/* Close file */
			::EGifCloseFile(info, nullptr);

			throw std::runtime_error("'GifMakeMapObject' failure");
		}
	}
	info->SBackGroundColor = 0;

	/* info->ImageCount is automatically set while adding images */
	for (size_t imgIter = 0; imgIter < m_frames.size(); imgIter++) {
		size_t imgSize = 
			m_frames[imgIter].imgPtr->size().x * 
			m_frames[imgIter].imgPtr->size().y;

		SavedImage img;
		img.ImageDesc.Left = 0;
		img.ImageDesc.Top = 0;
		img.ImageDesc.Width = m_frames[imgIter].imgPtr->size().x;
		img.ImageDesc.Height = m_frames[imgIter].imgPtr->size().y;
		img.ImageDesc.Interlace = false;
		img.RasterBits = nullptr;
		img.ExtensionBlockCount = 0;
		img.ExtensionBlocks = nullptr;

		try {
			std::vector<ATD::Pixel> colormap = 
				_extractColormap(*(m_frames[imgIter].imgPtr));

			/* Colormap */
			{
				/* Colormap size must be power of 2. */
				int cmSize = 1 << ::GifBitSize(colormap.size());
				GifColorType *colors = new GifColorType[cmSize];
				for (size_t cIter = 0; cIter < colormap.size(); cIter++) {
					colors[cIter].Red = colormap[cIter].r;
					colors[cIter].Green = colormap[cIter].g;
					colors[cIter].Blue = colormap[cIter].b;
				}
				for (size_t cIter = colormap.size(); 
						cIter < static_cast<size_t>(cmSize); cIter++) {
					colors[cIter].Red =   0x00;
					colors[cIter].Green = 0x00;
					colors[cIter].Blue =  0x00;
				}
				img.ImageDesc.ColorMap = ::GifMakeMapObject(cmSize, colors);
				delete [] colors;
				if (!img.ImageDesc.ColorMap) {
					throw std::runtime_error("'GifMakeMapObject' failure");
				}
			}

			/* Pixels */
			img.RasterBits = new GifByteType[imgSize];
			for (size_t pxIter = 0; pxIter < imgSize; pxIter++) {
				img.RasterBits[pxIter] = 
					_indexInColormap(
							m_frames[imgIter].imgPtr->data()[pxIter], 
							colormap);
			}

			/* Extensions */
			if (!imgIter) {
				/* First image should have NETSCAPE2.0 extension. */
				char extNetscape[] = "NETSCAPE2.0";
				if (::GifAddExtensionBlock(&img.ExtensionBlockCount, 
							&img.ExtensionBlocks, APPLICATION_EXT_FUNC_CODE, 
							::strlen(extNetscape), 
							reinterpret_cast<unsigned char *>(extNetscape)
							) != GIF_OK) {
					throw std::runtime_error(
							"'GifAddExtensionBlock' failure");
				}
			}

			if (!::GifMakeSavedImage(info, &img)) {
				throw std::runtime_error("'GifMakeSavedImage' failure");
			}
		} catch (const std::exception &e) {
			/* Deinit img */
			::GifFreeExtensions(&img.ExtensionBlockCount, 
					&img.ExtensionBlocks);
			delete [] img.RasterBits;
			::GifFreeMapObject(img.ImageDesc.ColorMap);

			/* Close file */
			::EGifCloseFile(info, nullptr);

			throw std::runtime_error(Aux::printf("on image %lu: %s", imgIter, 
						e.what()));
		}

		/* Deinit img */
		::GifFreeExtensions(&img.ExtensionBlockCount, 
				&img.ExtensionBlocks);
		delete [] img.RasterBits;
		::GifFreeMapObject(img.ImageDesc.ColorMap);
	}

	/* Set graphic control extensions */
	for (size_t imgIter = 0; imgIter < m_frames.size(); imgIter++) {
		GraphicsControlBlock gcb;
		gcb.DisposalMode = DISPOSE_DO_NOT;
		gcb.UserInputFlag = false;
		gcb.DelayTime = static_cast<int>(m_frames[imgIter].delayMs) / 10;
		gcb.TransparentColor = NO_TRANSPARENT_COLOR;

		if (::EGifGCBToSavedExtension(&gcb, info, 
					static_cast<int>(imgIter)) != GIF_OK) {
			/* Close file */
			::EGifCloseFile(info, nullptr);

			throw std::runtime_error(Aux::printf(
						"'EGifGCBToSavedExtension' failure: %s", 
						::GifErrorString(info->Error)));
		}
	}

	info->ExtensionBlockCount = 0;
	info->ExtensionBlocks = nullptr;

	/* info->Image is ignored. I guess, it is only used for playing gif, 
	 * and I need only saving here. */

	/* Spew */
	if (::EGifSpew(info) == GIF_ERROR) {
		int spewErr = info->Error;
		::EGifCloseFile(info, nullptr);
		throw std::runtime_error(Aux::printf(
					"'EGifSpew(..)' failure: %d %s", 
					spewErr, ::GifErrorString(spewErr)
					));
	}

	/* Close file */
	::EGifCloseFile(info, &error);
	if (error) {
		throw std::runtime_error(Aux::printf(
					"'EGifCloseFile(..)' failure: %d %s", 
					error, ::GifErrorString(error)
					));
	}
}

void ATD::AnimatedGif::checkFrameIndex(size_t frameIndex) const
{
	if (frameIndex >= m_frames.size()) {
		throw std::runtime_error(ATD::Aux::printf("out of range (access %lu/%lu)", 
					frameIndex, m_frames.size()));
	}
}


