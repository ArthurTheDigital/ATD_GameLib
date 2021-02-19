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
	gl.getIntegerv(Gl::ARRAY_BUFFER_BINDING, 
			reinterpret_cast<Gl::Int *>(&m_prevBuffer));

	if (m_prevBuffer != buffer.glId()) {
		gl.bindBuffer(Gl::ARRAY_BUFFER, buffer.glId());
		m_activated = true;
	}
}

ATD::VertexBuffer2D::Usage::~Usage()
{
	if (m_activated) { gl.bindBuffer(Gl::ARRAY_BUFFER, m_prevBuffer); }
}


/* ATD::VertexBuffer2D aux: */

static const std::vector<ATD::Vertex2D::GlVertex> _DFT_GL_VERTICES_VALS = {
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

static const std::vector<ATD::Vertex2D::GlVertex> _DFT_GL_VERTICES = {
	_DFT_GL_VERTICES_VALS[3], 
	_DFT_GL_VERTICES_VALS[0], 
	_DFT_GL_VERTICES_VALS[2], 

	_DFT_GL_VERTICES_VALS[1], 
	_DFT_GL_VERTICES_VALS[2], 
	_DFT_GL_VERTICES_VALS[0]
};

static std::vector<ATD::Vertex2D> _dftVerticesFromRect(
		const ATD::RectL &textureBounds, 
		const ATD::Pixel &color)
{
	const std::vector<ATD::Vertex2D> verticesVals = {
		ATD::Vertex2D(ATD::Vector2D(0., 0.), 
				textureBounds.pos(), 
				color), 
		ATD::Vertex2D(
				ATD::Vector2D(static_cast<double>(textureBounds.size().x), 0.), 
				textureBounds.pos() + ATD::Vector2L(textureBounds.size().x, 0), 
				color), 
		ATD::Vertex2D(static_cast<ATD::Vector2D>(textureBounds.size()), 
				textureBounds.pos() + textureBounds.size(), 
				color), 
		ATD::Vertex2D(
				ATD::Vector2D(0., static_cast<double>(textureBounds.size().y)), 
				textureBounds.pos() + ATD::Vector2L(0, textureBounds.size().y), 
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
	: VertexBuffer2D(_dftVerticesFromRect(textureBounds, color), textureSize)
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
		Vertex2D::GlVertex glVertex = vertex.glVertex(textureSize);
		glVertices.push_back(glVertex);

		/* verticesStr += Aux::printf(" {pos: %f %f, tex: %f %f}", 
				glVertex.position.x, glVertex.position.y, 
				glVertex.texCoords.x, glVertex.texCoords.y); // DEBUG */
	}

	/* IPRINTF("", "GL vertices:%s", verticesStr.c_str()); // DEBUG */

	gl.genBuffers(1, &m_bufferId);
	Usage use(*this);
	gl.bufferData(Gl::ARRAY_BUFFER, sizeof(Vertex2D::GlVertex) * m_size,
			glVertices.data(), Gl::STATIC_DRAW);
}

ATD::VertexBuffer2D::VertexBuffer2D(
		const std::vector<ATD::Vertex2D::GlVertex> &glVertices, 
		const ATD::VertexBuffer2D::Primitive &primitive)
	: m_bufferId(0)
	, m_size(glVertices.size())
	, m_primitive(primitive)
{
	gl.genBuffers(1, &m_bufferId);
	Usage use(*this);
	gl.bufferData(Gl::ARRAY_BUFFER, sizeof(Vertex2D::GlVertex) * m_size,
			glVertices.data(), Gl::STATIC_DRAW);
}

ATD::VertexBuffer2D::~VertexBuffer2D()
{
	gl.deleteBuffers(1, &m_bufferId);
}

void ATD::VertexBuffer2D::drawSelfInternal(
		const ATD::VertexBuffer2D::AttrIndices &attrIndices) const
{
	gl.enableVertexAttribArray(attrIndices.positionIndex);
	if (attrIndices.texCoordsAreRequired) {
		gl.enableVertexAttribArray(attrIndices.texCoordsIndex);
	}
	if (attrIndices.colorIsRequired) {
		gl.enableVertexAttribArray(attrIndices.colorIndex);
	}

	{
		Usage useVBuffer(*this);

		/* position is set unconditionally */
		gl.vertexAttribPointer(attrIndices.positionIndex, 
				static_cast<Gl::Int>(sizeof(Vector2F) / sizeof(float)), 
				Gl::FLOAT, 
				Gl::FALSE, 
				static_cast<Gl::Sizei>(sizeof(Vertex2D::GlVertex)), 
				reinterpret_cast<const void *>(0));
	
		/* texCoords are set optionally */
		if (attrIndices.texCoordsAreRequired) {
			gl.vertexAttribPointer(attrIndices.texCoordsIndex, 
					static_cast<Gl::Int>(sizeof(Vector2F) / sizeof(float)), 
					Gl::FLOAT, 
					Gl::FALSE, 
					static_cast<Gl::Sizei>(sizeof(Vertex2D::GlVertex)), 
					reinterpret_cast<const void *>(sizeof(Vector2F)));
		}
	
		/* color is set optionally */
		if (attrIndices.colorIsRequired) {
			gl.vertexAttribPointer(attrIndices.colorIndex, 
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

		gl.drawArrays(primitive, 0, static_cast<Gl::Sizei>(m_size));
	}

	if (attrIndices.colorIsRequired) {
		gl.disableVertexAttribArray(attrIndices.colorIndex);
	}
	if (attrIndices.texCoordsAreRequired) {
		gl.disableVertexAttribArray(attrIndices.texCoordsIndex);
	}
	gl.disableVertexAttribArray(attrIndices.positionIndex);
}


