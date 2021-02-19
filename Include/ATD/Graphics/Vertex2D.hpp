/**
 * @file      
 * @brief     2D vertex for OpenGL rendering.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/MinMax.hpp>
#include <ATD/Core/Vector2.hpp>
#include <ATD/Core/Vector4.hpp>
#include <ATD/Graphics/Pixel.hpp>


namespace ATD {

/**
 * @brief ...
 * @class ... */
class Vertex2D
{
public:
	/**
	 * @brief ... */
	class GlVertex
	{
	public:
		/**
		 * @brief ...
		 * @param n_position  - ...
		 * @param n_texCoords - ...
		 * @param n_color     - ... */
		inline GlVertex(const Vector2F &n_position, 
				const Vector2F &n_texCoords, 
				const Vector4F &n_color)
			: position(n_position)
			, texCoords(n_texCoords)
			, color(n_color)
		{}

		Vector2F position;
		Vector2F texCoords;
		Vector4F color;
	};

	/**
	 * @brief ...
	 * @param n_position  - ...
	 * @param n_texCoords - ...
	 * @param n_color     - ... */
	inline Vertex2D(const Vector2D &n_position = Vector2D(), 
			const Vector2L &n_texCoords = Vector2L(), 
			const Pixel &n_color = Pixel(0xFF, 0xFF, 0xFF))
		: position(n_position)
		, texCoords(n_texCoords)
		, color(n_color)
	{}

	/**
	 * @brief ...
	 * @param other - ... */
	inline Vertex2D(const Vertex2D &other)
		: Vertex2D(other.position, other.texCoords, other.color)
	{}

	/**
	 * @brief ...
	 * @param textureSize - ... */
	inline GlVertex glVertex(
			const Vector2S &textureSize = Vector2S()) const
	{
		/* Position is given untransformed, in pixel coordinates. The 
		 * transformation to OpenGL coordinates shall happen inside drawing 
		 * Shader and the parameters of transformation shall be provided to 
		 * shader by FrameBuffer while drawing! */
		Vector2F glPosition = static_cast<Vector2F>(position);

		/* Texture coordinates are given transformed. */
		Vector2F glTexCoords = Vector2F(
				(textureSize.x > 0 ? 
					clamp<float>(static_cast<float>(texCoords.x) / 
						static_cast<float>(textureSize.x), 0.f, 1.f) : 
					0.f), 
				(textureSize.y > 0 ? 
					clamp<float>(static_cast<float>(texCoords.y) / 
						static_cast<float>(textureSize.y), 0.f, 1.f) : 
					0.f));

		/* Color is casted to 4-float vector. */
		Vector4F glColor = color.glColor();

		return GlVertex(glPosition, glTexCoords, glColor);
	}


	Vector2D position; /* Position in pixels. */
	Vector2L texCoords;
	Pixel color;
};

} /* namespace ATD */


