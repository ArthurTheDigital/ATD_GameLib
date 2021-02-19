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
 * @brief ...
 * @class ... */
class FrameBuffer
{
public:
	typedef std::shared_ptr<FrameBuffer> Ptr;
	typedef std::shared_ptr<const FrameBuffer> CPtr;

	/**
	 * @brief ...
	 * @class ... */
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
		virtual void drawSelf(FrameBuffer &target) const = 0;
	};

	/**
	 * @brief ...
	 * @class ... */
	class Drawable2D : public Drawable
	{
	public:
		/**
		 * @brief ... */
		Drawable2D();

		/**
		 * @brief ... */
		virtual ~Drawable2D();

		/* Methods for handling transform: */

		/**
		 * @brief ...
		 * @param transform - ... */
		void setTransform(const Transform2D &transform);

		/**
		 * @brief ...
		 * @param scale - ... */
		void setScale(const Vector2D &scale);

		/**
		 * @brief ...
		 * @param angleFrc - ... */
		void setAngleFrc(double angleFrc);

		/**
		 * @brief ...
		 * @param offset - ... */
		void setOffset(const Vector2D &offset);

		/**
		 * @brief ...
		 * @return ... */
		inline const Transform2D &transform() const
		{ return m_transform; }

		/* virtual void DrawSelf(FrameBuffer &target) const = 0; */

	protected:
		Transform2D m_transform;
	};

	static const uint32_t BUFFER_COLOR = 0x00000001 << 0;
	static const uint32_t BUFFER_DEPTH = 0x00000001 << 1;

	/**
	 * @brief ...
	 * @class ... */
	class Usage
	{
	public:
		/**
		 * @brief ...
		 * @param frameBuffer - FrameBuffer object to use. */
		Usage(const FrameBuffer &frameBuffer);

		/**
		 * @brief ... */
		~Usage();

	private:
		Gl::Uint m_prevFrameBuffer;
		bool m_activated;
	};


	/**
	 * @brief ...
	 * @param size       - ...
	 * @param bufferMask - ... */
	FrameBuffer(const Vector2S &size, 
			uint32_t bufferMask = BUFFER_COLOR | BUFFER_DEPTH);

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

	/**
	 * @brief ...
	 * @return ... */
	inline uint32_t bufferMask() const
	{ return m_bufferMask; }

	/**
	 * @brief ...
	 * @return ... */
	inline Gl::Uint glId() const
	{ return m_frameBufferId; }

	/**
	 * @brief ...
	 * @return ... */
	inline const Vector2S &size() const
	{ return m_size; }

	/**
	 * @brief ...
	 * @return aspect ratio - width, divided by height */
	double aspectRatio() const;

	/**
	 * @brief ...
	 * @param shader2DPtr - ... */
	void setShader2D(Shader2D::Ptr shader2DPtr);

	/**
	 * @brief ...
	 * @param shader3DPtr - ... */
	void setShader3D(Shader3D::Ptr shader3DPtr);

	/**
	 * @brief ...
	 * @param projection2D - ... */
	void setProjection2D(const Projection2D &projection2D);

	/**
	 * @brief ...
	 * @param projection3D - ... */
	void setProjection3D(const Projection3D &projection3D);

	/**
	 * @brief ... */
	void clear();

	/**
	 * @brief ...
	 * @param drawable - ... */
	void draw(const Drawable &drawable);

	/**
	 * @brief ...
	 * @param vertices2D - ...
	 * @param transform  - ... */
	void draw(const VertexBuffer2D &vertices2D, 
			const Transform2D &transform);

	/**
	 * @brief ...
	 * @param vertices3D - ...
	 * @param transform  - ... */
	void draw(const VertexBuffer3D &vertices3D, 
			const Transform3D &transform);

	/**
	 * @brief ...
	 * @return ... */
	inline Texture::CPtr getColorTexture() const
	{ return static_cast<Texture::CPtr>(m_colorTexturePtr); }

	/**
	 * @brief ...
	 * @return ... */
	inline Texture::CPtr getDepthTexture() const
	{ return static_cast<Texture::CPtr>(m_depthTexturePtr); }

private:
	/**
	 * @brief ...
	 * @param shader - ... */
	void setupShader2D(Shader2D &shader) const;

	/**
	 * @brief ...
	 * @param shader - ... */
	void setupShader3D(Shader3D &shader) const;

	/**
	 * @brief Set canvas uniforms in shader if required.
	 * @param shader - ... */
	void updateCanvasUniformsOnShader(Shader &shader) const;

	/**
	 * @brief ...
	 * @param shader - ... */
	void updateProjectionOnShader2D(Shader2D &shader) const;

	/**
	 * @brief ...
	 * @param shader - ... */
	void updateProjectionOnShader3D(Shader3D &shader) const;

	/**
	 * @brief Copy contents from texSrc to texDst with align.
	 * @param texSrc - ...
	 * @param texDst - ...
	 * @param alignX - ...
	 * @param alignY - ... */
	void drawPlainAligned(const Texture &texSrc, 
			Texture &texDst, 
			const Align &alignX, 
			const Align &alignY);


	Vector2S m_size;
	Gl::Uint m_frameBufferId;
	uint32_t m_bufferMask;
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


