
#include <ATD/Core/Debug.hpp>
#include <ATD/Core/Transform2D.hpp>
#include <ATD/Graphics/VertexBuffer2D.hpp>
#include <ATD/Graphics/Image.hpp>
#include <ATD/Graphics/Shader.hpp>
#include <ATD/Graphics/Texture.hpp>
#include <ATD/Graphics/Vertex2D.hpp>
#include <ATD/Window/Window.hpp>

#include <string>


class Sprite : public ATD::FrameBuffer::Drawable
{
public:
	Sprite(const ATD::Image &image);

	void DrawSelf(ATD::FrameBuffer &target) const;

	void RotateCw(double angleDeltaFrc);

	void Shift(const ATD::Vector2D &offsetDelta);

private:
	ATD::Texture m_texture;
	ATD::VertexBuffer2D m_vertices;
	ATD::Transform2D m_transform;
};


/* === */

Sprite::Sprite(const ATD::Image &image)
	: ATD::FrameBuffer::Drawable()
	, m_texture(image)
	, m_vertices(ATD::RectL(image.Size()), image.Size())
	, m_transform()
{
	IPRINTF("", "texture : %u, vertices : %u", 
			m_texture.GetGlId(), m_vertices.GetGlId());
}

void Sprite::DrawSelf(ATD::FrameBuffer &target) const
{
	ATD::Texture::Usage useTexture(m_texture);
	target.Draw(m_vertices, m_transform);
}

void Sprite::RotateCw(double angleDeltaFrc)
{
	m_transform.SetAngleFrc(m_transform.AngleFrc() + angleDeltaFrc);
}

void Sprite::Shift(const ATD::Vector2D &offsetDelta)
{
	m_transform.SetOffset(m_transform.Offset() + offsetDelta);
}


