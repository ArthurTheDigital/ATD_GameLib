/**
 * @file      
 * @brief     Pixel manipulation.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Vector4.hpp>


namespace ATD {

/**
 * @brief Pixel of image
 * @class ... */
class Pixel
{
public:
	/**
	 * @brief Default constructor */
	inline Pixel()
		: value(0xFF000000)
	{}

	/**
	 * @brief ...
	 * @param n_value - ... */
	inline Pixel(uint32_t n_value)
		: value(n_value)
	{}

	/**
	 * @brief ...
	 * @param n_r - ...
	 * @param n_g - ...
	 * @param n_b - ...
	 * @param n_a - ... */
	inline Pixel(uint8_t n_r, 
			uint8_t n_g, 
			uint8_t n_b, 
			uint8_t n_a = 0xFF)
		: r(n_r)
		, g(n_g)
		, b(n_b)
		, a(n_a) 
	{}

	/**
	 * @brief ...
	 * @param other - ... */
	inline Pixel(const Pixel &other)
		: Pixel(other.value)
	{}

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	inline bool operator==(const Pixel &other) const
	{ return value == other.value; }

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	inline bool operator!=(const Pixel &other) const
	{ return value != other.value; }

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	inline Pixel &operator=(const Pixel &other)
	{
		value = other.value;
		return *this;
	}

	/**
	 * @brief Color as vector of 4 floats in [0;1].
	 * @return ...
	 *
	 * Suitable for OpenGL. */
	inline Vector4F glColor() const
	{
		return Vector4F(static_cast<float>(r) / 255.f, 
				static_cast<float>(g) / 255.f, 
				static_cast<float>(b) / 255.f, 
				static_cast<float>(a) / 255.f);
	}

	/**
	 * @brief Color as vector of 4 floats in [0;1].
	 * @return ...
	 *
	 * Suitable for OpenGL. */
	inline Vector3F glColor3() const
	{
		return Vector3F(static_cast<float>(r) / 255.f, 
				static_cast<float>(g) / 255.f, 
				static_cast<float>(b) / 255.f);
	}


	union
	{
		uint32_t value;
		struct
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		};
	};
};

} /* namespace ATD */


