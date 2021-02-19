/**
 * @file      
 * @brief     Ansi frame.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Ansi/Frame.hpp>

#include <ATD/Core/Printf.hpp>

#include <stdexcept>


/* ATD::Ansi::Frame: */

ATD::Ansi::Frame::Frame(const std::string &glyphs, 
		unsigned char forecolor, 
		unsigned char backcolor, 
		const ATD::RectL &rectangle)
	: m_unicode(glyphs)
	, m_forecolor(forecolor)
	, m_backcolor(backcolor)
	, m_rectangle(rectangle)
{
	if (m_unicode.size() != 9) {
		throw std::runtime_error(
				Aux::printf(
					"Frame needs exactly 9 glyphs (%lu provided)", 
					m_unicode.size()
					)
				);
	}
}

ATD::Ansi::Frame::Frame(const ATD::Ansi::Frame &other)
	: m_unicode(other.m_unicode)
	, m_forecolor(other.m_forecolor)
	, m_backcolor(other.m_backcolor)
	, m_rectangle(other.m_rectangle)
{}

ATD::Ansi::Frame &ATD::Ansi::Frame::operator=(const ATD::Ansi::Frame &other)
{
	m_unicode = other.m_unicode;
	m_forecolor = other.m_forecolor;
	m_backcolor = other.m_backcolor;
	m_rectangle = other.m_rectangle;
	return *this;
}

void ATD::Ansi::Frame::setForecolor(unsigned char forecolor)
{ m_forecolor = forecolor; }

void ATD::Ansi::Frame::setBackcolor(unsigned char backcolor)
{ m_backcolor = backcolor; }

void ATD::Ansi::Frame::setRectangle(const ATD::RectL &rectangle)
{ m_rectangle = rectangle; }

void ATD::Ansi::Frame::drawSelf(ATD::Ansi::Image &target) const
{
	/* Top-left */
	target.draw(
			Vector2L(
				m_rectangle.x - 1, 
				m_rectangle.y - 1
				), 
			Glyph(
				m_unicode[0], 
				m_forecolor, 
				m_backcolor
				)
			);

	/* Top-right */
	target.draw(
			Vector2L(
				m_rectangle.x + m_rectangle.w, 
				m_rectangle.y - 1
				), 
			Glyph(
				m_unicode[2], 
				m_forecolor, 
				m_backcolor
				)
			);

	/* Bottom-left */
	target.draw(
			Vector2L(
				m_rectangle.x - 1, 
				m_rectangle.y + m_rectangle.h
				), 
			Glyph(
				m_unicode[6], 
				m_forecolor, 
				m_backcolor
				)
			);

	/* Bottom-right */
	target.draw(
			Vector2L(
				m_rectangle.x + m_rectangle.w, 
				m_rectangle.y + m_rectangle.h
				), 
			Glyph(
				m_unicode[8], 
				m_forecolor, 
				m_backcolor
				)
			);

	for (long xIter = m_rectangle.pos().x; 
			xIter < m_rectangle.size().x; xIter++) {
		/* Top */
		target.draw(
				Vector2L(
					xIter, 
					m_rectangle.y - 1
					), 
				Glyph(
					m_unicode[1], 
					m_forecolor, 
					m_backcolor
					)
				);

		/* Bottom */
		target.draw(
				Vector2L(
					xIter, 
					m_rectangle.y + m_rectangle.h
					), 
				Glyph(
					m_unicode[7], 
					m_forecolor, 
					m_backcolor
					)
				);
	}

	for (long yIter = m_rectangle.pos().y; 
			yIter < m_rectangle.size().y; yIter++) {
		/* Left */
		target.draw(
				Vector2L(
					m_rectangle.x - 1, 
					yIter
					), 
				Glyph(
					m_unicode[3], 
					m_forecolor, 
					m_backcolor
					)
				);

		/* Right */
		target.draw(
				Vector2L(
					m_rectangle.x + m_rectangle.w, 
					yIter
					), 
				Glyph(
					m_unicode[5], 
					m_forecolor, 
					m_backcolor
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

