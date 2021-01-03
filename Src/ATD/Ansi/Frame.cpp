

#include <ATD/Ansi/Frame.hpp>

#include <ATD/Core/Printf.hpp>

#include <stdexcept>


ATD::Ansi::Frame::Frame(const std::string &glyphs, 
		unsigned char foreColor, 
		unsigned char backColor, 
		const ATD::RectL &rectangle)
	: m_unicode(glyphs)
	, m_foreColor(foreColor)
	, m_backColor(backColor)
	, m_rectangle(rectangle)
{
	if (m_unicode.Size() != 9) {
		throw std::runtime_error(
				Printf(
					"Frame needs exactly 9 glyphs (%lu provided)", 
					m_unicode.Size()
					)
				);
	}
}

ATD::Ansi::Frame::Frame(const ATD::Ansi::Frame &other)
	: m_unicode(other.m_unicode)
	, m_foreColor(other.m_foreColor)
	, m_backColor(other.m_backColor)
	, m_rectangle(other.m_rectangle)
{}

ATD::Ansi::Frame &ATD::Ansi::Frame::operator=(const ATD::Ansi::Frame &other)
{
	m_unicode = other.m_unicode;
	m_foreColor = other.m_foreColor;
	m_backColor = other.m_backColor;
	m_rectangle = other.m_rectangle;
	return *this;
}

void ATD::Ansi::Frame::SetForeColor(unsigned char foreColor)
{ m_foreColor = foreColor; }

void ATD::Ansi::Frame::SetBackColor(unsigned char backColor)
{ m_backColor = backColor; }

void ATD::Ansi::Frame::SetRectangle(const ATD::RectL &rectangle)
{ m_rectangle = rectangle; }

void ATD::Ansi::Frame::DrawSelf(ATD::Ansi::Image &target) const
{
	/* Top-left */
	target.Draw(
			Vector2L(
				m_rectangle.x - 1, 
				m_rectangle.y - 1
				), 
			Glyph(
				m_unicode[0], 
				m_foreColor, 
				m_backColor
				)
			);

	/* Top-right */
	target.Draw(
			Vector2L(
				m_rectangle.x + m_rectangle.w, 
				m_rectangle.y - 1
				), 
			Glyph(
				m_unicode[2], 
				m_foreColor, 
				m_backColor
				)
			);

	/* Bottom-left */
	target.Draw(
			Vector2L(
				m_rectangle.x - 1, 
				m_rectangle.y + m_rectangle.h
				), 
			Glyph(
				m_unicode[6], 
				m_foreColor, 
				m_backColor
				)
			);

	/* Bottom-right */
	target.Draw(
			Vector2L(
				m_rectangle.x + m_rectangle.w, 
				m_rectangle.y + m_rectangle.h
				), 
			Glyph(
				m_unicode[8], 
				m_foreColor, 
				m_backColor
				)
			);

	for (long xIter = m_rectangle.Pos().x; 
			xIter < m_rectangle.Size().x; xIter++) {
		/* Top */
		target.Draw(
				Vector2L(
					xIter, 
					m_rectangle.y - 1
					), 
				Glyph(
					m_unicode[1], 
					m_foreColor, 
					m_backColor
					)
				);

		/* Bottom */
		target.Draw(
				Vector2L(
					xIter, 
					m_rectangle.y + m_rectangle.h
					), 
				Glyph(
					m_unicode[7], 
					m_foreColor, 
					m_backColor
					)
				);
	}

	for (long yIter = m_rectangle.Pos().y; 
			yIter < m_rectangle.Size().y; yIter++) {
		/* Left */
		target.Draw(
				Vector2L(
					m_rectangle.x - 1, 
					yIter
					), 
				Glyph(
					m_unicode[3], 
					m_foreColor, 
					m_backColor
					)
				);

		/* Right */
		target.Draw(
				Vector2L(
					m_rectangle.x + m_rectangle.w, 
					yIter
					), 
				Glyph(
					m_unicode[5], 
					m_foreColor, 
					m_backColor
					)
				);
	}
}



ATD::Ansi::Frame ATD::Ansi::solidFrame(
		"┏━┓"
		"┃ ┃"
		"┗━┛"
		);

ATD::Ansi::Frame ATD::Ansi::dashedFrame(
		"┏╍┓"
		"╏ ╏"
		"┗╍┛"
		);

ATD::Ansi::Frame ATD::Ansi::doubleFrame(
		"╔═╗"
		"║ ║"
		"╚═╝"
		);

