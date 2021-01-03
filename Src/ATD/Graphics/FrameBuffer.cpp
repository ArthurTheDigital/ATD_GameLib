/**
 * @file      
 * @brief     Wrap around OpenGl frame buffer.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */


#include <ATD/Graphics/FrameBuffer.hpp>

#include <ATD/Core/Debug.hpp>
#include <ATD/Core/Printf.hpp>
#include <ATD/Graphics/GlCheck.hpp>

#include <stdexcept>


/* ATD::FrameBuffer::Drawable: */

ATD::FrameBuffer::Drawable::Drawable()
{}

ATD::FrameBuffer::Drawable::~Drawable()
{}


/* ATD::FrameBuffer::Usage: */

ATD::FrameBuffer::Usage::Usage(const ATD::FrameBuffer &frameBuffer)
	: m_prevFrameBuffer(0)
	, m_activated(false)
{
	gl._GetIntegerv(Gl::FRAMEBUFFER_BINDING, 
			reinterpret_cast<Gl::Int *>(&m_prevFrameBuffer));

	if (m_prevFrameBuffer != frameBuffer.GetGlId()) {
		gl._BindFramebuffer(Gl::FRAMEBUFFER, frameBuffer.GetGlId());
		m_activated = true;
	}
}

ATD::FrameBuffer::Usage::~Usage()
{
	if (m_activated) {
		gl._BindFramebuffer(ATD::Gl::FRAMEBUFFER, m_prevFrameBuffer);
	}
}


/* ATD::FrameBuffer auxiliary: */

ATD::Transform2D Coords2DScaleTransform(const ATD::Vector2S &size)
{
	ATD::Transform2D transform;
	transform.SetScale(ATD::Vector2D(2.f / static_cast<float>(size.x), 
				2.f / static_cast<float>(size.y)));

	return transform;
}

ATD::Transform2D Coords2DOffsetTransform()
{
	ATD::Transform2D transform;
	transform.SetOffset(ATD::Vector2D(-1., -1.));

	return transform;
}

/* Textures, retrieved from FrameBuffer are flipped by Y-axis. To solve 
 * this problem I decided to draw all the 3D objects Y-flipped. */
ATD::Matrix4F YFlip3D()
{
	ATD::Matrix4F yFlip;
	yFlip[1][1] = -1.f;

	return yFlip;
}


/* ATD::FrameBuffer auxiliary: basic shader: */

ATD::Shader2D::Ptr _BasicShader2DPtr(nullptr);
std::recursive_mutex _BasicShader2DMtx;


/* ATD::FrameBuffer: */

ATD::FrameBuffer::FrameBuffer(const ATD::Vector2S &size, 
		uint32_t enable)
	: m_size(size)
	, m_frameBufferId(0)
	, m_enableMask(enable)
	, m_colorTexturePtr(nullptr)
	, m_depthTexturePtr(nullptr)

	, m_dftShader2DPtr(
			new Shader2D(
				Shader2D::ALPHA_VERTEX_SOURCE, 
				Shader2D::ALPHA_FRAGMENT_SOURCE))
	, m_shader2DPtr(nullptr)
	, m_projection2D()
	, m_coords2DScale(Coords2DScaleTransform(size))
	, m_coords2DOffset(Coords2DOffsetTransform())

	, m_dftShader3DPtr(
			new Shader3D(
				Shader3D::PLAIN_VERTEX_SOURCE, 
				Shader3D::PLAIN_FRAGMENT_SOURCE))
	, m_shader3DPtr(nullptr)
	, m_projection3D()
{
	GL_CHECK("", gl._GenFramebuffers(1, &m_frameBufferId));
	Usage useFB(*this);

	if (m_enableMask & ENABLE_COLOR) {
		m_colorTexturePtr = Texture::Ptr(new Texture(m_size, Pixel(), 
					Texture::COLOR));

		gl._FramebufferTexture2D(Gl::FRAMEBUFFER, 
				Gl::COLOR_ATTACHMENT0, /* attachment */
				Gl::TEXTURE_2D, /* textarget */
				m_colorTexturePtr->GetGlId(), 
				0);
	}

	if (m_enableMask & ENABLE_DEPTH) {
		m_depthTexturePtr = Texture::Ptr(new Texture(m_size, Pixel(), 
					Texture::DEPTH));

		gl._FramebufferTexture2D(Gl::FRAMEBUFFER, 
				Gl::DEPTH_ATTACHMENT, /* attachment */
				Gl::TEXTURE_2D, /* textarget */
				m_depthTexturePtr->GetGlId(), 
				0);
	}

	{
		Gl::Enum status = gl._CheckFramebufferStatus(Gl::FRAMEBUFFER);
		if (status != Gl::FRAMEBUFFER_COMPLETE) {
			// TODO: Add more details
			throw std::runtime_error(Printf("incomplete frame buffer"));
		}
	}

	SetupShader2D(*m_dftShader2DPtr);
	SetupShader3D(*m_dftShader3DPtr);
}

ATD::FrameBuffer::FrameBuffer(const ATD::FrameBuffer &other, 
		const ATD::Vector2S &size, 
		const ATD::Align &alignX, 
		const ATD::Align &alignY)
	: m_size(size)
	, m_frameBufferId(0)
	, m_enableMask(other.m_enableMask)
	, m_colorTexturePtr(nullptr)
	, m_depthTexturePtr(nullptr)

	, m_dftShader2DPtr(other.m_dftShader2DPtr)
	, m_shader2DPtr(other.m_shader2DPtr)
	, m_projection2D(other.m_projection2D)
	, m_coords2DScale(Coords2DScaleTransform(size))
	, m_coords2DOffset(Coords2DOffsetTransform())

	, m_dftShader3DPtr(other.m_dftShader3DPtr)
	, m_shader3DPtr(other.m_shader3DPtr)
	, m_projection3D(other.m_projection3D)
{
	GL_CHECK("", gl._GenFramebuffers(1, &m_frameBufferId));
	Usage useFB(*this);

	if (m_enableMask & ENABLE_COLOR) {
		/* IPRINTF("", "color"); // DEBUG */

		m_colorTexturePtr = Texture::Ptr(new Texture(m_size, Pixel(), 
					Texture::COLOR));

		/* Draw contents of other's color texture on new color texture. */
		DrawPlainAligned(*other.m_colorTexturePtr, *m_colorTexturePtr, 
				alignX, alignY);

		GL_CHECK("", gl._FramebufferTexture2D(Gl::FRAMEBUFFER, 
				Gl::COLOR_ATTACHMENT0, /* attachment */
				Gl::TEXTURE_2D, /* textarget */
				m_colorTexturePtr->GetGlId(), 
				0));
	}

	if (m_enableMask & ENABLE_DEPTH) {
		/* IPRINTF("", "depth"); // DEBUG */

		m_depthTexturePtr = Texture::Ptr(new Texture(m_size, Pixel(), 
					Texture::DEPTH));

		/* Draw contents of other's depth texture on new depth texture. */
		// DrawPlainAligned(*other.m_depthTexturePtr, *m_depthTexturePtr, 
		//		alignX, alignY);
		/* FIXME: Temporary frame buffer is incomplete after attaching 
		 * depth texture as color. Shall find a better way to copy textures. */

		GL_CHECK("", gl._FramebufferTexture2D(Gl::FRAMEBUFFER, 
				Gl::DEPTH_ATTACHMENT, /* attachment */
				Gl::TEXTURE_2D, /* textarget */
				m_depthTexturePtr->GetGlId(), 
				0));
	}

	{
		Gl::Enum status = gl._CheckFramebufferStatus(Gl::FRAMEBUFFER);
		if (status != Gl::FRAMEBUFFER_COMPLETE) {
			throw std::runtime_error(
					Printf(
						"incomplete frame buffer %u: 0x%x", 
						GetGlId(), 
						static_cast<unsigned>(status)));
		}
	}

	SetupShader2D(*m_dftShader2DPtr);
	SetupShader3D(*m_dftShader3DPtr);
}

ATD::FrameBuffer::~FrameBuffer()
{
	/* If bound via 'Usage' subclass, it is already unbound automatically. 
	 * Textures get deinited automatically when all smart pointers destroyed. 
	 * No other deinit required. */
	gl._DeleteFramebuffers(1, &m_frameBufferId);
}

uint32_t ATD::FrameBuffer::GetEnableMask() const
{ return m_enableMask; }

ATD::Gl::Uint ATD::FrameBuffer::GetGlId() const
{ return m_frameBufferId; }

ATD::Vector2S ATD::FrameBuffer::Size() const
{ return m_size; }

double ATD::FrameBuffer::AspectRatio() const
{
	double aspectRatio = (m_size.x > 0 && m_size.y > 0) ?  
		(static_cast<double>(m_size.x) / static_cast<double>(m_size.y)) : 1.;

	return aspectRatio;
}

void ATD::FrameBuffer::SetShader2D(ATD::Shader2D::Ptr shader2DPtr)
{
	m_shader2DPtr = shader2DPtr;
	Shader2D &shader = m_shader2DPtr ? *m_shader2DPtr : *m_dftShader2DPtr;
	SetupShader2D(shader);
}

void ATD::FrameBuffer::SetShader3D(ATD::Shader3D::Ptr shader3DPtr)
{
	m_shader3DPtr = shader3DPtr;
	Shader3D &shader = m_shader3DPtr ? *m_shader3DPtr : *m_dftShader3DPtr;
	SetupShader3D(shader);
}

void ATD::FrameBuffer::SetProjection2D(const ATD::Projection2D &projection2D)
{
	m_projection2D = projection2D;
	Shader2D &shader = m_shader2DPtr ? *m_shader2DPtr : *m_dftShader2DPtr;
	UpdateProjectionOnShader2D(shader);
}

void ATD::FrameBuffer::SetProjection3D(const ATD::Projection3D &projection3D)
{
	m_projection3D = projection3D;
	Shader3D &shader = m_shader3DPtr ? *m_shader3DPtr : *m_dftShader3DPtr;
	UpdateProjectionOnShader3D(shader);
}

void ATD::FrameBuffer::Clear()
{
	Usage useFB(*this);
	gl._Clear(Gl::COLOR_BUFFER_BIT | Gl::DEPTH_BUFFER_BIT);
}

void ATD::FrameBuffer::Draw(const Drawable &drawable)
{ drawable.DrawSelf(*this); }

void ATD::FrameBuffer::Draw(const ATD::VertexBuffer2D &vertices2D, 
		const ATD::Transform2D &transform)
{
	Shader2D &shader2D = m_shader2DPtr ? *m_shader2DPtr : *m_dftShader2DPtr;
	shader2D.SetUniform("unfTransform", 
			(m_coords2DOffset.GetMatrix() *
				m_coords2DScale.GetMatrix() * 
				transform.GetMatrix()));

	/* Texture::Usage is expected to be set before. */
	if (shader2D.IsCanvasRequired() && m_colorTexturePtr) {
		Texture::Usage useCanvas(*m_colorTexturePtr, Texture::TEX_7);

		Usage useFBuffer(*this);
		Shader::Usage useShader(shader2D);
		gl._Viewport(0, 0, m_size.x, m_size.y);
		/* https://stackoverflow.com/questions/33718237/\
		 * do-you-have-to-call-glviewport-every-time-you-bind-\
		 * a-frame-buffer-with-a-differe */

		vertices2D.DrawSelfInternal(shader2D.GetAttrIndices());
	} else {
		Usage useFBuffer(*this);
		Shader::Usage useShader(shader2D);
		gl._Viewport(0, 0, m_size.x, m_size.y);
		/* https://stackoverflow.com/questions/33718237/\
		 * do-you-have-to-call-glviewport-every-time-you-bind-\
		 * a-frame-buffer-with-a-differe */

		vertices2D.DrawSelfInternal(shader2D.GetAttrIndices());
	}
}

void ATD::FrameBuffer::Draw(const ATD::VertexBuffer3D &vertices3D, 
		const ATD::Transform3D &transform)
{

	Shader3D &shader3D = m_shader3DPtr ? *m_shader3DPtr : *m_dftShader3DPtr;
	shader3D.SetUniform("unfTransform", transform.GetMatrix());

	/* Texture::Usage is expected to be set before. */
	if (shader3D.IsCanvasRequired() && m_colorTexturePtr) {
		Texture::Usage useCanvas(*m_colorTexturePtr, Texture::TEX_7);

		Usage useFBuffer(*this);
		Shader::Usage useShader(shader3D);
		gl._Viewport(0, 0, m_size.x, m_size.y);

		vertices3D.DrawSelfInternal(shader3D.GetAttrIndices());
	} else {
		Usage useFBuffer(*this);
		Shader::Usage useShader(shader3D);
		gl._Viewport(0, 0, m_size.x, m_size.y);

		vertices3D.DrawSelfInternal(shader3D.GetAttrIndices());
	}
}

ATD::Texture::CPtr ATD::FrameBuffer::GetColorTexture() const
{ return static_cast<Texture::CPtr>(m_colorTexturePtr); }

ATD::Texture::CPtr ATD::FrameBuffer::GetDepthTexture() const
{ return static_cast<Texture::CPtr>(m_depthTexturePtr); }

void ATD::FrameBuffer::SetupShader2D(Shader2D &shader) const
{
	UpdateCanvasUniformsOnShader(static_cast<Shader &>(shader));
	UpdateProjectionOnShader2D(shader);
}

void ATD::FrameBuffer::SetupShader3D(Shader3D &shader) const
{
	UpdateCanvasUniformsOnShader(static_cast<Shader &>(shader));
	UpdateProjectionOnShader3D(shader);
}

void ATD::FrameBuffer::UpdateCanvasUniformsOnShader(Shader &shader) const
{
	if (shader.IsCanvasRequired()) {
		shader.SetUniform("unfCanvasSize", 
				static_cast<Vector2F>(m_size));

		shader.SetUniformSampler2DUnit("unfCanvasTexture", 
				Texture::TEX_7);
	}
}

void ATD::FrameBuffer::UpdateProjectionOnShader2D(Shader2D &shader) const
{
	shader.SetUniform("unfProject", m_projection2D.GetMatrix());
}

void ATD::FrameBuffer::UpdateProjectionOnShader3D(Shader3D &shader) const
{
	shader.SetUniform("unfProject", 
			YFlip3D() * m_projection3D.GetMatrix(AspectRatio()));
}

void ATD::FrameBuffer::DrawPlainAligned(const ATD::Texture &texSrc, 
		ATD::Texture &texDst, 
		const ATD::Align &alignX, 
		const ATD::Align &alignY)
{
	long deltaX = texDst.Size().x - texSrc.Size().x;
	long deltaY = texDst.Size().y - texDst.Size().y;

	Vector2L offset;

	offset.x = (alignX == Align::LOWER) ? 0 : 
		(alignX == Align::CENTER) ? deltaX / 2 : 
		deltaX;

	offset.y = (alignY == Align::LOWER) ? 0 : 
		(alignY == Align::CENTER) ? deltaY / 2 : 
		deltaY;

	/* Init vertices. */
	VertexBuffer2D vertices(RectL(texSrc.Size()), texSrc.Size());

	/* Init transform. */
	Transform2D transform;
	transform.SetOffset(static_cast<ATD::Vector2D>(offset));	

	/* Create temporary frame buffer. */
	Gl::Uint frameBufferTmpId = 0;
	GL_CHECK("", gl._GenFramebuffers(1, &frameBufferTmpId));

	/* Remember the previously bound frame buffer and use the 
	 * temporary one. */
	Gl::Uint frameBufferPrevId = 0;
	gl._GetIntegerv(Gl::FRAMEBUFFER_BINDING, 
			reinterpret_cast<Gl::Int *>(&frameBufferPrevId));

	GL_CHECK("", gl._BindFramebuffer(Gl::FRAMEBUFFER, frameBufferTmpId));

	/* Attach dst texture to the frame buffer as color. */
	GL_CHECK("", gl._FramebufferTexture2D(Gl::FRAMEBUFFER, 
			Gl::COLOR_ATTACHMENT0, /* attachment */
			Gl::TEXTURE_2D, /* textarget */
			texDst.GetGlId(), 
			0));

	/* Check temporary frame buffer. */
	{
		Gl::Enum status = gl._CheckFramebufferStatus(Gl::FRAMEBUFFER);
		if (status != Gl::FRAMEBUFFER_COMPLETE) {
			/* Restore previous frame buffer. */
			GL_CHECK("", gl._BindFramebuffer(Gl::FRAMEBUFFER, 
						frameBufferPrevId));

			/* Delete temporary frame buffer. */
			gl._DeleteFramebuffers(1, &frameBufferTmpId);

			throw std::runtime_error(
					Printf(
						"incomplete frame buffer %u: 0x%x", 
						GetGlId(), 
						static_cast<unsigned>(status)));
		}
	}

	/* Do drawing with basic 2D shader. */
	{
		std::lock_guard<std::recursive_mutex> lock(_BasicShader2DMtx);

		if (!_BasicShader2DPtr) {
			_BasicShader2DPtr = 
				Shader2D::Ptr(
						new Shader2D(
							Shader2D::PLAIN_VERTEX_SOURCE, 
							Shader2D::PLAIN_FRAGMENT_SOURCE));
		}

		Transform2D tmpCoords2DScale = Coords2DScaleTransform(texDst.Size());
		Transform2D tmpCoords2DOffset = Coords2DOffsetTransform();

		_BasicShader2DPtr->SetUniform("unfProject", 
				Projection2D().GetMatrix());

		_BasicShader2DPtr->SetUniform("unfTransform", 
				(tmpCoords2DOffset.GetMatrix() *
					tmpCoords2DScale.GetMatrix() * 
					transform.GetMatrix()));

		Texture::Usage useTexture(texSrc);

		Shader::Usage useShader(*_BasicShader2DPtr);

		GL_CHECK("", vertices.DrawSelfInternal(
					_BasicShader2DPtr->GetAttrIndices()));
	}

	/* Restore previous frame buffer. */
	GL_CHECK("", gl._BindFramebuffer(Gl::FRAMEBUFFER, frameBufferPrevId));

	/* Delete temporary frame buffer. */
	gl._DeleteFramebuffers(1, &frameBufferTmpId);
}


