/**
 * @file      
 * @brief     Wrap around OpenGl frame buffer.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Transform2D.hpp>
#include <ATD/Core/Transform3D.hpp>
#include <ATD/Core/Vector2.hpp>
#include <ATD/Graphics/Gl.hpp>
#include <ATD/Graphics/VertexBuffer2D.hpp>
#include <ATD/Graphics/VertexBuffer3D.hpp>
#include <ATD/Graphics/Shader.hpp>
#include <ATD/Graphics/Texture.hpp>

#include <memory>
#include <vector>


namespace ATD {

/**
 * @brief ... */
class FrameBuffer
{
public:
	typedef std::shared_ptr<FrameBuffer> Ptr;
	typedef std::shared_ptr<const FrameBuffer> CPtr;

	/**
	 * @brief ... */
	class Drawable
	{
	public:
		/**
		 * @brief ... */
		Drawable();

		/**
		 * @brief ... */
		virtual ~Drawable();

		/**
		 * @brief ...
		 * @param target - ... */
		virtual void DrawSelf(FrameBuffer &target) const = 0;
	};

	static const uint32_t ENABLE_COLOR = 0x00000001 << 0;
	static const uint32_t ENABLE_DEPTH = 0x00000001 << 1;

	/**
	 * @brief ... */
	class Usage
	{
	public:
		Usage(const FrameBuffer &frameBuffer);

		~Usage();

	private:
		Gl::Uint m_prevFrameBuffer;
		bool m_activated;
	};


	/**
	 * @brief ...
	 * @param size   - ...
	 * @param enable - ... */
	FrameBuffer(const Vector2S &size, 
			uint32_t enable = ENABLE_COLOR | ENABLE_DEPTH);

	/**
	 * @brief ...
	 * @param other  - ...
	 * @param size   - ...
	 * @param alignX - ...
	 * @param alignY - ...
	 *
	 * Creates a new FrameBuffer with different size, but same 
	 * shader/projection as the other. */
	FrameBuffer(const FrameBuffer &other, 
			const Vector2S &size, 
			const Align &alignX = Align::LOWER, 
			const Align &alignY = Align::LOWER);

	/**
	 * @brief ... */
	virtual ~FrameBuffer();

	uint32_t GetEnableMask() const;

	Gl::Uint GetGlId() const;

	Vector2S Size() const;

	double AspectRatio() const;

	void SetShader2D(Shader2D::Ptr shader2DPtr);

	void SetShader3D(Shader3D::Ptr shader3DPtr);

	void SetProjection2D(const Projection2D &projection2D);

	void SetProjection3D(const Projection3D &projection3D);

	void Clear();

	void Draw(const Drawable &drawable);

	void Draw(const VertexBuffer2D &vertices2D, 
			const Transform2D &transform);

	void Draw(const VertexBuffer3D &vertices3D, 
			const Transform3D &transform);

	Texture::CPtr GetColorTexture() const;

	Texture::CPtr GetDepthTexture() const;

private:
	void SetupShader2D(Shader2D &shader) const;
	void SetupShader3D(Shader3D &shader) const;

	void UpdateCanvasUniformsOnShader(Shader &shader) const;
	void UpdateProjectionOnShader2D(Shader2D &shader) const;
	void UpdateProjectionOnShader3D(Shader3D &shader) const;

	void DrawPlainAligned(const Texture &texSrc, 
			Texture &texDst, 
			const Align &alignX, 
			const Align &alignY);


	Vector2S m_size;
	Gl::Uint m_frameBufferId;
	uint32_t m_enableMask;
	Texture::Ptr m_colorTexturePtr;
	Texture::Ptr m_depthTexturePtr; /* FIXME: depth+stencil instead? */

	Shader2D::Ptr m_dftShader2DPtr;
	Shader2D::Ptr m_shader2DPtr;
	Projection2D m_projection2D;
	Transform2D m_coords2DScale;
	Transform2D m_coords2DOffset;

	Shader3D::Ptr m_dftShader3DPtr;
	Shader3D::Ptr m_shader3DPtr;
	Projection3D m_projection3D;
};

} /* namespace ATD */


