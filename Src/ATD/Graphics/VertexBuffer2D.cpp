/**
 * @file      
 * @brief     Wrap around OpenGL vertex buffer (2D vertices).
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Graphics/VertexBuffer2D.hpp>

#include <ATD/Core/Debug.hpp>
#include <ATD/Core/Printf.hpp>


/* ATD::VertexBuffer2D::Usage: */

ATD::VertexBuffer2D::Usage::Usage(const ATD::VertexBuffer2D &buffer)
	: m_prevBuffer(0)
	, m_activated(false)
{
	gl._GetIntegerv(Gl::ARRAY_BUFFER_BINDING, 
			reinterpret_cast<Gl::Int *>(&m_prevBuffer));

	if (m_prevBuffer != buffer.GetGlId()) {
		gl._BindBuffer(Gl::ARRAY_BUFFER, buffer.GetGlId());
		m_activated = true;
	}
}

ATD::VertexBuffer2D::Usage::~Usage()
{
	if (m_activated) { gl._BindBuffer(Gl::ARRAY_BUFFER, m_prevBuffer); }
}


/* ATD::VertexBuffer2D aux: */

const std::vector<ATD::Vertex2D::GlVertex> _DFT_GL_VERTICES_VALS = {
	ATD::Vertex2D::GlVertex(ATD::Vector2F(-1.f,  1.f), 
			ATD::Vector2F(0.f, 0.f), 
			ATD::Vector4F(1.f, 1.f, 1.f, 1.f)), /* #0 */
	ATD::Vertex2D::GlVertex(ATD::Vector2F( 1.f,  1.f), 
			ATD::Vector2F(1.f, 0.f), 
			ATD::Vector4F(1.f, 1.f, 1.f, 1.f)), /* #1 */
	ATD::Vertex2D::GlVertex(ATD::Vector2F( 1.f, -1.f), 
			ATD::Vector2F(1.f, 1.f), 
			ATD::Vector4F(1.f, 1.f, 1.f, 1.f)), /* #2 */
	ATD::Vertex2D::GlVertex(ATD::Vector2F(-1.f, -1.f), 
			ATD::Vector2F(0.f, 1.f), 
			ATD::Vector4F(1.f, 1.f, 1.f, 1.f))  /* #3 */
};

const std::vector<ATD::Vertex2D::GlVertex> _DFT_GL_VERTICES = {
	_DFT_GL_VERTICES_VALS[3], 
	_DFT_GL_VERTICES_VALS[0], 
	_DFT_GL_VERTICES_VALS[2], 

	_DFT_GL_VERTICES_VALS[1], 
	_DFT_GL_VERTICES_VALS[2], 
	_DFT_GL_VERTICES_VALS[0]
};

std::vector<ATD::Vertex2D> DftVerticesFromRect(
		const ATD::RectL &textureBounds, 
		const ATD::Pixel &color)
{
	const std::vector<ATD::Vertex2D> verticesVals = {
		ATD::Vertex2D(ATD::Vector2D(0., 0.), 
				textureBounds.Pos(), 
				color), 
		ATD::Vertex2D(
				ATD::Vector2D(static_cast<double>(textureBounds.Size().x), 0.), 
				textureBounds.Pos() + ATD::Vector2L(textureBounds.Size().x, 0), 
				color), 
		ATD::Vertex2D(static_cast<ATD::Vector2D>(textureBounds.Size()), 
				textureBounds.Pos() + textureBounds.Size(), 
				color), 
		ATD::Vertex2D(
				ATD::Vector2D(0., static_cast<double>(textureBounds.Size().y)), 
				textureBounds.Pos() + ATD::Vector2L(0, textureBounds.Size().y), 
				color)
	};

	const std::vector<ATD::Vertex2D> vertices = {
		verticesVals[3], 
		verticesVals[0], 
		verticesVals[2], 

		verticesVals[1], 
		verticesVals[2], 
		verticesVals[0]
	};

	return vertices;
}


/* ATD::VertexBuffer2D: */

ATD::VertexBuffer2D::VertexBuffer2D()
	: VertexBuffer2D(_DFT_GL_VERTICES, TRIANGLES)
{}

ATD::VertexBuffer2D::VertexBuffer2D(const ATD::RectL &textureBounds, 
		const ATD::Vector2S &textureSize, 
		const ATD::Pixel &color)
	: VertexBuffer2D(DftVerticesFromRect(textureBounds, color), textureSize)
{}

ATD::VertexBuffer2D::VertexBuffer2D(
		const std::vector<ATD::Vertex2D> &vertices, 
		const ATD::Vector2S &textureSize, 
		const ATD::VertexBuffer2D::Primitive &primitive)
	: m_bufferId(0)
	, m_size(vertices.size())
	, m_primitive(primitive)
{
	/* std::string verticesStr = ""; // DEBUG */

	std::vector<Vertex2D::GlVertex> glVertices;
	for (auto &vertex : vertices) {
		Vertex2D::GlVertex glVertex = vertex.GetGlVertex(textureSize);
		glVertices.push_back(glVertex);

		/* verticesStr += Printf(" {pos: %f %f, tex: %f %f}", 
				glVertex.position.x, glVertex.position.y, 
				glVertex.texCoords.x, glVertex.texCoords.y); // DEBUG */
	}

	/* IPRINTF("", "GL vertices:%s", verticesStr.c_str()); // DEBUG */

	gl._GenBuffers(1, &m_bufferId);
	Usage use(*this);
	gl._BufferData(Gl::ARRAY_BUFFER, sizeof(Vertex2D::GlVertex) * m_size,
			glVertices.data(), Gl::STATIC_DRAW);
}

ATD::VertexBuffer2D::VertexBuffer2D(
		const std::vector<ATD::Vertex2D::GlVertex> &glVertices, 
		const ATD::VertexBuffer2D::Primitive &primitive)
	: m_bufferId(0)
	, m_size(glVertices.size())
	, m_primitive(primitive)
{
	gl._GenBuffers(1, &m_bufferId);
	Usage use(*this);
	gl._BufferData(Gl::ARRAY_BUFFER, sizeof(Vertex2D::GlVertex) * m_size,
			glVertices.data(), Gl::STATIC_DRAW);
}

ATD::VertexBuffer2D::~VertexBuffer2D()
{
	gl._DeleteBuffers(1, &m_bufferId);
}

ATD::Gl::Uint ATD::VertexBuffer2D::GetGlId() const
{ return m_bufferId; }

size_t ATD::VertexBuffer2D::Size() const
{ return m_size; }

ATD::VertexBuffer2D::Primitive ATD::VertexBuffer2D::GetPrimitive() const
{ return m_primitive; }

void ATD::VertexBuffer2D::DrawSelfInternal(
		const ATD::VertexBuffer2D::AttrIndices &attrIndices) const
{
	gl._EnableVertexAttribArray(attrIndices.positionIndex);
	if (attrIndices.texCoordsAreRequired) {
		gl._EnableVertexAttribArray(attrIndices.texCoordsIndex);
	}
	if (attrIndices.colorIsRequired) {
		gl._EnableVertexAttribArray(attrIndices.colorIndex);
	}

	{
		Usage useVBuffer(*this);

		/* position is set unconditionally */
		gl._VertexAttribPointer(attrIndices.positionIndex, 
				static_cast<Gl::Int>(sizeof(Vector2F) / sizeof(float)), 
				Gl::FLOAT, 
				Gl::FALSE, 
				static_cast<Gl::Sizei>(sizeof(Vertex2D::GlVertex)), 
				reinterpret_cast<const void *>(0));
	
		/* texCoords are set optionally */
		if (attrIndices.texCoordsAreRequired) {
			gl._VertexAttribPointer(attrIndices.texCoordsIndex, 
					static_cast<Gl::Int>(sizeof(Vector2F) / sizeof(float)), 
					Gl::FLOAT, 
					Gl::FALSE, 
					static_cast<Gl::Sizei>(sizeof(Vertex2D::GlVertex)), 
					reinterpret_cast<const void *>(sizeof(Vector2F)));
		}
	
		/* color is set optionally */
		if (attrIndices.colorIsRequired) {
			gl._VertexAttribPointer(attrIndices.colorIndex, 
					static_cast<Gl::Int>(sizeof(Vector4F) / sizeof(float)), 
					Gl::FLOAT, 
					Gl::FALSE, 
					static_cast<Gl::Sizei>(sizeof(Vertex2D::GlVertex)), 
					reinterpret_cast<const void *>(sizeof(Vector2F) + 
						sizeof(Vector2F)));
		}

		Gl::Enum primitive = 
			m_primitive == TRIANGLES ? Gl::TRIANGLES : 
			m_primitive == TRIANGLE_STRIP ? Gl::TRIANGLE_STRIP : 
			Gl::TRIANGLE_FAN;

		gl._DrawArrays(primitive, 0, static_cast<Gl::Sizei>(m_size));
	}

	if (attrIndices.colorIsRequired) {
		gl._DisableVertexAttribArray(attrIndices.colorIndex);
	}
	if (attrIndices.texCoordsAreRequired) {
		gl._DisableVertexAttribArray(attrIndices.texCoordsIndex);
	}
	gl._DisableVertexAttribArray(attrIndices.positionIndex);
}


