/**
 * @file     
 * @brief    Unicode processing implementation.
 * @details  License: GPL v3.
 * @author   ArthurTheDigital (arthurthedigital@gmail.com)
 * @since    $Id: $ */

#include <ATD/Core/Unicode.hpp>

#include <ATD/Core/Printf.hpp>

#include <string.h>


/* ATD::Unicode auxiliary */

const std::vector<uint8_t> UTF8_HEAD_MASKS = {
	0x00 << 7, 
	0x06 << 5, 
	0x0e << 4, 
	0x1e << 3
};

static size_t Utf8Width(char c)
{
	uint8_t byte = *reinterpret_cast<uint8_t*>(&c);

	if (byte >> 7 == 0x00) { return 1; }
	if (byte >> 5 == 0x06) { return 2; }
	if (byte >> 4 == 0x0e) { return 3; }
	if (byte >> 3 == 0x1e) { return 4; }

	return static_cast<size_t>(-1);
}

static size_t UnicodeWidth(uint32_t u)
{
	if (u < 0x00000080) { return 1; }
	if (u < 0x00000800) { return 2; }
	if (u < 0x00010000) { return 3; }
	if (u < 0x00110000) { return 4; }

	return static_cast<size_t>(-1);
}

// TODO: Ptr -> C++ reference

/* *utfStringPtr is to be a valid pointer, 
 * *lengthLeftPtr is to be > 0. */
static uint32_t UnicodeFromUtf8(const char **utfStringPtr, 
		size_t *lengthLeftPtr, 
		bool *failBitPtr)
{
	size_t width = Utf8Width(**utfStringPtr);
	if (width == static_cast<size_t>(-1) || width > *lengthLeftPtr) {
		*failBitPtr = true;

		/* Bad UTF8 character. Not critical, put zero instead. */
		*lengthLeftPtr -= 1;
		*utfStringPtr += 1 * sizeof(char);
		return 0;
	} else {
		/* Save pointer to the character. */
		const char *str = *utfStringPtr;

		/* Update the utf8 string 'iterators'. */
		*lengthLeftPtr -= width;
		*utfStringPtr += width * sizeof(char);

		/* Do UTF8 -> Unicode conversion. */
		// TODO: Redo!
		const uint8_t utfBits0 = 
			*reinterpret_cast<const uint8_t*>(&str[0]);

		uint32_t unicode = 
			static_cast<uint32_t>(
				utfBits0 & ~UTF8_HEAD_MASKS[width - 1]);

		for (size_t extIter = 1; extIter < width; extIter++) {
			const uint8_t utfBitsExt = 
				*reinterpret_cast<const uint8_t*>(&str[extIter]);

			unicode = 
				(unicode << 6) | 
				static_cast<uint32_t>(utfBitsExt & ~(0x80));
		}
		return unicode;
	}
}

/* *uniStringPtr is to be a valid pointer, 
 * *lengthLeftPtr is to be > 0. */
std::string Utf8FromUnicode(const uint32_t **uniStringPtr, 
		size_t *lengthLeftPtr, 
		bool *failBitPtr)
{
	/* Save the current unicode value. */
	uint32_t unicode = **uniStringPtr;

	/* Update the unicode string 'iterators'. */
	*lengthLeftPtr -= 1;
	*uniStringPtr += 1 * sizeof(uint32_t);

	/* Do Unicode -> UTF8 conversion. */
	size_t width = UnicodeWidth(unicode);

	if (width == static_cast<size_t>(-1)) {
		*failBitPtr = true;
		return std::string(1, '\0');
	}

	std::string utf(width, '\0');
	if (width == 1) {
		*reinterpret_cast<uint8_t*>(&utf[0]) |= static_cast<uint8_t>(unicode);
	} else {
		*reinterpret_cast<uint8_t*>(&utf[0]) |= 
			0x80 | 
			static_cast<uint8_t>(
					(unicode >> (6 * (width - 1))) & 0x3F);

		for (size_t extIter = 1; extIter < width; extIter++) {
			*reinterpret_cast<uint8_t*>(&utf[0]) |= 
				static_cast<uint8_t>(1) << (7 - extIter);

			*reinterpret_cast<uint8_t*>(&utf[extIter]) |= 
				0x80 | 
				static_cast<uint8_t>(
						(unicode >> (6 * (width - 1 - extIter))) & 0x3F);
		}
	}
	return utf;
}

std::basic_string<uint32_t> StringUnicodeFromUtf8(
		const std::string &utf, 
		bool &failBit)
{
	std::basic_string<uint32_t> unicode;

	const char *utfString = &utf[0];
	size_t lengthLeft = utf.size();

	while (lengthLeft) {
		unicode.append(1, UnicodeFromUtf8(&utfString, &lengthLeft, &failBit));
	}

	return unicode;
}


/* ATD::Unicode constants */

std::string ATD::Unicode::StrFromGlyph(const ATD::Unicode::Glyph &glyph)
{
	const uint32_t *uniString = &glyph;
	size_t lengthLeft = 1;
	bool failBitDummy = false;

	return Utf8FromUnicode(&uniString, &lengthLeft, &failBitDummy);
}

const ATD::Unicode::Glyph ATD::Unicode::NEW_LINE = 
	ATD::Unicode("\n").Front();

const ATD::Unicode::Glyph ATD::Unicode::SPACE = 
	ATD::Unicode(" ").Front();


/* ATD::Unicode */

ATD::Unicode::Unicode(const std::string &str)
	: m_fail(false)
	, m_glyphs(StringUnicodeFromUtf8(str, m_fail))
{}

ATD::Unicode::Unicode(const ATD::Unicode &other)
	: m_fail(other.m_fail)
	, m_glyphs(other.m_glyphs)
{}

void ATD::Unicode::CheckFailure() const
{
	if (m_fail) {
		throw std::runtime_error(
				Printf(
					"Errors while creating Unicode from bad UTF-8"));
	}
}

ATD::Unicode::Glyph &ATD::Unicode::operator[](size_t index)
{ return m_glyphs[index]; }

const ATD::Unicode::Glyph &ATD::Unicode::operator[](size_t index) const
{ return m_glyphs[index]; }

ATD::Unicode::Glyph &ATD::Unicode::Front()
{ return m_glyphs.front(); }

const ATD::Unicode::Glyph &ATD::Unicode::Front() const
{ return m_glyphs.front(); }

size_t ATD::Unicode::Size() const
{ return m_glyphs.size(); }

bool ATD::Unicode::operator==(const Unicode &other) const
{ return (m_glyphs == other.m_glyphs); }

bool ATD::Unicode::operator!=(const Unicode &other) const
{ return (m_glyphs != other.m_glyphs); }

bool ATD::Unicode::operator<(const Unicode &other) const
{ return m_glyphs < other.m_glyphs; }

bool ATD::Unicode::operator>(const Unicode &other) const
{ return m_glyphs > other.m_glyphs; }

ATD::Unicode &ATD::Unicode::operator+=(const ATD::Unicode &other)
{
	m_fail = m_fail || other.m_fail;
	m_glyphs += other.m_glyphs;
	return *this;
}

ATD::Unicode ATD::Unicode::operator+(const ATD::Unicode &other) const
{
	Unicode unicode;
	unicode += other;
	return unicode;
}

ATD::Unicode &ATD::Unicode::operator=(const ATD::Unicode &other)
{
	m_fail = other.m_fail;
	m_glyphs = other.m_glyphs;
	return *this;
}

std::string ATD::Unicode::Str() const
{
	std::string utf = "";

	const uint32_t *uniString = &m_glyphs[0];
	size_t lengthLeft = m_glyphs.size();
	bool failBitDummy = false;

	while (lengthLeft) {
		utf += Utf8FromUnicode(&uniString, &lengthLeft, &failBitDummy);
	}

	return utf;
}

std::string ATD::Unicode::Str(size_t index) const
{
	if (index < m_glyphs.size()) {
		const uint32_t *uniString = &m_glyphs[index];
		size_t lengthLeft = m_glyphs.size() - index;
		bool failBitDummy = false;

		return Utf8FromUnicode(&uniString, &lengthLeft, &failBitDummy);
	}

	/* TODO: throw */
	return std::string();
}

ATD::Unicode ATD::Unicode::Copy(size_t first, size_t length) const
{
	return Unicode(m_fail, m_glyphs.substr(first, length));
}

ATD::Unicode &ATD::Unicode::Erase(size_t first, size_t length)
{
	m_glyphs.erase(first, length);
	return *this;
}

// TODO: Separate class TextMonospaced, with geometry features

/*
ATD::Unicode ATD::Unicode::LimitedLength(size_t length) const
{
	if (length == 0) { return Unicode(""); }

	size_t curLineLen = 0;
	Glyphs glyphs;

	for (auto &glyph : m_glyphs) {
		if (glyph == NEW_LINE) {
			glyphs.push_back(glyph);
			curLineLen = 0;
			continue;
		}
		if (curLineLen == length - 1) {
			glyphs.push_back(glyph);
			glyphs.push_back(NEW_LINE);
			curLineLen = 0;
			continue;
		}
		glyphs.push_back(glyph);
		curLineLen++;
	}

	return Unicode(glyphs);
}
*/

/*
ATD::Vector2S ATD::Unicode::CaretGeometry(size_t caretPos) const
{
	Vector2S geom(0, 0);
	for (size_t iter = 0, gNum = 0; 
			iter < caretPos && gNum < m_glyphs.size();
			) {
		if (m_glyphs[gNum] == NEW_LINE) {
			geom.x = 0;
			geom.y++;
			gNum++;
		} else {
			geom.x++;
			gNum++;
			iter++;
		}
	}
	return geom;
}
*/

ATD::Unicode::Unicode(bool fail, const std::basic_string<Glyph> &glyphs)
	: m_fail(fail)
	, m_glyphs(glyphs)
{}


