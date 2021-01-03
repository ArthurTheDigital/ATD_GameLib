/**
 * @file     
 * @brief    Terminal ansi graphics implementation.
 * @details  License: GPL v3.
 * @author   ArthurTheDigital (arthurthedigital@gmail.com)
 * @since    $Id: $ */

#include <ATD/Ansi/Graphics.hpp>

#include <ATD/Core/MinMax.hpp>
#include <ATD/Core/ImageImpl.hpp>
#include <ATD/Core/Printf.hpp>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <stdexcept>


/* ATD::Ansi::Glyph constants */

const unsigned char ATD::Ansi::Glyph::FOREGROUND_DEFAULT = 0x07;
const unsigned char ATD::Ansi::Glyph::BACKGROUND_DEFAULT = 0xE8;
const ATD::Unicode::Glyph ATD::Ansi::Glyph::UTF8_GLYPH_DEFAULT = 
	ATD::Unicode(" ").Front();

const std::string ATD::Ansi::Glyph::STR_RESET = "\033[00m";


const ATD::Ansi::Glyph::ModeTable ATD::Ansi::Glyph::MODE_TABLE = {
	/* flag, setter, remover */
	ATD::Ansi::Glyph::ModeTableEntry({0x0001, 1, 22}), 
	ATD::Ansi::Glyph::ModeTableEntry({0x0002, 2, 22}), 
	ATD::Ansi::Glyph::ModeTableEntry({0x0004, 3, 23}), 
	ATD::Ansi::Glyph::ModeTableEntry({0x0008, 4, 24}), 
	ATD::Ansi::Glyph::ModeTableEntry({0x0010, 7, 27}), 
	ATD::Ansi::Glyph::ModeTableEntry({0x0020, 8, 28}), 
	ATD::Ansi::Glyph::ModeTableEntry({0x0040, 9, 29})
};

const uint16_t ATD::Ansi::Glyph::M_INTENSE = 
	ATD::Ansi::Glyph::MODE_TABLE[0][0];

const uint16_t ATD::Ansi::Glyph::M_DIM = 
	ATD::Ansi::Glyph::MODE_TABLE[1][0];

const uint16_t ATD::Ansi::Glyph::M_ITALIC = 
	ATD::Ansi::Glyph::MODE_TABLE[2][0];

const uint16_t ATD::Ansi::Glyph::M_UNDERLINED = 
	ATD::Ansi::Glyph::MODE_TABLE[3][0];

const uint16_t ATD::Ansi::Glyph::M_REVERSED = 
	ATD::Ansi::Glyph::MODE_TABLE[4][0];

const uint16_t ATD::Ansi::Glyph::M_HIDDEN = 
	ATD::Ansi::Glyph::MODE_TABLE[5][0];

const uint16_t ATD::Ansi::Glyph::M_STRIKED = 
	ATD::Ansi::Glyph::MODE_TABLE[6][0];


/* ATD::Ansi::Glyph */

ATD::Ansi::Glyph::Glyph()
	: Glyph(UTF8_GLYPH_DEFAULT)
{}

ATD::Ansi::Glyph::Glyph(const Unicode::Glyph &unicode, 
		unsigned char foreground, 
		unsigned char background, 
		uint16_t mode)
	: m_unicode(unicode)
	, m_foreground(foreground)
	, m_background(background)
	, m_mode(mode)
{}

bool ATD::Ansi::Glyph::operator==(const ATD::Ansi::Glyph &other) const
{
	return (
			m_unicode == other.m_unicode && 
			m_foreground == other.m_foreground && 
			m_background == other.m_background && 
			m_mode == other.m_mode
			);
}

bool ATD::Ansi::Glyph::operator!=(const ATD::Ansi::Glyph &other) const
{
	return !(operator==(other));
}

std::string ATD::Ansi::Glyph::Str() const
{
	std::string str = Printf(
			"\033[38;5;%u;48;5;%u", 
			m_foreground, 
			m_background
			);
	for (size_t iM = 0; iM < MODE_NUM; iM++) {
		if (m_mode & MODE_TABLE[iM][0]) {
			str += Printf(";%u", static_cast<unsigned>(MODE_TABLE[iM][1]));
		}
	}
	str += "m";
	str += Unicode::StrFromGlyph(m_unicode);
	str += STR_RESET;

	return str;
}

std::string ATD::Ansi::Glyph::StrOptimized(const ATD::Ansi::Glyph *prev) const
{
	if (!prev) {
		std::string str = Printf(
				"\033[38;5;%u;48;5;%u", 
				m_foreground, 
				m_background
				);
		for (size_t iM = 0; iM < MODE_NUM; iM++) {
			if (m_mode & MODE_TABLE[iM][0]) {
				str += Printf(";%u", 
						static_cast<unsigned>(MODE_TABLE[iM][1]));
			}
		}
		str += "m";
		str += Unicode::StrFromGlyph(m_unicode);

		return str;
	} else {
		std::vector<std::string> prefixes;

		/* Mode removers first */
		for (size_t iM = 0; iM < MODE_NUM; iM++) {
			if ((prev->m_mode & MODE_TABLE[iM][0]) && 
					!(m_mode & MODE_TABLE[iM][0])) {
				prefixes.push_back(
						Printf("%u", 
							static_cast<unsigned>(MODE_TABLE[iM][2])));
			}
		}

		/* Set foreground */
		if (m_foreground != prev->m_foreground) {
			prefixes.push_back(Printf("38;5;%u", m_foreground));
		}

		/* Set background */
		if (m_background != prev->m_background) {
			prefixes.push_back(Printf("48;5;%u", m_background));
		}

		/* Mode setters */
		for (size_t iM = 0; iM < MODE_NUM; iM++) {
			if (!(prev->m_mode & MODE_TABLE[iM][0]) && 
					(m_mode & MODE_TABLE[iM][0])) {
				prefixes.push_back(
						Printf("%u", 
							static_cast<unsigned>(MODE_TABLE[iM][1])));
			}
		}

		/* Create string */
		std::string str = prefixes.size() ? 
			std::string("\033[") + prefixes[0] : 
			std::string();

		for (size_t iPre = 1; iPre < prefixes.size(); iPre++) {
			str += std::string(";") + prefixes[iPre];
		}
		str += prefixes.size() ? "m" : "";
		str += Unicode::StrFromGlyph(m_unicode);

		return str;
	}
}

/* Stupid setters/getters */

ATD::Unicode::Glyph ATD::Ansi::Glyph::GetUnicode() const
{ return m_unicode; }

void ATD::Ansi::Glyph::SetUnicode(const ATD::Unicode::Glyph &unicode)
{ m_unicode = unicode; }

unsigned char ATD::Ansi::Glyph::Foreground() const
{ return m_foreground; }

void ATD::Ansi::Glyph::Foreground(const unsigned char foreground)
{ m_foreground = foreground; }

unsigned char ATD::Ansi::Glyph::Background() const
{ return m_background; }

void ATD::Ansi::Glyph::Background(const unsigned char background)
{ m_background = background; }

uint16_t ATD::Ansi::Glyph::Mode() const
{ return m_mode; }

void ATD::Ansi::Glyph::Mode(uint16_t mode)
{ m_mode = mode; }

/* === */


/* ATD::Ansi::GlyphMask */

ATD::Ansi::GlyphMask::GlyphMask(const ATD::Ansi::Glyph &glyph, 
		uint16_t bitMask)
	: m_glyph(glyph)
	, m_bitMask(bitMask)
{}

ATD::Ansi::GlyphMask::GlyphMask()
	: GlyphMask(Glyph(), 0x0000)
{}

bool ATD::Ansi::GlyphMask::Check(const ATD::Ansi::Glyph &glyph) const
{
	bool resultAnd = static_cast<bool>(m_bitMask & 
			(BIT_UTF8 | BIT_FG | BIT_BG | BIT_MODE));

	bool resultOr = false;

	if (m_bitMask & BIT_UTF8) {
		bool matchUnicode = (m_glyph.GetUnicode() == glyph.GetUnicode());

		resultAnd = resultAnd && matchUnicode;
		resultOr = resultOr || matchUnicode;
	}

	if (m_bitMask & BIT_FG) {
		bool matchForeground = (m_glyph.Foreground() == glyph.Foreground());

		resultAnd = resultAnd && matchForeground;
		resultOr = resultOr || matchForeground;
	}

	if (m_bitMask & BIT_BG) {
		bool matchBackground = (m_glyph.Background() == glyph.Background());

		resultAnd = resultAnd && matchBackground;
		resultOr = resultOr || matchBackground;
	}

	if (m_bitMask & BIT_MODE) {
		bool matchMode = (m_bitMask & BIT_M_ANY) ? 
			static_cast<bool>(m_glyph.Mode() & glyph.Mode()) : 
			(m_glyph.Mode() == glyph.Mode());

		resultAnd = resultAnd && matchMode;
		resultOr = resultOr || matchMode;
	}

	return (m_bitMask & BIT_ANY) ? resultOr : resultAnd;
}

void ATD::Ansi::GlyphMask::Apply(ATD::Ansi::Glyph &glyph) const
{
	if (m_bitMask & BIT_UTF8) { glyph.SetUnicode(m_glyph.GetUnicode()); }
	if (m_bitMask & BIT_FG) { glyph.Foreground(m_glyph.Foreground()); }
	if (m_bitMask & BIT_BG) { glyph.Background(m_glyph.Background()); }
	if (m_bitMask & BIT_MODE) { glyph.Mode(m_glyph.Mode()); }
}


/* ATD::Ansi::Image::Drawable */

ATD::Ansi::Image::Drawable::Drawable()
{}

ATD::Ansi::Image::Drawable::~Drawable()
{}


/* ATD::Ansi::Image auxiliary */

static void ParseAnsi(const std::string &ansi, 
		unsigned char &fg, 
		unsigned char &bg, 
		uint16_t &mode, 
		const unsigned char &initFg, 
		const unsigned char &initBg, 
		const uint16_t &initMode)
{
	if (ansi.size()) {
		std::vector<unsigned char> ansiOptions;
		bool wasDigit = false;
		for (size_t iC = 0; iC < ansi.size(); iC++) {
			if (ansi[iC] >= '0' && ansi[iC] <= '9') {
				if (!wasDigit) { ansiOptions.push_back(0); }
				ansiOptions.back() *= 10;
				ansiOptions.back() += static_cast<int>(ansi[iC] - '0');
				wasDigit = true;
			} else {
				wasDigit = false;
			}
		}

		for (size_t iO = 0; iO < ansiOptions.size(); iO++) {
			if (ansiOptions[iO] == 38 && iO + 1 < ansiOptions.size()) {
				if (ansiOptions[iO + 1] == 5 && iO + 2 < ansiOptions.size()) {
					fg = ansiOptions[iO + 2];
					iO += 2;
				}
				/* FIXME: Parse r;g;b? */
			} else if (ansiOptions[iO] == 48 && iO + 1 < ansiOptions.size()) {
				if (ansiOptions[iO + 1] == 5 && iO + 2 < ansiOptions.size()) {
					bg = ansiOptions[iO + 2];
					iO += 2;
				}
				/* FIXME: Parse r;g;b? */
			} else if (ansiOptions[iO] == 0) {
				fg = initFg;
				bg = initBg;
				mode = initMode;
			} else {
				bool checked = false;
				for (size_t iM = 0; 
						iM < ATD::Ansi::Glyph::MODE_NUM && !checked; 
						iM++) {
					if (ATD::Ansi::Glyph::MODE_TABLE[iM][1] == 
							ansiOptions[iO]) {
						mode |= ATD::Ansi::Glyph::MODE_TABLE[iM][0];
						checked = true;
					}
				}

				for (size_t iM = 0; 
						iM < ATD::Ansi::Glyph::MODE_NUM && !checked; 
						iM++) {
					if (ATD::Ansi::Glyph::MODE_TABLE[iM][2] == 
							ansiOptions[iO]) {
						mode &= ~(ATD::Ansi::Glyph::MODE_TABLE[iM][0]);
						checked = true;
					}
				}
			}
		}
	}
}


/* ATD::Ansi::Image */

ATD::Ansi::Image::Image()
	: Image(Vector2S(0, 0))
{}

ATD::Ansi::Image::Image(const ATD::Vector2S &size, 
		const ATD::Ansi::Glyph &glyph)
	: m_size(size)
	, m_glyphs(m_size.x * m_size.y ? new Glyph[m_size.x * m_size.y] : nullptr)
{
	for (size_t i = 0; i < m_size.x * m_size.y; i++) { m_glyphs[i] = glyph; }
}

ATD::Ansi::Image::Image(const ATD::Ansi::Image &other)
	: m_size(other.m_size)
	, m_glyphs(m_size.x * m_size.y ? new Glyph[m_size.x * m_size.y] : nullptr)
{
	for (size_t i = 0; i < m_size.x * m_size.y; i++) {
		m_glyphs[i] = other.m_glyphs[i];
	}
}

ATD::Ansi::Image::Image(const std::string &chars, 
		unsigned char initFg, 
		unsigned char initBg, 
		uint16_t initMode)
	: Image()
{
	std::vector<std::vector<Glyph>> lines;
	lines.push_back(std::vector<Glyph>());
	size_t maxLength = 0;

	Unicode unicode(chars);
	unsigned char foreground = initFg;
	unsigned char background = initBg;
	uint16_t mode = initMode;

	for (size_t uIndex = 0; uIndex < unicode.Size(); uIndex++) {
		if (unicode[uIndex] == Unicode("\n").Front()) {
			/* New line */
			lines.push_back(std::vector<Glyph>());
		} else if (unicode[uIndex] == Unicode("\033").Front()) {
			/* ANSI sequence */
			std::string ansi;
			size_t uIndex2 = uIndex + 1;

			/* Get ANSI string. We search only for style modifying strings, 
			 * which match expression '[*m' */
			if (uIndex2 != unicode.Size()) {
				if (unicode[uIndex2] != Unicode("[").Front()) { continue; }
			}
			for (; uIndex2 != unicode.Size(); uIndex2++) {
				ansi += unicode.Str(uIndex2);
				if (ansi.back() == 'm') { break; }
			}

			if (uIndex2 != unicode.Size()) {
				uIndex = uIndex2;

				ParseAnsi(ansi, 
						foreground, background, mode, 
						initFg, initBg, initMode);
			}
		} else {
			/* Actual glyph */
			/* FIXME: Cut unprintable characters. */
			lines.back().push_back(
					Glyph(
						unicode[uIndex], 
						foreground, 
						background, 
						mode
						)
					);
			maxLength = Max<size_t>(maxLength, lines.back().size());
		}
	}

	m_size = Vector2S(maxLength, lines.size());
	m_glyphs = new Glyph[m_size.x * m_size.y];

	for (size_t iY = 0; iY < m_size.y; iY++) {
		for (size_t iX = 0; iX < m_size.x; iX++) {
			m_glyphs[iY * m_size.x + iX] = (
					iX < lines[iY].size() ? 
					lines[iY][iX] : 
					(
						(iY * m_size.x + iX) ?
						Glyph(
							Glyph::UTF8_GLYPH_DEFAULT, 
							m_glyphs[iY * m_size.x + iX - 1].Foreground(), 
							m_glyphs[iY * m_size.x + iX - 1].Background(), 
							m_glyphs[iY * m_size.x + iX - 1].Mode()
							) :
						Glyph(
							Glyph::UTF8_GLYPH_DEFAULT, 
							initFg, 
							initBg, 
							initMode
							)
						)
					);
		}
	}
}

ATD::Ansi::Image::~Image()
{
	delete [] m_glyphs;
}

ATD::Ansi::Glyph ATD::Ansi::Image::GetGlyph(const Vector2L &position, 
		bool repeat) const
{
	if (repeat) {
		if ((m_size.x * m_size.y)) {
			Vector2L p(position);

			/* Both non-zero, division acceptable */
			p.x %= m_size.x;
			p.x += p.x < 0 ? m_size.x : 0;

			p.y %= m_size.y;
			p.y += p.y < 0 ? m_size.y : 0;

			return m_glyphs[p.y * m_size.x + p.x];
		} else {
			return Glyph();
		}
	} else {
		return (RectL(m_size).Contains(position) ? 
				m_glyphs[position.y * m_size.x + position.x] : 
				Glyph());
	}
}

std::string ATD::Ansi::Image::Str(bool plain) const
{
	std::string result;
	if (plain) {
		for (size_t iY = 0; iY < m_size.y; iY++) {
			for (size_t iX = 0; iX < m_size.x; iX++) {
				result += Unicode::StrFromGlyph(
						m_glyphs[iY * m_size.x + iX].GetUnicode());
			}
			result += (iY + 1 < m_size.y ? "\n" : "");
		}
	} else {
		for (size_t iY = 0; iY < m_size.y; iY++) {
			const Glyph *cachedPtr = nullptr;
			for (size_t iX = 0; iX < m_size.x; iX++) {
				const Glyph *currPtr = &(m_glyphs[iY * m_size.x + iX]);
				result += currPtr->StrOptimized(cachedPtr);
				cachedPtr = currPtr;
			}
			result += Glyph::STR_RESET + (iY + 1 < m_size.y ? "\n" : "");
		}
	}
	return result;
}

std::string ATD::Ansi::Image::Str(const ATD::RectL &bounds, 
		bool repeat, 
		bool plain) const
{
	std::string result;
	if (repeat) {
		RectL b(bounds);

		b.x %= m_size.x;
		b.x += b.x < 0 ? m_size.x : 0;

		b.y %= m_size.y;
		b.y += b.y < 0 ? m_size.y : 0;

		if (plain) {
			for (long iY = 0; iY < b.h; iY++) {
				for (long iX = 0; iX < b.w; iX++) {
					const long index = (iY % m_size.y) * m_size.x + 
						(iX % m_size.x);

					result += Unicode::StrFromGlyph(
							m_glyphs[index].GetUnicode());
				}
				result += (iY + 1 < b.h ? "\n" : "");
			}
		} else {
			for (long iY = 0; iY < b.h; iY++) {
				const Glyph *cachedPtr = nullptr;
				for (long iX = 0; iX < b.w; iX++) {
					const long index = (iY % m_size.y) * m_size.x + 
						(iX % m_size.x);

					const Glyph *currPtr = &(m_glyphs[index]);
					result += currPtr->StrOptimized(cachedPtr);
					cachedPtr = currPtr;
				}
				result += Glyph::STR_RESET + (iY + 1 < b.h ? "\n" : "");
			}
		}
	} else {
		RectL b(bounds.Clamped(RectL(m_size)));

		if (plain) {
			for (long iY = 0; iY < b.h; iY++) {
				for (long iX = 0; iX < b.w; iX++) {
					result += Unicode::StrFromGlyph(
							m_glyphs[iY * m_size.x + iX].GetUnicode());
				}
				result += (iY + 1 < b.h ? "\n" : "");
			}
		} else {
			for (long iY = 0; iY < b.h; iY++) {
				const Glyph *cachedPtr = nullptr;
				for (long iX = 0; iX < b.w; iX++) {
					const Glyph *currPtr = &(m_glyphs[iY * m_size.x + iX]);
					result += currPtr->StrOptimized(cachedPtr);
					cachedPtr = currPtr;
				}
				result += Glyph::STR_RESET + (iY + 1 < b.h ? "\n" : "");
			}
		}
	}
	return result;
}

ATD::Vector2S ATD::Ansi::Image::Size() const
{
	return m_size;
}

const ATD::Ansi::Glyph *ATD::Ansi::Image::Data() const
{
	return m_glyphs;
}

ATD::Ansi::Glyph *ATD::Ansi::Image::DataModifyable()
{
	return m_glyphs;
}

bool ATD::Ansi::Image::operator==(const ATD::Ansi::Image &other) const
{
	if (m_size != other.m_size) { return false; }
	for (size_t iG = 0; iG < m_size.x * m_size.y; iG++) {
		if (m_glyphs[iG] != other.m_glyphs[iG]) { return false; }
	}
	return true;
}

bool ATD::Ansi::Image::operator!=(const ATD::Ansi::Image &other) const
{
	return !(operator==(other));
}

void ATD::Ansi::Image::Clear(const ATD::Ansi::Glyph &glyph)
{
	for (size_t iG = 0; iG < m_size.x * m_size.y; iG++) {
		m_glyphs[iG] = glyph;
	}
}

void ATD::Ansi::Image::SetBackground(unsigned char background)
{
	for (size_t iG = 0; iG < m_size.x * m_size.y; iG++) {
		m_glyphs[iG].Background(background);
	}
}

void ATD::Ansi::Image::SetForeground(unsigned char foreground)
{
	for (size_t iG = 0; iG < m_size.x * m_size.y; iG++) {
		m_glyphs[iG].Foreground(foreground);
	}
}

void ATD::Ansi::Image::SetMode(uint16_t mode)
{
	for (size_t iG = 0; iG < m_size.x * m_size.y; iG++) {
		m_glyphs[iG].Mode(mode);
	}
}

void ATD::Ansi::Image::Draw(const ATD::Vector2L &position, 
		const ATD::Ansi::Glyph &glyph)
{
	ImageImpl::DrawCell<Glyph>(
			m_size, m_glyphs, 
			position, 
			glyph
			);
}

void ATD::Ansi::Image::Draw(const ATD::Vector2L &position, 
		const ATD::Ansi::Image &image, 
		ATD::Ansi::Image::MixerFunc mixer)
{
	ImageImpl::DrawImage<Glyph>(
			m_size, m_glyphs, 
			position, 
			image.m_size, image.m_glyphs, 
			mixer ? mixer : [this](const Glyph &dst, const Glyph &src) {
				return this->MixOpacity(dst, src);
			}
			);
}

void ATD::Ansi::Image::Draw(const ATD::Vector2L &position, 
		const ATD::Ansi::Image &image, 
		const ATD::RectL &bounds, 
		bool repeat, 
		ATD::Ansi::Image::MixerFunc mixer)
{
	ImageImpl::DrawBounded<Glyph>(
			m_size, m_glyphs, 
			position, 
			image.m_size, image.m_glyphs, 
			bounds, 
			repeat, 
			mixer ? mixer : [this](const Glyph &dst, const Glyph &src) {
				return this->MixOpacity(dst, src);
			}
			);
}

void ATD::Ansi::Image::Draw(const ATD::Ansi::Image::Drawable &drawable)
{
	drawable.DrawSelf(*this);
}

void ATD::Ansi::Image::Filter(const ATD::Ansi::GlyphMask &checkMask, 
		const ATD::Ansi::GlyphMask &applyMask)
{
	for (size_t iG = 0; iG < m_size.x * m_size.y; iG++) {
		if (checkMask.Check(m_glyphs[iG])) {
			applyMask.Apply(m_glyphs[iG]);
		}
	}
}

ATD::Ansi::Glyph ATD::Ansi::Image::MixOpacity(const ATD::Ansi::Glyph &dst, 
		const ATD::Ansi::Glyph &src) const
{
	return src.GetUnicode() == Glyph::UTF8_GLYPH_DEFAULT ? dst : src;
}


/* ATD::Ansi::Screen auxiliary */

static ATD::Vector2S GetTerminalSize()
{
	struct winsize sz;
	int ioctlResult;

	/* FIXME: Make this function cross-platform */
	if ((ioctlResult = ::ioctl(::fileno(stdout), TIOCGWINSZ, &sz)) == -1) {
		int errnoVal = errno;
		throw std::runtime_error(
				ATD::Printf(
					"'ioctl(fileno(stdout), TIOCGWINSZ, ..)' failure: %d %s", 
					errnoVal, 
					::strerror(errnoVal)
					)
				);
	}

	return ATD::Vector2S(
			static_cast<size_t>(sz.ws_col), 
			static_cast<size_t>(sz.ws_row)
			);
}


/* ATD::Ansi::Screen */

ATD::Ansi::Screen::Screen(const ATD::Align &alignX, 
		const ATD::Align &alignY, 
		const ATD::Vector2L &offset)
	: Screen(GetTerminalSize(), alignX, alignY, offset)
{}

ATD::Ansi::Screen::Screen(const ATD::Vector2S &rtSize,  
		const ATD::Align &alignX, 
		const ATD::Align &alignY, 
		const ATD::Vector2L &offset)
	: Image(rtSize)
	, m_alignX(alignX)
	, m_alignY(alignY)
	, m_offset(offset)
	, m_linesDisplayed(0)
{}

ATD::Ansi::Screen::~Screen()
{
	if (m_linesDisplayed) {
		::fprintf(stdout, "\n");
	}
}

ATD::RectL ATD::Ansi::Screen::View() const
{
	Vector2S viewSize = GetTerminalSize();
	return RectL(
			m_offset, 
			static_cast<Vector2L>(Image::Size())
			).AlignClamped(
				RectL(viewSize), 
				m_alignX, 
				m_alignY
				);
}

/* Offset setters/getters */

ATD::Vector2L ATD::Ansi::Screen::Offset() const
{ return m_offset; }

void ATD::Ansi::Screen::Offset(const ATD::Vector2L &offset)
{ m_offset = offset; }

/* Align setters/getters */

ATD::Align ATD::Ansi::Screen::AlignX() const
{ return m_alignX; }

void ATD::Ansi::Screen::AlignX(const ATD::Align &alignX)
{ m_alignX = alignX; }

ATD::Align ATD::Ansi::Screen::AlignY() const
{ return m_alignY; }

void ATD::Ansi::Screen::AlignY(const ATD::Align &alignY)
{ m_alignY = alignY; }

/* === */

void ATD::Ansi::Screen::Display() const
{
	RectL view = View();
	std::string display = Image::Str(view);

	/* Delete previously written lines */
	if (m_linesDisplayed) {
		/* Move cursor left */
		::fprintf(stdout, "\r");

		/* Move cursor up */
		::fprintf(stdout, "\033[%luA", m_linesDisplayed);

		/* Erase everything down */
		::fprintf(stdout, "\033[J");
	}

	::fprintf(stdout, "%s", display.c_str());
	::fflush(stdout);

	m_linesDisplayed = view.h;
}

ATD::Ansi::Glyph ATD::Ansi::Screen::MixOpacity(const ATD::Ansi::Glyph &dst, 
		const ATD::Ansi::Glyph &src) const
{
	return (src.GetUnicode() == Glyph::UTF8_GLYPH_DEFAULT) ? 
		((dst.GetUnicode() == Glyph::UTF8_GLYPH_DEFAULT) ? src : dst) : src;
}


