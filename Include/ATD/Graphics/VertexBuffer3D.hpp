/**
 * @file      
 * @brief     Wrap around OpenGL vertex buffer (3D vertices).
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Graphics/Gl.hpp>
#include <ATD/Graphics/Vertex3D.hpp>

#include <memory>
#include <vector>


namespace ATD {

/**
 * @brief ... */
class VertexBuffer3D
{
public:
	/**
	 * @brief ... */
	class Usage
	{
	public:
		/**
		 * @brief ...
		 * @param buffer - ... */
		Usage(const VertexBuffer3D &buffer);

		/**
		 * @brief ... */
		~Usage();

	private:
		Gl::Uint m_prevBuffer;
		bool m_activated;
	};

	typedef std::shared_ptr<VertexBuffer3D> Ptr;
	typedef std::shared_ptr<const VertexBuffer3D> CPtr;

	/**
	 * @brief ... */
	enum Primitive {
		TRIANGLES, 
		TRIANGLE_STRIP, 
		TRIANGLE_FAN
	};

	/**
	 * @brief describes attributes to pass while drawing */
	struct AttrIndices
	{
		Gl::Uint positionIndex = 0;
		Gl::Uint texCoordsIndex = 0;
		Gl::Uint normalIndex = 0;
		Gl::Uint colorIndex = 0;

		bool texCoordsAreRequired = false;
		bool normalIsRequired = false;
		bool colorIsRequired = false;
	};


	/**
	 * @brief ...
	 *
	 * Default vertices - textured cube with the same texture on each side, 
	 * using the whole given texture. */
	VertexBuffer3D();

	/**
	 * @brief ...
	 * @param vertices    - ...
	 * @param textureSize - ...
	 * @param primitive   - ... */
	VertexBuffer3D(const std::vector<Vertex3D> &vertices, 
			const Vector2S &textureSize, 
			const Primitive &primitive = TRIANGLES);

	/**
	 * @brief ...
	 * @param glVertices - ...
	 * @param primitive  - ... */
	VertexBuffer3D(const std::vector<Vertex3D::GlVertex> &glVertices, 
			const Primitive &primitive = TRIANGLES);

	/**
	 * @brief ... */
	~VertexBuffer3D();

	/**
	 * @brief ...
	 * @return ... */
	Gl::Uint GetGlId() const;

	/**
	 * @brief ...
	 * @return ... */
	size_t Size() const;

	/**
	 * @brief ...
	 * @return ... */
	Primitive GetPrimitive() const;

	/**
	 * @brief Draw vertices using current OpenGL texture and shader.
	 * @param attrIndices - ...
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


