/**
 * @file      
 * @brief     Wrap around OpenGL vertex buffer (2D vertices).
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Rectangle.hpp>
#include <ATD/Graphics/Gl.hpp>
#include <ATD/Graphics/Vertex2D.hpp>

#include <memory>
#include <vector>


namespace ATD {

/**
 * @brief ...
 * @class ... */
class VertexBuffer2D
{
public:
	/**
	 * @brief ...
	 * @class ... */
	class Usage
	{
	public:
		/**
		 * @brief ...
		 * @param buffer - ... */
		Usage(const VertexBuffer2D &buffer);

		/**
		 * @brief ... */
		~Usage();

	private:
		Gl::Uint m_prevBuffer;
		bool m_activated;
	};

	typedef std::shared_ptr<VertexBuffer2D> Ptr;
	typedef std::shared_ptr<const VertexBuffer2D> CPtr;

	/**
	 * @brief ... */
	enum Primitive {
		TRIANGLES, 
		TRIANGLE_STRIP, 
		TRIANGLE_FAN
	};

	/**
	 * @brief describes attributes to pass while drawing. */
	struct AttrIndices
	{
		Gl::Uint positionIndex = 0;
		Gl::Uint texCoordsIndex = 0;
		Gl::Uint colorIndex = 0;

		bool texCoordsAreRequired = false;
		bool colorIsRequired = false;
	};


	/**
	 * @brief ...
	 *
	 * Default vertices - textured square, taking whole texture and filling 
	 * whole FrameBuffer */
	VertexBuffer2D();

	VertexBuffer2D(const RectL &textureBounds, 
			const Vector2S &textureSize, 
			const Pixel &color = ATD::Pixel(0xFF, 0xFF, 0xFF));

	/**
	 * @brief ...
	 * @param vertices    - ...
	 * @param textureSize - ...
	 * @param primitive   - ... */
	VertexBuffer2D(const std::vector<Vertex2D> &vertices, 
			const Vector2S &textureSize, 
			const Primitive &primitive = TRIANGLES);

	/**
	 * @brief ...
	 * @param glVertices - ...
	 * @param primitive  - ... */
	VertexBuffer2D(const std::vector<Vertex2D::GlVertex> &glVertices, 
			const Primitive &primitive = TRIANGLES);

	// TODO: Copy constructor

	/**
	 * @brief ... */
	~VertexBuffer2D();

	/**
	 * @brief ...
	 * @return ... */
	Gl::Uint GetGlId() const;

	size_t Size() const;

	/**
	 * @brief ...
	 * @return ... */
	Primitive GetPrimitive() const;

	/**
	 * @brief Draw vertices using current OpenGL texture and shader.
	 * @param attrIndices - indices of attributes to be passed
	 *
	 * Texture and shader must be set via proper ::Usage subclass before 
	 * calling this function. */
	void DrawSelfInternal(const AttrIndices &attrIndices) const;

private:
	Gl::Uint m_bufferId;
	size_t m_size;
	Primitive m_primitive;
};

} /* namespace ATD */


