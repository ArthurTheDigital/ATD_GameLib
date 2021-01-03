

#include <ATD/Graphics/PxText.hpp>

#include <ATD/Core/MinMax.hpp>


/* PxText auxiliary: */

static std::vector<ATD::Vertex2D> VerticesFromGlyph(
		const ATD::PxFont::Glyph &glyph, 
		ATD::Vector2L &joint)
{
	ATD::Vector2L size(glyph.GetTextureRect().w, glyph.GetTextureRect().h);
	ATD::Vector2L position = joint - glyph.GetJointLeft();
	joint = position + glyph.GetJointRight();

	std::vector<ATD::Vector2L> positions = 
		ATD::RectL(position, size).Vertices();

	std::vector<ATD::Vector2L> texCoords = 
		glyph.GetTextureRect().Vertices();

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

static std::vector<ATD::Vertex2D> VerticesFromUnicode(
		const ATD::Unicode &unicode, 
		const ATD::PxFont &pxFont)
{
	std::vector<ATD::Vertex2D> vertices;

	/* Obtain all the vertices, assuming joint (0, 0). */
	ATD::Vector2L topLeft;
	ATD::Vector2L joint;
	for (size_t uIndex = 0; uIndex < unicode.Size(); uIndex++) {
		std::vector<ATD::Vertex2D> verticesNew = 
			VerticesFromGlyph(pxFont.GetGlyph(unicode[uIndex]), joint);

		for (auto &vertex : verticesNew) {
			topLeft.x = 
				ATD::Min<long>(
						static_cast<ATD::Vector2L>(vertex.position).x, 
						topLeft.x);

			topLeft.y = 
				ATD::Min<long>(
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
	: FrameBuffer::Drawable()
	, m_unicode(unicode)
	, m_pxFontPtr(pxFontPtr)
	, m_vertices(
			VerticesFromUnicode(m_unicode, *m_pxFontPtr), 
			m_pxFontPtr->GetTexturePtr()->Size())

	, m_transform()
{}

void ATD::PxText::SetOffset(const ATD::Vector2L &offset)
{
	m_transform.SetOffset(static_cast<Vector2D>(offset));
}

void ATD::PxText::DrawSelf(ATD::FrameBuffer &target) const
{
	Texture::Usage useTexture(*m_pxFontPtr->GetTexturePtr());
	target.Draw(m_vertices, m_transform);
}


