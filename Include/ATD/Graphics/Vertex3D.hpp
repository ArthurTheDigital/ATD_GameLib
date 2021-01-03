/**
 * @file      
 * @brief     3D vertex for OpenGL rendering.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/MinMax.hpp>
#include <ATD/Core/Vector2.hpp>
#include <ATD/Core/Vector3.hpp>
#include <ATD/Core/Vector4.hpp>
#include <ATD/Graphics/Pixel.hpp>


namespace ATD {

/**
 * @brief ...
 * @class ... */
class Vertex3D
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
		 * @param n_normal    - ...
		 * @param n_color     - ... */
		inline GlVertex(const Vector3F &n_position, 
				const Vector2F &n_texCoords, 
				const Vector3F &n_normal, 
				const Vector4F &n_color)
			: position(n_position)
			, texCoords(n_texCoords)
			, normal(n_normal)
			, color(n_color)
		{}

		Vector3F position;
		Vector2F texCoords;
		Vector3F normal;
		Vector4F color;
	};


	/**
	 * @brief ...
	 * @param n_position  - ...
	 * @param n_texCoords - ...
	 * @param n_normal    - ...
	 * @param n_color     - ... */
	inline Vertex3D(const Vector3D &n_position = Vector3D(), 
			const Vector2L n_texCoords = Vector2L(), 
			const Vector3D &n_normal = Vector3D(0., 0., -1), 
			const Pixel &n_color = Pixel(0xFF, 0xFF, 0xFF))
		: position(n_position)
		, texCoords(n_texCoords)
		, normal(n_normal)
		, color(n_color)
	{}

	/**
	 * @brief ...
	 * @param other - ... */
	inline Vertex3D(const Vertex3D &other)
		: Vertex3D(other.position, other.texCoords, other.normal, 
				other.color)
	{}

	/**
	 * @brief ...
	 * @param textureSize - ... */
	inline GlVertex GetGlVertex(
			const Vector2S &textureSize = Vector2S()) const
	{
		/* Position is given untransformed. */
		Vector3F glPosition = static_cast<Vector3F>(position);

		/* Texture coordinates are given transformed. */
		Vector2F glTexCoords = Vector2F(
				(textureSize.x > 0 ? 
					Clamp<float>(static_cast<float>(texCoords.x) / 
						static_cast<float>(textureSize.x), 0.f, 1.f) : 
					0.f), 
				(textureSize.y > 0 ? 
					Clamp<float>(static_cast<float>(texCoords.y) / 
						static_cast<float>(textureSize.y), 0.f, 1.f) : 
					0.f));

		/* Normal is given untransformed. */
		Vector3F glNormal = static_cast<Vector3F>(normal);

		/* Color is casted to 4-float vector. */
		Vector4F glColor = color.GlColor();

		return GlVertex(glPosition, glTexCoords, glNormal, glColor);
	}


	Vector3D position; /* Position in OpenGL coords. */
	Vector2L texCoords;
	Vector3D normal;
	Pixel color;
};

} /* namespace ATD */


