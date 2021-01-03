/**
 * @file      
 * @brief     Vector4 template and types.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Vector2.hpp>
#include <ATD/Core/Vector3.hpp>


namespace ATD {

/**
 * @brief 4D vector template
 * @class ...
 *
 * T shall have constructor, taking 0 and support arithmetics. */
template<typename T>
class Vector4
{
public:
	/**
	 * @brief ... */
	inline Vector4()
		: Vector4(0, 0, 0, 0)
	{}

	/**
	 * @brief ...
	 * @param n_x - ...
	 * @param n_y - ...
	 * @param n_z - ...
	 * @param n_w - ... */
	inline Vector4(T n_x, T n_y, T n_z, T n_w)
		: x(n_x), y(n_y), z(n_z), w(n_w)
	{}

	/* Stolen from GLSL for comfortable usage: */

	/**
	 * @brief ...
	 * @param n_xy - ...
	 * @param n_z  - ...
	 * @param n_w  - ... */
	inline Vector4(const Vector2<T> &n_xy, T n_z, T n_w)
		: Vector4(n_xy.x, n_xy.y, n_z, n_w)
	{}

	/**
	 * @brief ...
	 * @param n_x  - ...
	 * @param n_yz - ...
	 * @param n_w  - ... */
	inline Vector4(T n_x, const Vector2<T> &n_yz, T n_w)
		: Vector4(n_x, n_yz.x, n_yz.y, n_w)
	{}

	/**
	 * @brief ...
	 * @param n_x  - ...
	 * @param n_y  - ...
	 * @param n_zw - ... */
	inline Vector4(T n_x, T n_y, const Vector2<T> &n_zw)
		: Vector4(n_x, n_y, n_zw.x, n_zw.y)
	{}

	/**
	 * @brief ...
	 * @param n_xy - ...
	 * @param n_zw - ... */
	inline Vector4(const Vector2<T> &n_xy, const Vector2<T> &n_zw)
		: Vector4(n_xy.x, n_xy.y, n_zw.x, n_zw.y)
	{}

	/**
	 * @brief ...
	 * @param n_xyz - ...
	 * @param n_w   - ... */
	inline Vector4(const Vector3<T> &n_xyz, T n_w)
		: Vector4(n_xyz.x, n_xyz.y, n_xyz.z, n_w)
	{}

	/**
	 * @brief ...
	 * @param n_x   - ...
	 * @param n_yzw - ... */
	inline Vector4(T n_x, const Vector3<T> &n_yzw)
		: Vector4(n_x, n_yzw.x, n_yzw.y, n_yzw.z)
	{}

	/* === */

	/**
	 * @brief ...
	 * @param other - ... */
	inline Vector4(const Vector4<T> &other)
		: x(other.x), y(other.y), z(other.z), w(other.w)
	{}

	/* Comparators: */

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	bool operator==(const Vector4<T> &v) const
	{ return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w)); }

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	bool operator!=(const Vector4<T> &v) const
	{ return ((x != v.x) || (y != v.y) || (z != v.z) || (w != v.w)); }

	/* Vector math: */

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	inline T Dot(const Vector4<T> &v) const
	{ return (x * v.x + y * v.y + z * v.z + w * v.w); }

	/* Constant arithmetics: */

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	inline Vector4<T> operator+ (const Vector4<T> &v) const
	{ return Vector4<T>(x + v.x, y + v.y, z + v.z, w + v.w); }

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	inline Vector4<T> operator- (const Vector4<T> &v) const
	{ return Vector4<T>(x - v.x, y - v.y, z - v.z, w - v.w); }

	/**
	 * @brief ...
	 * @param a - ...
	 * @return ... */
	inline Vector4<T> operator* (const T &a) const
	{ return Vector4<T>(x * a, y * a, z * a, w * a); }

	/**
	 * @brief ...
	 * @param a - ...
	 * @return ... */
	inline Vector4<T> operator/ (const T &a) const
	{
		if (a == T(0)) { throw std::runtime_error("Division by zero"); }
		return Vector4<T>(x / a, y / a, z / a, w / a);
	}

	/* Modifying arithmetics: */

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	inline Vector4<T> &operator+= (const Vector4<T> &v)
	{ x += v.x; y += v.y; z += v.z; w += v.w; return *this; }

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	inline Vector4<T> &operator-= (const Vector4<T> &v)
	{ x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }

	/**
	 * @brief ...
	 * @param a - ...
	 * @return ... */
	inline Vector4<T> &operator*= (const T &a)
	{ x *= a; y *= a; z *= a; w *= a; return *this; }

	/**
	 * @brief ...
	 * @param a - ...
	 * @return ... */
	inline Vector4<T> &operator/= (const T &a)
	{
		if (a == T(0)) { throw std::runtime_error("Division by zero"); }
		x /= a; y /= a; z /= a; w /= a; return *this;
	}

	/* === */

	/**
	 * @brief Cast to another vector4 */
	template<typename F>
	inline operator Vector4<F>() const
	{ return Vector4<F>(static_cast<F>(x), static_cast<F>(y), static_cast<F>(z), static_cast<F>(w)); }


	T x;
	T y;
	T z;
	T w;
};

/**
 * @brief 4D vector template for real values (aka float, double)
 * @class ... */
template<typename T>
class Vector4Real : public Vector4<T>
{
public:
	/**
	 * @brief ... */
	inline Vector4Real()
		: Vector4<T>()
	{}

	/**
	 * @brief ...
	 * @param n_x - ...
	 * @param n_y - ...
	 * @param n_z - ...
	 * @param n_w - ... */
	inline Vector4Real(T n_x, T n_y, T n_z, T n_w)
		: Vector4<T>(n_x, n_y, n_z, n_w)
	{}

	/* Stolen from GLSL for comfortable usage: */

	/**
	 * @brief ...
	 * @param n_xy - ...
	 * @param n_z  - ...
	 * @param n_w  - ... */
	inline Vector4Real(const Vector2<T> &n_xy, T n_z, T n_w)
		: Vector4<T>(n_xy, n_z, n_w)
	{}

	/**
	 * @brief ...
	 * @param n_x  - ...
	 * @param n_yz - ...
	 * @param n_w  - ... */
	inline Vector4Real(T n_x, const Vector2<T> &n_yz, T n_w)
		: Vector4<T>(n_x, n_yz, n_w)
	{}

	/**
	 * @brief ...
	 * @param n_x  - ...
	 * @param n_y  - ...
	 * @param n_zw - ... */
	inline Vector4Real(T n_x, T n_y, const Vector2<T> &n_zw)
		: Vector4<T>(n_x, n_y, n_zw)
	{}

	/**
	 * @brief ...
	 * @param n_xy - ...
	 * @param n_zw - ... */
	inline Vector4Real(const Vector2<T> &n_xy, const Vector2<T> &n_zw)
		: Vector4<T>(n_xy, n_zw)
	{}

	/**
	 * @brief ...
	 * @param n_xyz - ...
	 * @param n_w   - ... */
	inline Vector4Real(const Vector3<T> &n_xyz, T n_w)
		: Vector4<T>(n_xyz, n_w)
	{}

	/**
	 * @brief ...
	 * @param n_x   - ...
	 * @param n_yzw - ... */
	inline Vector4Real(T n_x, const Vector3<T> &n_yzw)
		: Vector4<T>(n_x, n_yzw)
	{}

	/* === */

	/**
	 * @brief ...
	 * @param other - ... */
	inline Vector4Real(const Vector4<T> &other)
		: Vector4<T>(other.x, other.y, other.z, other.w)
	{}

	/**
	 * @brief ...
	 * @return ... */
	inline T LengthSquare() const
	{
		return (Vector4<T>::x * Vector4<T>::x + 
				Vector4<T>::y * Vector4<T>::y + 
				Vector4<T>::z * Vector4<T>::z + 
				Vector4<T>::w * Vector4<T>::w);
	}

	/**
	 * @brief ...
	 * @return ... */
	inline T Length() const
	{ return Root<T, 2>(LengthSquare()); }

	/**
	 * @brief ...
	 * @return vector of the same direction, but of 1. length */
	inline Vector3Real<T> Normalized() const
	{
		T n = Length();
		return Vector4Real<T>(
				Vector4<T>(Vector4<T>::x, Vector4<T>::y, 
					Vector4<T>::z, Vector4<T>::w) / n);
	}
};

/* Typedefs: */

typedef Vector4<int> Vector4I;
typedef Vector4Real<float> Vector4F;

} /* namespace ATD */


