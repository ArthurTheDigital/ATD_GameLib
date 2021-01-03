/**
 * @file      
 * @brief     Sprite for Ansi::Image.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Transform2D.hpp>
#include <ATD/Core/Vector2.hpp>
#include <ATD/Ansi/Graphics.hpp>


namespace ATD {

namespace Ansi {

/**
 * @brief ...
 * @class ... */
class Sprite : public Image::Drawable
{
public:
	/**
	 * @brief ...
	 * @param texture  - ...
	 * @param bounds   - ...
	 * @param center   - ...
	 * @param position - ...
	 * @param angle    - angle as a fraction of circle
	 * @param scale    - ... */
	Sprite(Image::CPtr &texture, 
			const RectL &bounds, 
			const Vector2D &center = Vector2D(), 
			const Vector2D &position = Vector2D(), 
			double angleFrc = 0., 
			double scale = 1., 
			bool repeat = false);

	virtual void DrawSelf(Image &target) const override;

private:
	Image::CPtr m_texture;
	RectL m_bounds;
	Vector2D m_center;
	Vector2D m_position;
	Transform2D m_transform;
	bool m_repeat;
};

} /* namespace Con */

} /* namespace ATD */


