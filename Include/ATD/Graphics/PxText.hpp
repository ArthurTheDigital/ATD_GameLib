/**
 * @file      
 * @brief     PxText (drawable pixel text) class.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Transform2D.hpp>
#include <ATD/Core/Unicode.hpp>
#include <ATD/Graphics/FrameBuffer.hpp>
#include <ATD/Graphics/PxFont.hpp>
#include <ATD/Graphics/Shader.hpp>
#include <ATD/Graphics/VertexBuffer2D.hpp>

#include <memory>
#include <vector>


namespace ATD {

/* FIXME: PxText class is now crude. It requires redesign.
 * Shall handle multilines.
 * Shall support ordered glyphs.
 *
 * How to handle text being edited? Recreate it from scratch every time? */

/**
 * @brief ...
 * @class ... */
class PxText : public FrameBuffer::Drawable2D
{
public:
	typedef std::shared_ptr<PxText> Ptr;
	typedef std::shared_ptr<const PxText> CPtr;


	/* FIXME: Pick a default pxFont and change the args order */
	/**
	 * @brief ...
	 * @param pxFontPtr - ...
	 * @param unicode - ... */
	PxText(PxFont::CPtr pxFontPtr, const Unicode &unicode = Unicode());

	/**
	 * @brief ...
	 * @param target - ... */
	virtual void drawSelf(FrameBuffer &target) const override;

private:
	Unicode m_unicode;
	PxFont::CPtr m_pxFontPtr;

	VertexBuffer2D m_vertices;
};

} /* namespace ATD */


