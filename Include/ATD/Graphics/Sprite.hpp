/**
 * @file      
 * @brief     A basic sprite.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Graphics/FrameBuffer.hpp>
#include <ATD/Graphics/Texture.hpp>
#include <ATD/Graphics/VertexBuffer2D.hpp>


namespace ATD {

/**
 * @brief ...
 * @class ... */
class Sprite : public FrameBuffer::Drawable2D
{
public:
	/**
	 * @brief ...
	 * @param texturePtr    - ...
	 * @param textureBounds - ...
	 * @param transform     - ... */
	Sprite(Texture::CPtr texturePtr, 
			const RectL &textureBounds, 
			const Transform2D &transform = Transform2D());

	/**
	 * @brief ...
	 * @param texturePtr - ...
	 * @param transform  - ... */
	Sprite(Texture::CPtr texturePtr, 
			const Transform2D &transform = Transform2D());

	/**
	 * @brief ...
	 * @param target - ... */
	virtual void drawSelf(FrameBuffer &target) const override;

protected:
	Texture::CPtr m_texturePtr;
	VertexBuffer2D m_vertices;
};

}


