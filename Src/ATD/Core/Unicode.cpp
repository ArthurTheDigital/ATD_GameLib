/**
 * @file      
 * @brief     Unicode processing.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Core/Unicode.hpp>

#include <ATD/Core/Debug.hpp>
#include <ATD/Core/Printf.hpp>

#include <string.h>


/* ATD::Unicode auxiliary */

const std::vector<uint8_t> _UTF8_HEAD_MASKS = {
	0x00 << 7, 
	0x06 << 5, 
	0x0e << 4, 
	0x1e << 3
};

static size_t _widthFromUtf8(char utfHead)
{
	uint8_t utfHeadBits = *reinterpret_cast<uint8_t *>(&utfHead);

	if (utfHeadBits >> 7 == 0x00) { return 1; }
	if (utfHeadBits >> 5 == 0x06) { return 2; }
	if (utfHeadBits >> 4 == 0x0e) { return 3; }
	if (utfHeadBits >> 3 == 0x1e) { return 4; }

	return static_cast<size_t>(-1);
}

static size_t _widthFromUnicode(uint32_t u)
{
	if (u < 0x00000080) { return 1; }
	if (u < 0x00000800) { return 2; }
	if (u < 0x00010000) { return 3; }
	if (u < 0x00110000) { return 4; }

	return static_cast<size_t>(-1);
}

static uint32_t _unicodeFromUtf8(const std::string &utfString, 
		size_t &utfIndex, 
		bool &failBit)
{
	size_t width = _widthFromUtf8(utfString[utfIndex]);
	if (width == static_cast<size_t>(-1) || utfIndex + width > utfString.size()) {
		failBit = true;

		/* IPRINTF("", "DBG: Bad UTF"); // DEBUG */

		/* Bad UTF8 character. Not critical, put zero instead. */
		utfIndex += 1;
		return 0;
	} else {
		/* Save the current index. */
		size_t utfIndex0 = utfIndex;

		/* Update the given one. */
		utfIndex += width;

		/* Do UTF8 -> Unicode conversion. */
		const uint8_t utfBits0 = 
			*reinterpret_cast<const uint8_t *>(&utfString[utfIndex0]);

		/* E.g. 110xxxxx -> 000xxxxx */
		uint32_t unicode = 
			static_cast<uint32_t>(
				utfBits0 & (~_UTF8_HEAD_MASKS[width - 1]));

		for (size_t extIter = 1; extIter < width; extIter++) {
			const uint8_t utfBitsExt = 
				*reinterpret_cast<const uint8_t *>(&utfString[utfIndex0 + extIter]);

			/* E.g. 10xxxxxx -> 00xxxxxx */
			unicode = 
				(unicode << 6) | 
				static_cast<uint32_t>(utfBitsExt & ~(0x80));
		}
		return unicode;
	}
}

/* *uniStringPtr is to be a valid pointer, 
 * *lengthLeftPtr is to be > 0. */
std::string _utf8FromUnicode(const std::basic_string<uint32_t> &uniString, 
		size_t &uniIndex, 
		bool &failBit)
{
	/* Save the current unicode value. */
	uint32_t unicode = uniString[uniIndex];

	/* Update the unicode string index. */
	uniIndex += 1;

	/* Calculate width. */
	size_t width = _widthFromUnicode(unicode);

	if (width == static_cast<size_t>(-1)) {
		failBit = true;

		/* IPRINTF("", "DBG: bad unicode!"); // DEBUG */

		return std::string(1, '\0');
	}

	/* Do Unicode -> UTF8 conversion. */
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

std::basic_string<uint32_t> _stringUnicodeFromUtf8(const std::string &utf, 
		bool &failBit)
{
	std::basic_string<uint32_t> unicode;

	size_t utfIndex = 0;
	while (utfIndex < utf.size()) {
		unicode.append(1, _unicodeFromUtf8(utf, utfIndex, failBit));
	}

	return unicode;
}


/* ATD::Unicode constants: */

const ATD::Unicode::Glyph ATD::Unicode::NEW_LINE = 
	ATD::Unicode("\n").front();

const ATD::Unicode::Glyph ATD::Unicode::SPACE = 
	ATD::Unicode(" ").front();


/* ATD::Unicode static functions: */

std::string ATD::Unicode::strFromGlyph(const ATD::Unicode::Glyph &glyph)
{
	std::basic_string<uint32_t> uniString(1, glyph);
	size_t uniIndex = 0;
	bool failBitDummy = false;

	return _utf8FromUnicode(uniString, uniIndex, failBitDummy);
}


/* ATD::Unicode */

ATD::Unicode::Unicode(const std::string &str)
	: m_fail(false)
	, m_glyphs(_stringUnicodeFromUtf8(str, m_fail))
{}

ATD::Unicode::Unicode(const ATD::Unicode &other)
	: m_fail(other.m_fail)
	, m_glyphs(other.m_glyphs)
{}

void ATD::Unicode::checkFailure() const
{
	if (m_fail) {
		throw std::runtime_error(
				Aux::printf(
					"Errors while creating Unicode from bad UTF-8"));
	}
}

ATD::Unicode::Glyph &ATD::Unicode::operator[](size_t index)
{ return m_glyphs[index]; }

const ATD::Unicode::Glyph &ATD::Unicode::operator[](size_t index) const
{ return m_glyphs[index]; }

ATD::Unicode::Glyph &ATD::Unicode::front()
{ return m_glyphs.front(); }

const ATD::Unicode::Glyph &ATD::Unicode::front() const
{ return m_glyphs.front(); }

size_t ATD::Unicode::size() const
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

std::string ATD::Unicode::str() const
{
	std::string utf = "";

	size_t uniIndex = 0;
	bool failBitDummy = false;

	while (uniIndex < m_glyphs.size()) {
		utf += _utf8FromUnicode(m_glyphs, uniIndex, failBitDummy);
	}

	return utf;
}

std::string ATD::Unicode::str(size_t index) const
{
	if (index < m_glyphs.size()) {
		size_t uniIndex = index;
		bool failBitDummy = false;

		return _utf8FromUnicode(m_glyphs, uniIndex, failBitDummy);
	}

	/* TODO: throw */
	return std::string();
}

ATD::Unicode ATD::Unicode::copy(size_t first, size_t length) const
{
	return Unicode(m_fail, m_glyphs.substr(first, length));
}

ATD::Unicode &ATD::Unicode::erase(size_t first, size_t length)
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


