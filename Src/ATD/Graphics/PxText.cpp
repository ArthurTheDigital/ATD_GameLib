/**
 * @file      
 * @brief     PxText (drawable pixel text) class.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Graphics/PxText.hpp>

#include <ATD/Core/MinMax.hpp>


/* PxText auxiliary: */

static std::vector<ATD::Vertex2D> _verticesFromGlyph(
		const ATD::PxFont::Glyph &glyph, 
		ATD::Vector2L &joint)
{
	ATD::Vector2L size(glyph.textureRect().w, glyph.textureRect().h);
	ATD::Vector2L position = joint - glyph.leftJoint();
	joint = position + glyph.rightJoint();

	std::vector<ATD::Vector2L> positions = 
		ATD::RectL(position, size).vertices();

	std::vector<ATD::Vector2L> texCoords = 
		glyph.textureRect().vertices();

	const std::vector<ATD::Vertex2D> vtxSamples = {
		ATD::Vertex2D(static_cast<ATD::Vector2D>(positions[0]), texCoords[0]), 
		ATD::Vertex2D(static_cast<ATD::Vector2D>(positions[1]), texCoords[1]), 
		ATD::Vertex2D(static_cast<ATD::Vector2D>(positions[2]), texCoords[2]), 
		ATD::Vertex2D(static_cast<ATD::Vector2D>(positions[3]), texCoords[3])
	};

	const std::vector<ATD::Vertex2D> vertices = {
		vtxSamples[3], 
		vtxSamples[0], 
		vtxSamples[2], 

		vtxSamples[1], 
		vtxSamples[2], 
		vtxSamples[0]
	};

	return vertices;
}

static std::vector<ATD::Vertex2D> _verticesFromUnicode(
		const ATD::Unicode &unicode, 
		const ATD::PxFont &pxFont)
{
	std::vector<ATD::Vertex2D> vertices;

	/* Obtain all the vertices, assuming joint (0, 0). */
	ATD::Vector2L topLeft;
	ATD::Vector2L joint;
	for (size_t uIndex = 0; uIndex < unicode.size(); uIndex++) {
		std::vector<ATD::Vertex2D> verticesNew = 
			_verticesFromGlyph(pxFont.getGlyph(unicode[uIndex]), joint);

		for (auto &vertex : verticesNew) {
			topLeft.x = 
				ATD::min<long>(
						static_cast<ATD::Vector2L>(vertex.position).x, 
						topLeft.x);

			topLeft.y = 
				ATD::min<long>(
						static_cast<ATD::Vector2L>(vertex.position).y, 
						topLeft.y);
		}

		vertices.insert(vertices.end(), 
				verticesNew.begin(), 
				verticesNew.end());
	}

	/* Offset all the vertices, so the top left corner will become (0, 0) */
	for (auto &vertex : vertices) {
		vertex.position -= topLeft;
	}

	return vertices;
}


/* ATD::PxText: */

ATD::PxText::PxText(ATD::PxFont::CPtr pxFontPtr, const ATD::Unicode &unicode)
	: FrameBuffer::Drawable2D()
	, m_unicode(unicode)
	, m_pxFontPtr(pxFontPtr)
	, m_vertices(_verticesFromUnicode(m_unicode, *m_pxFontPtr), 
			m_pxFontPtr->texturePtr()->size())

{}

void ATD::PxText::drawSelf(ATD::FrameBuffer &target) const
{
	Texture::Usage useTexture(*m_pxFontPtr->texturePtr());
	target.draw(m_vertices, m_transform);
}


