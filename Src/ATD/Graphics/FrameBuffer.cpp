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


/* ATD::FrameBuffer::Drawable2D: */

ATD::FrameBuffer::Drawable2D::Drawable2D()
	: Drawable()
	, m_transform()
{}

ATD::FrameBuffer::Drawable2D::~Drawable2D()
{}

void ATD::FrameBuffer::Drawable2D::setTransform(
		const ATD::Transform2D &transform)
{
	m_transform = transform;
}

void ATD::FrameBuffer::Drawable2D::setScale(const ATD::Vector2D &scale)
{
	m_transform.setScale(scale);
}

void ATD::FrameBuffer::Drawable2D::setAngleFrc(double angleFrc)
{
	m_transform.setAngleFrc(angleFrc);
}

void ATD::FrameBuffer::Drawable2D::setOffset(const ATD::Vector2D &offset)
{
	m_transform.setOffset(offset);
}


/* ATD::FrameBuffer::Usage: */

ATD::FrameBuffer::Usage::Usage(const ATD::FrameBuffer &frameBuffer)
	: m_prevFrameBuffer(0)
	, m_activated(false)
{
	gl.getIntegerv(Gl::FRAMEBUFFER_BINDING, 
			reinterpret_cast<Gl::Int *>(&m_prevFrameBuffer));

	if (m_prevFrameBuffer != frameBuffer.glId()) {
		gl.bindFramebuffer(Gl::FRAMEBUFFER, frameBuffer.glId());
		m_activated = true;
	}
}

ATD::FrameBuffer::Usage::~Usage()
{
	if (m_activated) {
		gl.bindFramebuffer(ATD::Gl::FRAMEBUFFER, m_prevFrameBuffer);
	}
}


/* ATD::FrameBuffer auxiliary: */

ATD::Transform2D _coords2DScaleTransform(const ATD::Vector2S &size)
{
	ATD::Transform2D transform;
	transform.setScale(ATD::Vector2D(2.f / static_cast<float>(size.x), 
				2.f / static_cast<float>(size.y)));

	return transform;
}

ATD::Transform2D _coords2DOffsetTransform()
{
	ATD::Transform2D transform;
	transform.setOffset(ATD::Vector2D(-1., -1.));

	return transform;
}

/* Textures, retrieved from FrameBuffer are flipped by Y-axis. To solve 
 * this problem I decided to draw all the 3D objects Y-flipped. */
ATD::Matrix4F _yFlip3D()
{
	ATD::Matrix4F yFlip;
	yFlip[1][1] = -1.f;

	return yFlip;
}


/* ATD::FrameBuffer auxiliary: basic shader: */

ATD::Shader2D::Ptr _basicShader2DPtr(nullptr);
std::recursive_mutex _basicShader2DMtx;


/* ATD::FrameBuffer: */

ATD::FrameBuffer::FrameBuffer(const ATD::Vector2S &size, 
		uint32_t bufferMask)
	: m_size(size)
	, m_frameBufferId(0)
	, m_bufferMask(bufferMask)
	, m_colorTexturePtr(nullptr)
	, m_depthTexturePtr(nullptr)

	, m_dftShader2DPtr(
			new Shader2D(
				Shader2D::ALPHA_VERTEX_SOURCE, 
				Shader2D::ALPHA_FRAGMENT_SOURCE))
	, m_shader2DPtr(nullptr)
	, m_projection2D()
	, m_coords2DScale(_coords2DScaleTransform(size))
	, m_coords2DOffset(_coords2DOffsetTransform())

	, m_dftShader3DPtr(
			new Shader3D(
				Shader3D::PLAIN_VERTEX_SOURCE, 
				Shader3D::PLAIN_FRAGMENT_SOURCE))
	, m_shader3DPtr(nullptr)
	, m_projection3D()
{
	GL_CHECK("", gl.genFramebuffers(1, &m_frameBufferId));
	Usage useFB(*this);

	if (m_bufferMask & BUFFER_COLOR) {
		m_colorTexturePtr = Texture::Ptr(new Texture(m_size, Pixel(), 
					Texture::COLOR));

		gl.framebufferTexture2D(Gl::FRAMEBUFFER, 
				Gl::COLOR_ATTACHMENT0, /* attachment */
				Gl::TEXTURE_2D, /* textarget */
				m_colorTexturePtr->glId(), 
				0);
	}

	if (m_bufferMask & BUFFER_DEPTH) {
		m_depthTexturePtr = Texture::Ptr(new Texture(m_size, Pixel(), 
					Texture::DEPTH));

		gl.framebufferTexture2D(Gl::FRAMEBUFFER, 
				Gl::DEPTH_ATTACHMENT, /* attachment */
				Gl::TEXTURE_2D, /* textarget */
				m_depthTexturePtr->glId(), 
				0);
	}

	{
		Gl::Enum status = gl.checkFramebufferStatus(Gl::FRAMEBUFFER);
		if (status != Gl::FRAMEBUFFER_COMPLETE) {
			// TODO: Add more details
			throw std::runtime_error(Aux::printf("incomplete frame buffer"));
		}
	}

	/* Must go before 'setupShader3D'! */
	m_projection3D.setAspectRatio(aspectRatio());

	/* === */

	setupShader2D(*m_dftShader2DPtr);
	setupShader3D(*m_dftShader3DPtr);
}

ATD::FrameBuffer::FrameBuffer(const ATD::FrameBuffer &other, 
		const ATD::Vector2S &size, 
		const ATD::Align &alignX, 
		const ATD::Align &alignY)
	: m_size(size)
	, m_frameBufferId(0)
	, m_bufferMask(other.m_bufferMask)
	, m_colorTexturePtr(nullptr)
	, m_depthTexturePtr(nullptr)

	, m_dftShader2DPtr(other.m_dftShader2DPtr)
	, m_shader2DPtr(other.m_shader2DPtr)
	, m_projection2D(other.m_projection2D)
	, m_coords2DScale(_coords2DScaleTransform(size))
	, m_coords2DOffset(_coords2DOffsetTransform())

	, m_dftShader3DPtr(other.m_dftShader3DPtr)
	, m_shader3DPtr(other.m_shader3DPtr)
	, m_projection3D(other.m_projection3D)
{
	GL_CHECK("", gl.genFramebuffers(1, &m_frameBufferId));
	Usage useFB(*this);

	if (m_bufferMask & BUFFER_COLOR) {
		/* IPRINTF("", "color"); // DEBUG */

		m_colorTexturePtr = Texture::Ptr(new Texture(m_size, Pixel(), 
					Texture::COLOR));

		/* Draw contents of other's color texture on new color texture. */
		drawPlainAligned(*other.m_colorTexturePtr, *m_colorTexturePtr, 
				alignX, alignY);

		GL_CHECK("", gl.framebufferTexture2D(Gl::FRAMEBUFFER, 
				Gl::COLOR_ATTACHMENT0, /* attachment */
				Gl::TEXTURE_2D, /* textarget */
				m_colorTexturePtr->glId(), 
				0));
	}

	if (m_bufferMask & BUFFER_DEPTH) {
		/* IPRINTF("", "depth"); // DEBUG */

		m_depthTexturePtr = Texture::Ptr(new Texture(m_size, Pixel(), 
					Texture::DEPTH));

		/* Draw contents of other's depth texture on new depth texture. */
		// drawPlainAligned(*other.m_depthTexturePtr, *m_depthTexturePtr, 
		//		alignX, alignY);
		/* FIXME: Temporary frame buffer is incomplete after attaching 
		 * depth texture as color. Shall find a better way to copy textures. */

		GL_CHECK("", gl.framebufferTexture2D(Gl::FRAMEBUFFER, 
				Gl::DEPTH_ATTACHMENT, /* attachment */
				Gl::TEXTURE_2D, /* textarget */
				m_depthTexturePtr->glId(), 
				0));
	}

	{
		Gl::Enum status = gl.checkFramebufferStatus(Gl::FRAMEBUFFER);
		if (status != Gl::FRAMEBUFFER_COMPLETE) {
			throw std::runtime_error(
					Aux::printf(
						"incomplete frame buffer %u: 0x%x", 
						glId(), 
						static_cast<unsigned>(status)));
		}
	}

	setupShader2D(*m_dftShader2DPtr);
	setupShader3D(*m_dftShader3DPtr);
}

ATD::FrameBuffer::~FrameBuffer()
{
	/* If bound via 'Usage' subclass, it is already unbound automatically. 
	 * Textures get deinited automatically when all smart pointers destroyed. 
	 * No other deinit required. */
	gl.deleteFramebuffers(1, &m_frameBufferId);
}

double ATD::FrameBuffer::aspectRatio() const
{
	double aspectRatio = (m_size.x > 0 && m_size.y > 0) ?  
		(static_cast<double>(m_size.x) / static_cast<double>(m_size.y)) : 1.;

	return aspectRatio;
}

void ATD::FrameBuffer::setShader2D(ATD::Shader2D::Ptr shader2DPtr)
{
	m_shader2DPtr = shader2DPtr;
	Shader2D &shader = m_shader2DPtr ? *m_shader2DPtr : *m_dftShader2DPtr;
	setupShader2D(shader);
}

void ATD::FrameBuffer::setShader3D(ATD::Shader3D::Ptr shader3DPtr)
{
	m_shader3DPtr = shader3DPtr;
	Shader3D &shader = m_shader3DPtr ? *m_shader3DPtr : *m_dftShader3DPtr;
	setupShader3D(shader);
}

void ATD::FrameBuffer::setProjection2D(const ATD::Projection2D &projection2D)
{
	m_projection2D = projection2D;
	Shader2D &shader = m_shader2DPtr ? *m_shader2DPtr : *m_dftShader2DPtr;
	updateProjectionOnShader2D(shader);
}

void ATD::FrameBuffer::setProjection3D(const ATD::Projection3D &projection3D)
{
	m_projection3D = projection3D;
	Shader3D &shader = m_shader3DPtr ? *m_shader3DPtr : *m_dftShader3DPtr;
	updateProjectionOnShader3D(shader);
}

void ATD::FrameBuffer::clear()
{
	Usage useFB(*this);
	gl.clear(Gl::COLOR_BUFFER_BIT | Gl::DEPTH_BUFFER_BIT);
}

void ATD::FrameBuffer::draw(const Drawable &drawable)
{ drawable.drawSelf(*this); }

void ATD::FrameBuffer::draw(const ATD::VertexBuffer2D &vertices2D, 
		const ATD::Transform2D &transform)
{
	Shader2D &shader2D = m_shader2DPtr ? *m_shader2DPtr : *m_dftShader2DPtr;
	shader2D.setUniform("unfTransform", 
			(m_coords2DOffset.matrix() *
				m_coords2DScale.matrix() * 
				transform.matrix()));

	/* Texture::Usage is expected to be set before. */
	if (shader2D.isCanvasRequired() && m_colorTexturePtr) {
		Texture::Usage useCanvas(*m_colorTexturePtr, Texture::TEX_7);

		Usage useFBuffer(*this);
		Shader::Usage useShader(shader2D);
		gl.viewport(0, 0, m_size.x, m_size.y);
		/* https://stackoverflow.com/questions/33718237/\
		 * do-you-have-to-call-glviewport-every-time-you-bind-\
		 * a-frame-buffer-with-a-differe */

		vertices2D.drawSelfInternal(shader2D.getAttrIndices());
	} else {
		Usage useFBuffer(*this);
		Shader::Usage useShader(shader2D);
		gl.viewport(0, 0, m_size.x, m_size.y);
		/* https://stackoverflow.com/questions/33718237/\
		 * do-you-have-to-call-glviewport-every-time-you-bind-\
		 * a-frame-buffer-with-a-differe */

		vertices2D.drawSelfInternal(shader2D.getAttrIndices());
	}
}

void ATD::FrameBuffer::draw(const ATD::VertexBuffer3D &vertices3D, 
		const ATD::Transform3D &transform)
{

	Shader3D &shader3D = m_shader3DPtr ? *m_shader3DPtr : *m_dftShader3DPtr;
	shader3D.setUniform("unfTransform", transform.matrix());

	/* Texture::Usage is expected to be set before. */
	if (shader3D.isCanvasRequired() && m_colorTexturePtr) {
		Texture::Usage useCanvas(*m_colorTexturePtr, Texture::TEX_7);

		Usage useFBuffer(*this);
		Shader::Usage useShader(shader3D);
		gl.viewport(0, 0, m_size.x, m_size.y);

		vertices3D.drawSelfInternal(shader3D.getAttrIndices());
	} else {
		Usage useFBuffer(*this);
		Shader::Usage useShader(shader3D);
		gl.viewport(0, 0, m_size.x, m_size.y);

		vertices3D.drawSelfInternal(shader3D.getAttrIndices());
	}
}

void ATD::FrameBuffer::setupShader2D(Shader2D &shader) const
{
	updateCanvasUniformsOnShader(static_cast<Shader &>(shader));
	updateProjectionOnShader2D(shader);
}

void ATD::FrameBuffer::setupShader3D(Shader3D &shader) const
{
	updateCanvasUniformsOnShader(static_cast<Shader &>(shader));
	updateProjectionOnShader3D(shader);
}

void ATD::FrameBuffer::updateCanvasUniformsOnShader(Shader &shader) const
{
	if (shader.isCanvasRequired()) {
		shader.setUniform("unfCanvasSize", 
				static_cast<Vector2F>(m_size));

		shader.setUniformSampler2DUnit("unfCanvasTexture", 
				Texture::TEX_7);
	}
}

void ATD::FrameBuffer::updateProjectionOnShader2D(Shader2D &shader) const
{
	shader.setUniform("unfProject", m_projection2D.matrix());
}

void ATD::FrameBuffer::updateProjectionOnShader3D(Shader3D &shader) const
{
	shader.setUniform("unfProject", _yFlip3D() * m_projection3D.matrix());
}

void ATD::FrameBuffer::drawPlainAligned(const ATD::Texture &texSrc, 
		ATD::Texture &texDst, 
		const ATD::Align &alignX, 
		const ATD::Align &alignY)
{
	long deltaX = texDst.size().x - texSrc.size().x;
	long deltaY = texDst.size().y - texDst.size().y;

	Vector2L offset;

	offset.x = (alignX == Align::LOWER) ? 0 : 
		(alignX == Align::CENTER) ? deltaX / 2 : 
		deltaX;

	offset.y = (alignY == Align::LOWER) ? 0 : 
		(alignY == Align::CENTER) ? deltaY / 2 : 
		deltaY;

	/* Init vertices. */
	VertexBuffer2D vertices(RectL(texSrc.size()), texSrc.size());

	/* Init transform. */
	Transform2D transform;
	transform.setOffset(static_cast<ATD::Vector2D>(offset));	

	/* Create temporary frame buffer. */
	Gl::Uint frameBufferTmpId = 0;
	GL_CHECK("", gl.genFramebuffers(1, &frameBufferTmpId));

	/* Remember the previously bound frame buffer and use the 
	 * temporary one. */
	Gl::Uint frameBufferPrevId = 0;
	gl.getIntegerv(Gl::FRAMEBUFFER_BINDING, 
			reinterpret_cast<Gl::Int *>(&frameBufferPrevId));

	GL_CHECK("", gl.bindFramebuffer(Gl::FRAMEBUFFER, frameBufferTmpId));

	/* Attach dst texture to the frame buffer as color. */
	GL_CHECK("", gl.framebufferTexture2D(Gl::FRAMEBUFFER, 
			Gl::COLOR_ATTACHMENT0, /* attachment */
			Gl::TEXTURE_2D, /* textarget */
			texDst.glId(), 
			0));

	/* Check temporary frame buffer. */
	{
		Gl::Enum status = gl.checkFramebufferStatus(Gl::FRAMEBUFFER);
		if (status != Gl::FRAMEBUFFER_COMPLETE) {
			/* Restore previous frame buffer. */
			GL_CHECK("", gl.bindFramebuffer(Gl::FRAMEBUFFER, 
						frameBufferPrevId));

			/* Delete temporary frame buffer. */
			gl.deleteFramebuffers(1, &frameBufferTmpId);

			throw std::runtime_error(
					Aux::printf(
						"incomplete frame buffer %u: 0x%x", 
						glId(), 
						static_cast<unsigned>(status)));
		}
	}

	/* Do drawing with basic 2D shader. */
	{
		std::lock_guard<std::recursive_mutex> lock(_basicShader2DMtx);

		if (!_basicShader2DPtr) {
			_basicShader2DPtr = 
				Shader2D::Ptr(
						new Shader2D(
							Shader2D::PLAIN_VERTEX_SOURCE, 
							Shader2D::PLAIN_FRAGMENT_SOURCE));
		}

		Transform2D tmpCoords2DScale = _coords2DScaleTransform(texDst.size());
		Transform2D tmpCoords2DOffset = _coords2DOffsetTransform();

		_basicShader2DPtr->setUniform("unfProject", 
				Projection2D().matrix());

		_basicShader2DPtr->setUniform("unfTransform", 
				(tmpCoords2DOffset.matrix() *
					tmpCoords2DScale.matrix() * 
					transform.matrix()));

		Texture::Usage useTexture(texSrc);

		Shader::Usage useShader(*_basicShader2DPtr);

		GL_CHECK("", vertices.drawSelfInternal(
					_basicShader2DPtr->getAttrIndices()));
	}

	/* Restore previous frame buffer. */
	GL_CHECK("", gl.bindFramebuffer(Gl::FRAMEBUFFER, frameBufferPrevId));

	/* Delete temporary frame buffer. */
	gl.deleteFramebuffers(1, &frameBufferTmpId);
}


