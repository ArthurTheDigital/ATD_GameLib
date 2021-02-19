

#pragma once

#include <ATD/Core/Debug.hpp>
#include <ATD/Graphics/FrameBuffer.hpp>
#include <ATD/Graphics/Shader.hpp>
#include <ATD/Graphics/Texture.hpp>
#include <ATD/Graphics/VertexBuffer3D.hpp>


class Model : public ATD::FrameBuffer::Drawable
{
public:
	Model(const ATD::Image &texture);

	void drawSelf(ATD::FrameBuffer &target) const;

private:
	/* ATD::Shader3D::Ptr m_shaderPtr; // DEBUG */
	ATD::Texture::Ptr m_texturePtr;
	ATD::VertexBuffer3D::Ptr m_verticesPtr;
};


/* Model auxiliary: */

static const std::vector<ATD::Vector3D> _VERTICES_POS = {
	ATD::Vector3D(-0.3, -0.3,  0.3 + 5.0), 
	ATD::Vector3D( 0.3, -0.3,  0.3 + 5.0), 
	ATD::Vector3D( 0.3,  0.3,  0.3 + 5.0), 
	ATD::Vector3D(-0.3,  0.3,  0.3 + 5.0), 

	ATD::Vector3D(-0.3, -0.3, -0.3 + 5.0), 
	ATD::Vector3D( 0.3, -0.3, -0.3 + 5.0), 
	ATD::Vector3D( 0.3,  0.3, -0.3 + 5.0), 
	ATD::Vector3D(-0.3,  0.3, -0.3 + 5.0)
};
static const std::vector<ATD::Vector2L> _VERTICES_TEX = {
	ATD::Vector2L( 32,   0), 
	ATD::Vector2L( 64,   0), 
	ATD::Vector2L(  0,  32), 
	ATD::Vector2L( 32,  32), 
	ATD::Vector2L( 64,  32), 
	ATD::Vector2L( 96,  32), 
	ATD::Vector2L(  0,  64), 
	ATD::Vector2L( 32,  64), 
	ATD::Vector2L( 64,  64), 
	ATD::Vector2L( 96,  64), 
	ATD::Vector2L( 32,  96), 
	ATD::Vector2L( 64,  96), 
	ATD::Vector2L( 32, 128), 
	ATD::Vector2L( 64, 128)
};

/*
static const std::string _SHADER_VERTEX_SOURCE = 
"#version 130\n"
"\n"
"attribute vec3 atrPosition;\n"
"attribute vec2 atrTexCoord;\n"
"attribute vec4 atrColor;\n"
"\n"
"varying vec2 varTexCoord;\n"
"varying vec4 varColor;\n"
"\n"
"uniform mat4 unfTransform;\n"
"uniform mat4 unfProject;\n"
"\n"
"\n"
"void main()\n"
"{\n"
"    vec4 position = unfTransform * vec4(atrPosition, 1.f);\n"
"\n"
"    varTexCoord = atrTexCoord;\n"
"    varColor = atrColor;\n"
"    gl_Position = unfProject * position;\n"
"}\n";

static const std::string _SHADER_FRAGMENT_SOURCE = 
"#version 130\n"
"\n"
"varying vec2 varTexCoord;\n"
"varying vec4 varColor;\n"
"\n"
"uniform sampler2D unfTexture0;\n"
"\n"
"\n"
"void main()\n"
"{\n"
"    vec4 txColor = texture2D(unfTexture0, varTexCoord);\n"
"    gl_FragColor = txColor * varColor;\n"
"}\n"; // DEBUG */

/* Since Y coord is flipped, front-facing triangles must be 
 * drawn counter-clockwise. */
static const std::vector<ATD::Vertex3D> _VERTICES = {
	ATD::Vertex3D(_VERTICES_POS[1], _VERTICES_TEX[ 4]), 
	ATD::Vertex3D(_VERTICES_POS[0], _VERTICES_TEX[ 3]), 
	ATD::Vertex3D(_VERTICES_POS[2], _VERTICES_TEX[ 8]), 
	ATD::Vertex3D(_VERTICES_POS[3], _VERTICES_TEX[ 7]), 
	ATD::Vertex3D(_VERTICES_POS[2], _VERTICES_TEX[ 8]), 
	ATD::Vertex3D(_VERTICES_POS[0], _VERTICES_TEX[ 3]), 

	ATD::Vertex3D(_VERTICES_POS[5], _VERTICES_TEX[ 5]), 
	ATD::Vertex3D(_VERTICES_POS[1], _VERTICES_TEX[ 4]), 
	ATD::Vertex3D(_VERTICES_POS[6], _VERTICES_TEX[ 9]), 
	ATD::Vertex3D(_VERTICES_POS[2], _VERTICES_TEX[ 8]), 
	ATD::Vertex3D(_VERTICES_POS[6], _VERTICES_TEX[ 9]), 
	ATD::Vertex3D(_VERTICES_POS[1], _VERTICES_TEX[ 4]), 

	ATD::Vertex3D(_VERTICES_POS[4], _VERTICES_TEX[12]), 
	ATD::Vertex3D(_VERTICES_POS[5], _VERTICES_TEX[13]), 
	ATD::Vertex3D(_VERTICES_POS[7], _VERTICES_TEX[10]), 
	ATD::Vertex3D(_VERTICES_POS[6], _VERTICES_TEX[11]), 
	ATD::Vertex3D(_VERTICES_POS[7], _VERTICES_TEX[10]), 
	ATD::Vertex3D(_VERTICES_POS[5], _VERTICES_TEX[13]), 

	ATD::Vertex3D(_VERTICES_POS[0], _VERTICES_TEX[ 3]), 
	ATD::Vertex3D(_VERTICES_POS[4], _VERTICES_TEX[ 2]), 
	ATD::Vertex3D(_VERTICES_POS[3], _VERTICES_TEX[ 7]), 
	ATD::Vertex3D(_VERTICES_POS[7], _VERTICES_TEX[ 6]), 
	ATD::Vertex3D(_VERTICES_POS[3], _VERTICES_TEX[ 7]), 
	ATD::Vertex3D(_VERTICES_POS[4], _VERTICES_TEX[ 2]), 

	ATD::Vertex3D(_VERTICES_POS[2], _VERTICES_TEX[ 8]), 
	ATD::Vertex3D(_VERTICES_POS[3], _VERTICES_TEX[ 7]), 
	ATD::Vertex3D(_VERTICES_POS[6], _VERTICES_TEX[11]), 
	ATD::Vertex3D(_VERTICES_POS[7], _VERTICES_TEX[10]), 
	ATD::Vertex3D(_VERTICES_POS[6], _VERTICES_TEX[11]), 
	ATD::Vertex3D(_VERTICES_POS[3], _VERTICES_TEX[ 7]), 

	ATD::Vertex3D(_VERTICES_POS[4], _VERTICES_TEX[ 0]), 
	ATD::Vertex3D(_VERTICES_POS[0], _VERTICES_TEX[ 3]), 
	ATD::Vertex3D(_VERTICES_POS[5], _VERTICES_TEX[ 1]), 
	ATD::Vertex3D(_VERTICES_POS[1], _VERTICES_TEX[ 4]), 
	ATD::Vertex3D(_VERTICES_POS[5], _VERTICES_TEX[ 1]), 
	ATD::Vertex3D(_VERTICES_POS[0], _VERTICES_TEX[ 3])
};


/* Model: */

Model::Model(const ATD::Image &texture)
	: ATD::FrameBuffer::Drawable()
/*	, m_shaderPtr(new ATD::Shader3D(_SHADER_VERTEX_SOURCE, 
				_SHADER_FRAGMENT_SOURCE)) */
	, m_texturePtr(nullptr)
	, m_verticesPtr(nullptr)
{
	/* IPRINTF("", "Texture size: (%lu x %lu)", texture.size().x, 
			texture.size().y); // DEBUG */

	m_texturePtr = ATD::Texture::Ptr(new ATD::Texture(texture));

	/* Setup vertices: */
	m_verticesPtr = ATD::VertexBuffer3D::Ptr(
			new ATD::VertexBuffer3D(_VERTICES, texture.size()));

	/* Enable triangle culling: */
	ATD::gl.frontFace(ATD::Gl::CW);
	ATD::gl.cullFace(ATD::Gl::BACK);
	ATD::gl.enable(ATD::Gl::CULL_FACE);

	/* IPRINTF("", "sizeof(ATD::Vertex3D::GlVertex) == %lu", 
			sizeof(ATD::Vertex3D::GlVertex)); // DEBUG */
	/* IPRINTF("", "VertexBuffer3D size: %lu", 
			m_verticesPtr->Size()); // DEBUG */
	/* IPRINTF("", "vertex shader source:\n%s", 
			_SHADER_VERTEX_SOURCE.c_str()); // DEBUG */
	/* IPRINTF("", "fragment shader source:\n%s", 
			_SHADER_FRAGMENT_SOURCE.c_str()); // DEBUG */

	/* const ATD::VertexBuffer3D::AttrIndices &attrIndices = 
			m_shaderPtr->getAttrIndices();
	IPRINTF("", "AttrIndices:\n"
			"position %u\ntexCoords %u %s\n"
			"normal %u %s\ncolor %u %s", 
			attrIndices.positionIndex, 

			attrIndices.texCoordsIndex, 
			attrIndices.texCoordsAreRequired ? "+" : "-", 

			attrIndices.normalIndex, 
			attrIndices.normalIsRequired ? "+" : "-", 

			attrIndices.colorIndex, 
			attrIndices.colorIsRequired ? "+" : "-"); // DEBUG */
}

void Model::drawSelf(ATD::FrameBuffer &target) const
{
	ATD::Texture::Usage useTexture(*m_texturePtr);
	target.draw(*m_verticesPtr, 
			ATD::Transform3D()); // TODO: Debug projection! */

/*	m_shaderPtr->setUniform("unfTransform", ATD::Matrix4F());
	m_shaderPtr->setUniform("unfProject", ATD::Matrix4F());

	ATD::FrameBuffer::Usage useFBuffer(target);
	ATD::Shader::Usage useShader(*m_shaderPtr);

	m_verticesPtr->drawSelfInternal(
			m_shaderPtr->getAttrIndices()); // DEBUG */
}


