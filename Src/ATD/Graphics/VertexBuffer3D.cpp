/**
 * @file      
 * @brief     Wrap around OpenGL vertex buffer (3D vertices).
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Graphics/VertexBuffer3D.hpp>

#include <ATD/Core/Debug.hpp>
#include <ATD/Core/Printf.hpp>
#include <ATD/Graphics/GlCheck.hpp>


/* ATD::VertexBuffer3D::Usage: */

ATD::VertexBuffer3D::Usage::Usage(const ATD::VertexBuffer3D &buffer)
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

ATD::VertexBuffer3D::Usage::~Usage()
{
	if (m_activated) { gl._BindBuffer(Gl::ARRAY_BUFFER, m_prevBuffer); }
}


/* ATD::VertexBuffer3D auxiliary: */

const std::vector<ATD::Vector3F> _DFT_POS = {
	ATD::Vector3F(-0.5f, -0.5f,  0.5f), 
	ATD::Vector3F( 0.5f, -0.5f,  0.5f), 
	ATD::Vector3F( 0.5f,  0.5f,  0.5f), 
	ATD::Vector3F(-0.5f,  0.5f,  0.5f), 

	ATD::Vector3F(-0.5f, -0.5f, -0.5f), 
	ATD::Vector3F( 0.5f, -0.5f, -0.5f), 
	ATD::Vector3F( 0.5f,  0.5f, -0.5f), 
	ATD::Vector3F(-0.5f,  0.5f, -0.5f)
};

const std::vector<ATD::Vector2F> _DFT_TEX = {
	ATD::Vector2F(0.f, 0.f), 
	ATD::Vector2F(1.f, 0.f), 
	ATD::Vector2F(0.f, 1.f), 
	ATD::Vector2F(1.f, 1.f)
};

const std::vector<ATD::Vector3F> _DFT_NRM = {
	ATD::Vector3F( 0.f,  0.f,  1.f), 
	ATD::Vector3F( 1.f,  0.f,  0.f), 
	ATD::Vector3F( 0.f,  0.f, -1.f), 
	ATD::Vector3F(-1.f,  0.f,  0.f), 
	ATD::Vector3F( 0.f,  1.f,  0.f), 
	ATD::Vector3F( 0.f, -1.f,  0.f), 
};

const ATD::Vector4F _DFT_CLR = ATD::Vector4F(1.f, 1.f, 1.f, 1.f);

const std::vector<ATD::Vertex3D::GlVertex> _DFT_GL_VERTICES = {
	ATD::Vertex3D::GlVertex(_DFT_POS[1], _DFT_TEX[1], _DFT_NRM[0], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[2], _DFT_TEX[3], _DFT_NRM[0], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[0], _DFT_TEX[0], _DFT_NRM[0], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[3], _DFT_TEX[2], _DFT_NRM[0], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[0], _DFT_TEX[0], _DFT_NRM[0], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[2], _DFT_TEX[3], _DFT_NRM[0], _DFT_CLR), 

	ATD::Vertex3D::GlVertex(_DFT_POS[5], _DFT_TEX[1], _DFT_NRM[1], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[6], _DFT_TEX[3], _DFT_NRM[1], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[1], _DFT_TEX[0], _DFT_NRM[1], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[2], _DFT_TEX[2], _DFT_NRM[1], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[1], _DFT_TEX[0], _DFT_NRM[1], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[6], _DFT_TEX[3], _DFT_NRM[1], _DFT_CLR), 

	ATD::Vertex3D::GlVertex(_DFT_POS[4], _DFT_TEX[1], _DFT_NRM[2], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[7], _DFT_TEX[3], _DFT_NRM[2], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[5], _DFT_TEX[0], _DFT_NRM[2], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[6], _DFT_TEX[2], _DFT_NRM[2], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[5], _DFT_TEX[0], _DFT_NRM[2], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[7], _DFT_TEX[3], _DFT_NRM[2], _DFT_CLR), 

	ATD::Vertex3D::GlVertex(_DFT_POS[0], _DFT_TEX[1], _DFT_NRM[3], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[3], _DFT_TEX[3], _DFT_NRM[3], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[4], _DFT_TEX[0], _DFT_NRM[3], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[7], _DFT_TEX[2], _DFT_NRM[3], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[4], _DFT_TEX[0], _DFT_NRM[3], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[3], _DFT_TEX[3], _DFT_NRM[3], _DFT_CLR), 

	ATD::Vertex3D::GlVertex(_DFT_POS[2], _DFT_TEX[1], _DFT_NRM[4], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[6], _DFT_TEX[3], _DFT_NRM[4], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[3], _DFT_TEX[0], _DFT_NRM[4], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[7], _DFT_TEX[2], _DFT_NRM[4], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[3], _DFT_TEX[0], _DFT_NRM[4], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[6], _DFT_TEX[3], _DFT_NRM[4], _DFT_CLR), 

	ATD::Vertex3D::GlVertex(_DFT_POS[4], _DFT_TEX[1], _DFT_NRM[5], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[5], _DFT_TEX[3], _DFT_NRM[5], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[0], _DFT_TEX[0], _DFT_NRM[5], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[1], _DFT_TEX[2], _DFT_NRM[5], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[0], _DFT_TEX[0], _DFT_NRM[5], _DFT_CLR), 
	ATD::Vertex3D::GlVertex(_DFT_POS[5], _DFT_TEX[3], _DFT_NRM[5], _DFT_CLR)
};


/* ATD::VertexBuffer3D: */

ATD::VertexBuffer3D::VertexBuffer3D()
	: VertexBuffer3D(_DFT_GL_VERTICES, TRIANGLES)
{}

ATD::VertexBuffer3D::VertexBuffer3D(
		const std::vector<ATD::Vertex3D> &vertices, 
		const ATD::Vector2S &textureSize, 
		const ATD::VertexBuffer3D::Primitive &primitive)
	: m_bufferId(0)
	, m_size(vertices.size())
	, m_primitive(primitive)
{
	/* std::string verticesStr = ""; // DEBUG */

	std::vector<Vertex3D::GlVertex> glVertices;
	for (auto &vertex : vertices) {
		Vertex3D::GlVertex glVertex = vertex.GetGlVertex(textureSize);
		glVertices.push_back(glVertex);

		/* verticesStr += Printf("\n{pos: %f %f %f, tex: %f %f, norm: %f %f %f, col: %f %f %f %f}", 
				glVertex.position.x, glVertex.position.y, glVertex.position.z, 
				glVertex.texCoords.x, glVertex.texCoords.y, 
				glVertex.normal.x, glVertex.normal.y, glVertex.normal.z, 
				glVertex.color.x, glVertex.color.y, glVertex.color.z, glVertex.color.w); // DEBUG */
	}

	/* IPRINTF("", "GL vertices:%s", verticesStr.c_str()); // DEBUG */

	gl._GenBuffers(1, &m_bufferId);
	Usage use(*this);
	gl._BufferData(Gl::ARRAY_BUFFER, sizeof(Vertex3D::GlVertex) * m_size, 
			glVertices.data(), Gl::STATIC_DRAW);
}

ATD::VertexBuffer3D::VertexBuffer3D(
		const std::vector<ATD::Vertex3D::GlVertex> &glVertices, 
		const ATD::VertexBuffer3D::Primitive &primitive)
	: m_bufferId(0)
	, m_size(glVertices.size())
	, m_primitive(primitive)
{
	gl._GenBuffers(1, &m_bufferId);
	Usage use(*this);
	gl._BufferData(Gl::ARRAY_BUFFER, sizeof(Vertex3D::GlVertex) * m_size, 
			glVertices.data(), Gl::STATIC_DRAW);
}

ATD::VertexBuffer3D::~VertexBuffer3D()
{
	gl._DeleteBuffers(1, &m_bufferId);
}

ATD::Gl::Uint ATD::VertexBuffer3D::GetGlId() const
{ return m_bufferId; }

size_t ATD::VertexBuffer3D::Size() const
{ return m_size; }

ATD::VertexBuffer3D::Primitive ATD::VertexBuffer3D::GetPrimitive() const
{ return m_primitive; }

void ATD::VertexBuffer3D::DrawSelfInternal(
		const ATD::VertexBuffer3D::AttrIndices &attrIndices) const
{
	/* Looks a bit ugly ... */
	gl._EnableVertexAttribArray(attrIndices.positionIndex);
	if (attrIndices.texCoordsAreRequired) {
		gl._EnableVertexAttribArray(attrIndices.texCoordsIndex);
	}
	if (attrIndices.normalIsRequired) {
		gl._EnableVertexAttribArray(attrIndices.normalIndex);
	}
	if (attrIndices.colorIsRequired) {
		gl._EnableVertexAttribArray(attrIndices.colorIndex);
	}

	{
		Usage useVBuffer(*this);

		/* position is set unconditionally */
		gl._VertexAttribPointer(attrIndices.positionIndex, 
				static_cast<Gl::Int>(sizeof(Vector3F) / sizeof(float)), 
				Gl::FLOAT, 
				Gl::FALSE, 
				static_cast<Gl::Sizei>(sizeof(Vertex3D::GlVertex)), 
				reinterpret_cast<const void *>(0));

		/* texCoords are set optionally */
		if (attrIndices.texCoordsAreRequired) {
			gl._VertexAttribPointer(attrIndices.texCoordsIndex, 
					static_cast<Gl::Int>(sizeof(Vector2F) / sizeof(float)), 
					Gl::FLOAT, 
					Gl::FALSE, 
					static_cast<Gl::Sizei>(sizeof(Vertex3D::GlVertex)), 
					reinterpret_cast<const void *>(sizeof(Vector3F)));
		}

		/* normal is set optionally */
		if (attrIndices.normalIsRequired) {
			gl._VertexAttribPointer(attrIndices.normalIndex, 
					static_cast<Gl::Int>(sizeof(Vector3F) / sizeof(float)), 
					Gl::FLOAT, 
					Gl::FALSE, 
					static_cast<Gl::Sizei>(sizeof(Vertex3D::GlVertex)), 
					reinterpret_cast<const void *>(sizeof(Vector3F) + 
						sizeof(Vector2F)));
		}

		/* color is set optionally */
		if (attrIndices.colorIsRequired) {
			gl._VertexAttribPointer(attrIndices.colorIndex, 
					static_cast<Gl::Int>(sizeof(Vector4F) / sizeof(float)), 
					Gl::FLOAT, 
					Gl::FALSE, 
					static_cast<Gl::Sizei>(sizeof(Vertex3D::GlVertex)), 
					reinterpret_cast<const void *>(sizeof(Vector3F) + 
						sizeof(Vector2F) + sizeof(Vector3F)));
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
	if (attrIndices.normalIsRequired) {
		gl._DisableVertexAttribArray(attrIndices.normalIndex);
	}
	if (attrIndices.texCoordsAreRequired) {
		gl._DisableVertexAttribArray(attrIndices.texCoordsIndex);
	}
	gl._DisableVertexAttribArray(attrIndices.positionIndex);
}


