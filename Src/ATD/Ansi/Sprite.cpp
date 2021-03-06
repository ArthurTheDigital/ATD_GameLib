/**
 * @file      
 * @brief     Sprite for Ansi::Image.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Ansi/Sprite.hpp>

#include <ATD/Core/ImageImpl.hpp>


ATD::Ansi::Sprite::Sprite(ATD::Ansi::Image::CPtr &texture, 
		const ATD::RectL &bounds, 
		const ATD::Vector2D &center, 
		const ATD::Vector2D &position, 
		double angleFrc, 
		double scale, 
		bool repeat)
	: m_texture(texture)
	, m_bounds(bounds)
	, m_center(center)
	, m_position(position)
	, m_transform(ATD::Vector2D(scale, scale), angleFrc)
	, m_repeat(repeat)
{}

void ATD::Ansi::Sprite::drawSelf(ATD::Ansi::Image &target) const
{
	ImageImpl::drawTransformed<Glyph>(target.size(), target.data(), 
			m_position, 
			m_transform, 
			m_texture->size(), m_texture->data(), 
			m_bounds, 
			m_center, 
			m_repeat, 
			[target](const Glyph &dst, const Glyph &src) {
					return target.mixOpacity(dst, src);
				}, 
			0.5
			);
}


