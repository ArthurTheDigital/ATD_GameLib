/**
 * @file      
 * @brief     Ansi blinking glyph.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Ansi/Graphics.hpp>


namespace ATD {

namespace Ansi {

/**
 * @brief Some terminals do not animate blinking text, so this id a rough work around.
 * @class ... */
class Blinker : public Image::Drawable
{
public:
	/**
	 * @brief ...
	 * @param ticks    - ...
	 * @param position - ... */
	Blinker(size_t ticks, const Vector2L &position = Vector2L());

	/**
	 * @brief ...
	 * @param other - ... */
	Blinker(const Blinker &other);

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	Blinker &operator=(const Blinker &other);

	/**
	 * @brief ...
	 * @param position - ... */
	void setPosition(const Vector2L &position);

	/**
	 * @brief ... */
	void reset() const;

	/**
	 * @brief ...
	 * @param ticksDelta - ... */
	void update(size_t ticksDelta = 1) const;

	/**
	 * @brief ...
	 * @param target - ... */
	void drawSelf(Image &target) const;

private:
	size_t m_ticks;
	Vector2L m_position;

	mutable size_t m_ticksCount;
};

} /* namespace Ansi */

} /* namespace ATD */


