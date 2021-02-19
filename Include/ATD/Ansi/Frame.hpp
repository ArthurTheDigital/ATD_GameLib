/**
 * @file      
 * @brief     Ansi frame.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Ansi/Graphics.hpp>
#include <ATD/Core/Rectangle.hpp>
#include <ATD/Core/Unicode.hpp>


namespace ATD {

namespace Ansi {

/**
 * @brief ...
 * @class ... */
class Frame : public Image::Drawable
{
public:
	/**
	 * @brief ...
	 * @param glyphs    - ...
	 * @param forecolor - ...
	 * @param backcolor - ...
	 * @param rectangle - ... */
	Frame(const std::string &glyphs, 
			unsigned char forecolor = Glyph::FORECOLOR_DEFAULT, 
			unsigned char backcolor = Glyph::BACKCOLOR_DEFAULT, 
			const RectL &rectangle = RectL());

	/**
	 * @brief ...
	 * @param other - ... */
	Frame(const Frame &other);

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	Frame &operator=(const Frame &other);

	/**
	 * @brief ...
	 * @param forecolor - ... */
	void setForecolor(unsigned char forecolor);

	/**
	 * @brief ...
	 * @param backcolor - ... */
	void setBackcolor(unsigned char backcolor);

	/**
	 * @brief Set the rectangle to be surrounded by frame
	 * @param rectangle - the rectangle to be framed */
	void setRectangle(const RectL &rectangle);

	/**
	 * @brief ...
	 * @param target - ... */
	virtual void drawSelf(Image &target) const override;

private:
	Unicode m_unicode;
	unsigned char m_forecolor;
	unsigned char m_backcolor;
	RectL m_rectangle;
};


extern Frame solidFrame;
extern Frame dashedFrame;
extern Frame doubleFrame;

} /* namespace Ansi */

} /* namespace ATD */


