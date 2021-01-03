

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

class PxText : public FrameBuffer::Drawable
{
public:
	typedef std::shared_ptr<PxText> Ptr;
	typedef std::shared_ptr<const PxText> CPtr;


	// TODO: Pick a default pxFont and change the args order
	PxText(PxFont::CPtr pxFontPtr, const Unicode &unicode = Unicode());

	void SetOffset(const Vector2L &offset);

	virtual void DrawSelf(FrameBuffer &target) const override;

private:


	Unicode m_unicode;
	PxFont::CPtr m_pxFontPtr;

	VertexBuffer2D m_vertices;
	Transform2D m_transform;
};

} /* namespace ATD */


