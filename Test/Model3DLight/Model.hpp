

#pragma once

#include <ATD/Core/Debug.hpp>
#include <ATD/Core/Fs.hpp>
#include <ATD/Core/Transform3D.hpp>
#include <ATD/Graphics/FrameBuffer.hpp>
#include <ATD/Graphics/Shader.hpp>
#include <ATD/Graphics/Texture.hpp>
#include <ATD/Graphics/VertexBuffer3D.hpp>


class Model : public ATD::FrameBuffer::Drawable
{
public:
	Model(const ATD::Image &texture);

	void Rotate(const ATD::Vector2D &rotationXYFrc, 
			const ATD::Vector3D &axisX, 
			const ATD::Vector3D &axisY);

	void DrawSelf(ATD::FrameBuffer &target) const;

private:
	/* ATD::Shader3D::Ptr m_shaderPtr; // DEBUG */
	ATD::Texture::Ptr m_texturePtr;
	ATD::VertexBuffer3D::Ptr m_verticesPtr;
	ATD::Transform3D m_transform;
};


/* Model auxiliary: */

static const std::vector<ATD::Vector3D> _VERTICES_POS = {
	ATD::Vector3D( 0.000,  0.500, -0.657), 
	ATD::Vector3D( 0.476,  0.155, -0.657), 
	ATD::Vector3D( 0.294, -0.405, -0.657), 
	ATD::Vector3D(-0.294, -0.405, -0.657), 
	ATD::Vector3D(-0.476,  0.155, -0.657), 

	ATD::Vector3D( 0.000,  0.809, -0.157), 
	ATD::Vector3D( 0.769,  0.250, -0.157), 
	ATD::Vector3D( 0.476, -0.654, -0.157), 
	ATD::Vector3D(-0.476, -0.654, -0.157), 
	ATD::Vector3D(-0.769,  0.250, -0.157), 

	ATD::Vector3D( 0.000, -0.809,  0.157), 
	ATD::Vector3D( 0.769, -0.250,  0.157), 
	ATD::Vector3D( 0.476,  0.654,  0.157), 
	ATD::Vector3D(-0.476,  0.654,  0.157), 
	ATD::Vector3D(-0.769, -0.250,  0.157), 

	ATD::Vector3D( 0.000, -0.500,  0.657), 
	ATD::Vector3D( 0.476, -0.155,  0.657), 
	ATD::Vector3D( 0.294,  0.405,  0.657), 
	ATD::Vector3D(-0.294,  0.405,  0.657), 
	ATD::Vector3D(-0.476, -0.155,  0.657)
};
static const std::vector<ATD::Vector2L> _VERTICES_TEX = {
	ATD::Vector2L(32,  0), 
	ATD::Vector2L(62, 22), 
	ATD::Vector2L(51, 58), 
	ATD::Vector2L(13, 58), 
	ATD::Vector2L( 2, 22)
};
static const std::vector<ATD::Vector3D> _VERTICES_NRM = {
	ATD::Vector3D( 0.,  0., -1.), 

	ATD::Vector3D( 0.554,  0.763, -0.333), 
	ATD::Vector3D( 0.897, -0.291, -0.333), 
	ATD::Vector3D( 0.000, -0.943, -0.333), 
	ATD::Vector3D(-0.897, -0.291, -0.333), 
	ATD::Vector3D(-0.554,  0.763, -0.333), 

	ATD::Vector3D( 0.000,  0.943,  0.333), 
	ATD::Vector3D(-0.897,  0.291,  0.333), 
	ATD::Vector3D(-0.554, -0.763,  0.333), 
	ATD::Vector3D( 0.554, -0.763,  0.333), 
	ATD::Vector3D( 0.897,  0.291,  0.333), 

	ATD::Vector3D( 0.,  0.,  1.)
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

/* Front triangles must be walked through counter-clockwise, 
 * because Y coordinate is flipped while drawing. */
static const std::vector<ATD::Vertex3D> _VERTICES = {
	ATD::Vertex3D(_VERTICES_POS[ 0], _VERTICES_TEX[0], _VERTICES_NRM[ 0]), 
	ATD::Vertex3D(_VERTICES_POS[ 4], _VERTICES_TEX[4], _VERTICES_NRM[ 0]), 
	ATD::Vertex3D(_VERTICES_POS[ 3], _VERTICES_TEX[3], _VERTICES_NRM[ 0]), 
	ATD::Vertex3D(_VERTICES_POS[ 0], _VERTICES_TEX[0], _VERTICES_NRM[ 0]), 
	ATD::Vertex3D(_VERTICES_POS[ 3], _VERTICES_TEX[3], _VERTICES_NRM[ 0]), 
	ATD::Vertex3D(_VERTICES_POS[ 2], _VERTICES_TEX[2], _VERTICES_NRM[ 0]), 
	ATD::Vertex3D(_VERTICES_POS[ 0], _VERTICES_TEX[0], _VERTICES_NRM[ 0]), 
	ATD::Vertex3D(_VERTICES_POS[ 2], _VERTICES_TEX[2], _VERTICES_NRM[ 0]), 
	ATD::Vertex3D(_VERTICES_POS[ 1], _VERTICES_TEX[1], _VERTICES_NRM[ 0]), 

	ATD::Vertex3D(_VERTICES_POS[12], _VERTICES_TEX[0], _VERTICES_NRM[ 1]), 
	ATD::Vertex3D(_VERTICES_POS[ 5], _VERTICES_TEX[4], _VERTICES_NRM[ 1]), 
	ATD::Vertex3D(_VERTICES_POS[ 0], _VERTICES_TEX[3], _VERTICES_NRM[ 1]), 
	ATD::Vertex3D(_VERTICES_POS[12], _VERTICES_TEX[0], _VERTICES_NRM[ 1]), 
	ATD::Vertex3D(_VERTICES_POS[ 0], _VERTICES_TEX[3], _VERTICES_NRM[ 1]), 
	ATD::Vertex3D(_VERTICES_POS[ 1], _VERTICES_TEX[2], _VERTICES_NRM[ 1]), 
	ATD::Vertex3D(_VERTICES_POS[12], _VERTICES_TEX[0], _VERTICES_NRM[ 1]), 
	ATD::Vertex3D(_VERTICES_POS[ 1], _VERTICES_TEX[2], _VERTICES_NRM[ 1]), 
	ATD::Vertex3D(_VERTICES_POS[ 6], _VERTICES_TEX[1], _VERTICES_NRM[ 1]), 

	ATD::Vertex3D(_VERTICES_POS[11], _VERTICES_TEX[0], _VERTICES_NRM[ 2]), 
	ATD::Vertex3D(_VERTICES_POS[ 6], _VERTICES_TEX[4], _VERTICES_NRM[ 2]), 
	ATD::Vertex3D(_VERTICES_POS[ 1], _VERTICES_TEX[3], _VERTICES_NRM[ 2]), 
	ATD::Vertex3D(_VERTICES_POS[11], _VERTICES_TEX[0], _VERTICES_NRM[ 2]), 
	ATD::Vertex3D(_VERTICES_POS[ 1], _VERTICES_TEX[3], _VERTICES_NRM[ 2]), 
	ATD::Vertex3D(_VERTICES_POS[ 2], _VERTICES_TEX[2], _VERTICES_NRM[ 2]), 
	ATD::Vertex3D(_VERTICES_POS[11], _VERTICES_TEX[0], _VERTICES_NRM[ 2]), 
	ATD::Vertex3D(_VERTICES_POS[ 2], _VERTICES_TEX[2], _VERTICES_NRM[ 2]), 
	ATD::Vertex3D(_VERTICES_POS[ 7], _VERTICES_TEX[1], _VERTICES_NRM[ 2]), 

	ATD::Vertex3D(_VERTICES_POS[10], _VERTICES_TEX[0], _VERTICES_NRM[ 3]), 
	ATD::Vertex3D(_VERTICES_POS[ 7], _VERTICES_TEX[4], _VERTICES_NRM[ 3]), 
	ATD::Vertex3D(_VERTICES_POS[ 2], _VERTICES_TEX[3], _VERTICES_NRM[ 3]), 
	ATD::Vertex3D(_VERTICES_POS[10], _VERTICES_TEX[0], _VERTICES_NRM[ 3]), 
	ATD::Vertex3D(_VERTICES_POS[ 2], _VERTICES_TEX[3], _VERTICES_NRM[ 3]), 
	ATD::Vertex3D(_VERTICES_POS[ 3], _VERTICES_TEX[2], _VERTICES_NRM[ 3]), 
	ATD::Vertex3D(_VERTICES_POS[10], _VERTICES_TEX[0], _VERTICES_NRM[ 3]), 
	ATD::Vertex3D(_VERTICES_POS[ 3], _VERTICES_TEX[2], _VERTICES_NRM[ 3]), 
	ATD::Vertex3D(_VERTICES_POS[ 8], _VERTICES_TEX[1], _VERTICES_NRM[ 3]), 

	ATD::Vertex3D(_VERTICES_POS[14], _VERTICES_TEX[0], _VERTICES_NRM[ 4]), 
	ATD::Vertex3D(_VERTICES_POS[ 8], _VERTICES_TEX[4], _VERTICES_NRM[ 4]), 
	ATD::Vertex3D(_VERTICES_POS[ 3], _VERTICES_TEX[3], _VERTICES_NRM[ 4]), 
	ATD::Vertex3D(_VERTICES_POS[14], _VERTICES_TEX[0], _VERTICES_NRM[ 4]), 
	ATD::Vertex3D(_VERTICES_POS[ 3], _VERTICES_TEX[3], _VERTICES_NRM[ 4]), 
	ATD::Vertex3D(_VERTICES_POS[ 4], _VERTICES_TEX[2], _VERTICES_NRM[ 4]), 
	ATD::Vertex3D(_VERTICES_POS[14], _VERTICES_TEX[0], _VERTICES_NRM[ 4]), 
	ATD::Vertex3D(_VERTICES_POS[ 4], _VERTICES_TEX[2], _VERTICES_NRM[ 4]), 
	ATD::Vertex3D(_VERTICES_POS[ 9], _VERTICES_TEX[1], _VERTICES_NRM[ 4]), 

	ATD::Vertex3D(_VERTICES_POS[13], _VERTICES_TEX[0], _VERTICES_NRM[ 5]), 
	ATD::Vertex3D(_VERTICES_POS[ 9], _VERTICES_TEX[4], _VERTICES_NRM[ 5]), 
	ATD::Vertex3D(_VERTICES_POS[ 4], _VERTICES_TEX[3], _VERTICES_NRM[ 5]), 
	ATD::Vertex3D(_VERTICES_POS[13], _VERTICES_TEX[0], _VERTICES_NRM[ 5]), 
	ATD::Vertex3D(_VERTICES_POS[ 4], _VERTICES_TEX[3], _VERTICES_NRM[ 5]), 
	ATD::Vertex3D(_VERTICES_POS[ 0], _VERTICES_TEX[2], _VERTICES_NRM[ 5]), 
	ATD::Vertex3D(_VERTICES_POS[13], _VERTICES_TEX[0], _VERTICES_NRM[ 5]), 
	ATD::Vertex3D(_VERTICES_POS[ 0], _VERTICES_TEX[2], _VERTICES_NRM[ 5]), 
	ATD::Vertex3D(_VERTICES_POS[ 5], _VERTICES_TEX[1], _VERTICES_NRM[ 5]), 

	ATD::Vertex3D(_VERTICES_POS[ 5], _VERTICES_TEX[0], _VERTICES_NRM[ 6]), 
	ATD::Vertex3D(_VERTICES_POS[12], _VERTICES_TEX[4], _VERTICES_NRM[ 6]), 
	ATD::Vertex3D(_VERTICES_POS[17], _VERTICES_TEX[3], _VERTICES_NRM[ 6]), 
	ATD::Vertex3D(_VERTICES_POS[ 5], _VERTICES_TEX[0], _VERTICES_NRM[ 6]), 
	ATD::Vertex3D(_VERTICES_POS[17], _VERTICES_TEX[3], _VERTICES_NRM[ 6]), 
	ATD::Vertex3D(_VERTICES_POS[18], _VERTICES_TEX[2], _VERTICES_NRM[ 6]), 
	ATD::Vertex3D(_VERTICES_POS[ 5], _VERTICES_TEX[0], _VERTICES_NRM[ 6]), 
	ATD::Vertex3D(_VERTICES_POS[18], _VERTICES_TEX[2], _VERTICES_NRM[ 6]), 
	ATD::Vertex3D(_VERTICES_POS[13], _VERTICES_TEX[1], _VERTICES_NRM[ 6]), 

	ATD::Vertex3D(_VERTICES_POS[ 9], _VERTICES_TEX[0], _VERTICES_NRM[ 7]), 
	ATD::Vertex3D(_VERTICES_POS[13], _VERTICES_TEX[4], _VERTICES_NRM[ 7]), 
	ATD::Vertex3D(_VERTICES_POS[18], _VERTICES_TEX[3], _VERTICES_NRM[ 7]), 
	ATD::Vertex3D(_VERTICES_POS[ 9], _VERTICES_TEX[0], _VERTICES_NRM[ 7]), 
	ATD::Vertex3D(_VERTICES_POS[18], _VERTICES_TEX[3], _VERTICES_NRM[ 7]), 
	ATD::Vertex3D(_VERTICES_POS[19], _VERTICES_TEX[2], _VERTICES_NRM[ 7]), 
	ATD::Vertex3D(_VERTICES_POS[ 9], _VERTICES_TEX[0], _VERTICES_NRM[ 7]), 
	ATD::Vertex3D(_VERTICES_POS[19], _VERTICES_TEX[2], _VERTICES_NRM[ 7]), 
	ATD::Vertex3D(_VERTICES_POS[14], _VERTICES_TEX[1], _VERTICES_NRM[ 7]), 

	ATD::Vertex3D(_VERTICES_POS[ 8], _VERTICES_TEX[0], _VERTICES_NRM[ 8]), 
	ATD::Vertex3D(_VERTICES_POS[14], _VERTICES_TEX[4], _VERTICES_NRM[ 8]), 
	ATD::Vertex3D(_VERTICES_POS[19], _VERTICES_TEX[3], _VERTICES_NRM[ 8]), 
	ATD::Vertex3D(_VERTICES_POS[ 8], _VERTICES_TEX[0], _VERTICES_NRM[ 8]), 
	ATD::Vertex3D(_VERTICES_POS[19], _VERTICES_TEX[3], _VERTICES_NRM[ 8]), 
	ATD::Vertex3D(_VERTICES_POS[15], _VERTICES_TEX[2], _VERTICES_NRM[ 8]), 
	ATD::Vertex3D(_VERTICES_POS[ 8], _VERTICES_TEX[0], _VERTICES_NRM[ 8]), 
	ATD::Vertex3D(_VERTICES_POS[15], _VERTICES_TEX[2], _VERTICES_NRM[ 8]), 
	ATD::Vertex3D(_VERTICES_POS[10], _VERTICES_TEX[1], _VERTICES_NRM[ 8]), 

	ATD::Vertex3D(_VERTICES_POS[ 7], _VERTICES_TEX[0], _VERTICES_NRM[ 9]), 
	ATD::Vertex3D(_VERTICES_POS[10], _VERTICES_TEX[4], _VERTICES_NRM[ 9]), 
	ATD::Vertex3D(_VERTICES_POS[15], _VERTICES_TEX[3], _VERTICES_NRM[ 9]), 
	ATD::Vertex3D(_VERTICES_POS[ 7], _VERTICES_TEX[0], _VERTICES_NRM[ 9]), 
	ATD::Vertex3D(_VERTICES_POS[15], _VERTICES_TEX[3], _VERTICES_NRM[ 9]), 
	ATD::Vertex3D(_VERTICES_POS[16], _VERTICES_TEX[2], _VERTICES_NRM[ 9]), 
	ATD::Vertex3D(_VERTICES_POS[ 7], _VERTICES_TEX[0], _VERTICES_NRM[ 9]), 
	ATD::Vertex3D(_VERTICES_POS[16], _VERTICES_TEX[2], _VERTICES_NRM[ 9]), 
	ATD::Vertex3D(_VERTICES_POS[11], _VERTICES_TEX[1], _VERTICES_NRM[ 9]), 

	ATD::Vertex3D(_VERTICES_POS[ 6], _VERTICES_TEX[0], _VERTICES_NRM[10]), 
	ATD::Vertex3D(_VERTICES_POS[11], _VERTICES_TEX[4], _VERTICES_NRM[10]), 
	ATD::Vertex3D(_VERTICES_POS[16], _VERTICES_TEX[3], _VERTICES_NRM[10]), 
	ATD::Vertex3D(_VERTICES_POS[ 6], _VERTICES_TEX[0], _VERTICES_NRM[10]), 
	ATD::Vertex3D(_VERTICES_POS[16], _VERTICES_TEX[3], _VERTICES_NRM[10]), 
	ATD::Vertex3D(_VERTICES_POS[17], _VERTICES_TEX[2], _VERTICES_NRM[10]), 
	ATD::Vertex3D(_VERTICES_POS[ 6], _VERTICES_TEX[0], _VERTICES_NRM[10]), 
	ATD::Vertex3D(_VERTICES_POS[17], _VERTICES_TEX[2], _VERTICES_NRM[10]), 
	ATD::Vertex3D(_VERTICES_POS[12], _VERTICES_TEX[1], _VERTICES_NRM[10]), 

	ATD::Vertex3D(_VERTICES_POS[15], _VERTICES_TEX[0], _VERTICES_NRM[11]), 
	ATD::Vertex3D(_VERTICES_POS[19], _VERTICES_TEX[4], _VERTICES_NRM[11]), 
	ATD::Vertex3D(_VERTICES_POS[18], _VERTICES_TEX[3], _VERTICES_NRM[11]), 
	ATD::Vertex3D(_VERTICES_POS[15], _VERTICES_TEX[0], _VERTICES_NRM[11]), 
	ATD::Vertex3D(_VERTICES_POS[18], _VERTICES_TEX[3], _VERTICES_NRM[11]), 
	ATD::Vertex3D(_VERTICES_POS[17], _VERTICES_TEX[2], _VERTICES_NRM[11]), 
	ATD::Vertex3D(_VERTICES_POS[15], _VERTICES_TEX[0], _VERTICES_NRM[11]), 
	ATD::Vertex3D(_VERTICES_POS[17], _VERTICES_TEX[2], _VERTICES_NRM[11]), 
	ATD::Vertex3D(_VERTICES_POS[16], _VERTICES_TEX[1], _VERTICES_NRM[11])
};


/* Model: */

Model::Model(const ATD::Image &texture)
	: ATD::FrameBuffer::Drawable()
/*	, m_shaderPtr(new ATD::Shader3D(_SHADER_VERTEX_SOURCE, 
				_SHADER_FRAGMENT_SOURCE)) */
	, m_texturePtr(nullptr)
	, m_verticesPtr(nullptr)
	, m_transform()
{
	/* IPRINTF("", "Texture size: (%lu x %lu)", texture.Size().x, 
			texture.Size().y); // DEBUG */

	m_texturePtr = ATD::Texture::Ptr(new ATD::Texture(texture));

	/* Setup vertices: */
	m_verticesPtr = ATD::VertexBuffer3D::Ptr(
			new ATD::VertexBuffer3D(_VERTICES, texture.Size()));

	/* Enable triangle culling: */
	ATD::gl._FrontFace(ATD::Gl::CW);
	ATD::gl._CullFace(ATD::Gl::BACK);
	ATD::gl._Enable(ATD::Gl::CULL_FACE);

	/* IPRINTF("", "sizeof(ATD::Vertex3D::GlVertex) == %lu", 
			sizeof(ATD::Vertex3D::GlVertex)); // DEBUG */
	/* IPRINTF("", "VertexBuffer3D size: %lu", 
			m_verticesPtr->Size()); // DEBUG */
	/* IPRINTF("", "vertex shader source:\n%s", 
			_SHADER_VERTEX_SOURCE.c_str()); // DEBUG */
	/* IPRINTF("", "fragment shader source:\n%s", 
			_SHADER_FRAGMENT_SOURCE.c_str()); // DEBUG */

	/* const ATD::VertexBuffer3D::AttrIndices &attrIndices = 
			m_shaderPtr->GetAttrIndices();
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

void Model::Rotate(const ATD::Vector2D &rotationXYFrc, 
		const ATD::Vector3D &axisX, 
		const ATD::Vector3D &axisY)
{
	ATD::Quaternion rotationCur = m_transform.Rotation();
	ATD::Vector3D axisXCur = rotationCur.Inverted().ApplyRotation(axisX);
	ATD::Vector3D axisYCur = rotationCur.Inverted().ApplyRotation(axisY);

	ATD::Quaternion rotateX = ATD::Quaternion::Rotation(rotationXYFrc.x, 
			axisXCur);
	ATD::Quaternion rotateY = ATD::Quaternion::Rotation(rotationXYFrc.y, 
			axisYCur);

	m_transform.SetRotation(rotateX * rotateY * rotationCur);
}

void Model::DrawSelf(ATD::FrameBuffer &target) const
{
	ATD::Texture::Usage useTexture(*m_texturePtr);
	target.Draw(*m_verticesPtr, 
			m_transform);

/*	m_shaderPtr->SetUniform("unfTransform", ATD::Matrix4F());
	m_shaderPtr->SetUniform("unfProject", ATD::Matrix4F());

	ATD::FrameBuffer::Usage useFBuffer(target);
	ATD::Shader::Usage useShader(*m_shaderPtr);

	m_verticesPtr->DrawSelfInternal(
			m_shaderPtr->GetAttrIndices()); // DEBUG */
}


