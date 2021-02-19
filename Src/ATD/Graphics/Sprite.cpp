/**
 * @file      
 * @brief     A basic sprite.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Graphics/Sprite.hpp>


/* ATD::Sprite: */

ATD::Sprite::Sprite(ATD::Texture::CPtr texturePtr, 
		const ATD::RectL &textureBounds, 
		const ATD::Transform2D &transform)
	: FrameBuffer::Drawable2D()
	, m_texturePtr(texturePtr)
	, m_vertices(textureBounds, texturePtr->size())
{
	FrameBuffer::Drawable2D::setTransform(transform);
}

ATD::Sprite::Sprite(ATD::Texture::CPtr texturePtr, 
		const ATD::Transform2D &transform)
	: Sprite(texturePtr, RectL(texturePtr->size()), transform)
{}

void ATD::Sprite::drawSelf(ATD::FrameBuffer &target) const
{
	Texture::Usage useTexture(*m_texturePtr);
	target.draw(m_vertices, m_transform);
}


