/**
 * @file      
 * @brief     Vector3 template and types.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Vector2.hpp>


namespace ATD {

/**
 * @brief 3D vector template
 * @class ...
 *
 * T shall have constructor, taking 0 and support arithmetics. */
template<typename T>
class Vector3
{
public:
	/**
	 * @brief ... */
	inline Vector3()
		: Vector3(0, 0, 0)
	{}

	/**
	 * @brief ...
	 * @param n_x - ...
	 * @param n_y - ...
	 * @param n_z - ... */
	inline Vector3(T n_x, T n_y, T n_z)
		: x(n_x), y(n_y), z(n_z)
	{}

	/* Stolen from GLSL for comfortable usage: */

	/**
	 * @brief ...
	 * @param n_xy - ...
	 * @param n_z  - ... */
	inline Vector3(const Vector2<T> &n_xy, T n_z)
		: Vector3(n_xy.x, n_xy.y, n_z)
	{}

	/**
	 * @brief ...
	 * @param n_x  - ...
	 * @param n_yz - ... */
	inline Vector3(T n_x, const Vector2<T> &n_yz)
		: Vector3(n_x, n_yz.x, n_yz.y)
	{}

	/* === */

	/**
	 * @brief ...
	 * @param other - ... */
	inline Vector3(const Vector3<T> &other)
		: x(other.x), y(other.y), z(other.z)
	{}

	/* Comparators: */

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	bool operator==(const Vector3<T> &v) const
	{ return ((x == v.x) && (y == v.y) && (z == v.z)); }

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	bool operator!=(const Vector3<T> &v) const
	{ return ((x != v.x) || (y != v.y) || (z != v.z)); }

	/* Vector math: */

	/**
	 * @brief ...
	 * @param count - the number of 90deg clockwise rotations around X axis
	 * @return vector, rotated count times x 90 degrees clockwise around X
	 *
	 * Let rotation be clockwise around axis if it is clockwise to the viewer, 
	 * looking in the same direction as the axis. */
	inline Vector3<T> RotatedCw90X(unsigned count = 1) const
	{
		Vector3<T> r(*this);
		for (unsigned i = 0; i < count % 4; i++) {
			T bY = -r.z; T bZ = r.y;
			r.y = bY; r.z = bZ;
		}
		return r;
	}

	/**
	 * @brief ...
	 * @param count - the number of 90deg clockwise rotations around Y axis
	 * @return vector, rotated count times x 90 degrees clockwise around Y */
	inline Vector3<T> RotatedCw90Y(unsigned count = 1) const
	{
		Vector3<T> r(*this);
		for (unsigned i = 0; i < count % 4; i++) {
			T bZ = -r.x; T bX = r.z;
			r.x = bX; r.z = bZ;
		}
		return r;
	}

	/**
	 * @brief ...
	 * @param count - the number of 90deg clockwise rotations around Z axis
	 * @return vector, rotated count times x 90 degrees clockwise around Z */
	inline Vector3<T> RotatedCw90Z(unsigned count = 1) const
	{
		Vector3<T> r(*this);
		for (unsigned i = 0; i < count % 4; i++) {
			T bX = -r.y; T bY = r.x;
			r.x = bX; r.y = bY;
		}
		return r;
	}

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	inline T Dot(const Vector3<T> &v) const
	{ return (x * v.x + y * v.y + z * v.z); }

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	inline Vector3<T> Cross(const Vector3<T> &v) const
	{
		return Vector3<T>(y * v.z - z * v.y, 
				z * v.x - x * v.z, 
				x * v.y - y * v.x);
	}

	/* Constant arithmetics: */

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	inline Vector3<T> operator+ (const Vector3<T> &v) const
	{ return Vector3<T>(x + v.x, y + v.y, z + v.z); }

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	inline Vector3<T> operator- (const Vector3<T> &v) const
	{ return Vector3<T>(x - v.x, y - v.y, z - v.z); }

	/**
	 * @brief ...
	 * @param a - ...
	 * @return ... */
	inline Vector3<T> operator* (const T &a) const
	{ return Vector3<T>(x * a, y * a, z * a); }

	/**
	 * @brief ...
	 * @param a - ...
	 * @return ... */
	inline Vector3<T> operator/ (const T &a) const
	{
		if (a == T(0)) { throw std::runtime_error("Division by zero"); }
		return Vector3<T>(x / a, y / a, z / a);
	}

	/* Modifying arithmetics: */

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	inline Vector3<T> &operator+= (const Vector3<T> &v)
	{ x += v.x; y += v.y; z += v.z; return *this; }

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	inline Vector3<T> &operator-= (const Vector3<T> &v)
	{ x -= v.x; y -= v.y; z -= v.z; return *this; }

	/**
	 * @brief ...
	 * @param a - ...
	 * @return ... */
	inline Vector3<T> &operator*= (const T &a)
	{ x *= a; y *= a; z *= a; return *this; }

	/**
	 * @brief ...
	 * @param a - ...
	 * @return ... */
	inline Vector3<T> &operator/= (const T &a)
	{
		if (a == T(0)) { throw std::runtime_error("Division by zero"); }
		x /= a; y /= a; z /= a; return *this;
	}

	/* === */

	/**
	 * @brief Cast to another vector3 */
	template<typename F>
	inline operator Vector3<F>() const
	{ return Vector3<F>(static_cast<F>(x), static_cast<F>(y), static_cast<F>(z)); }


	T x;
	T y;
	T z;
};

/**
 * @brief 3D vector template for real values (aka float, double)
 * @class ... */
template<typename T>
class Vector3Real : public Vector3<T>
{
public:
	/**
	 * @brief ... */
	inline Vector3Real()
		: Vector3<T>()
	{}

	/**
	 * @brief ...
	 * @param n_x - ...
	 * @param n_y - ...
	 * @param n_z - ... */
	inline Vector3Real(T n_x, T n_y, T n_z)
		: Vector3<T>(n_x, n_y, n_z)
	{}

	/* Stolen from GLSL for comfortable usage: */

	/**
	 * @brief ...
	 * @param n_x - ...
	 * @param n_yz - ... */
	inline Vector3Real(T n_x, const Vector2<T> &n_yz)
		: Vector3<T>(n_x, n_yz)
	{}

	/**
	 * @brief ...
	 * @param n_xy - ...
	 * @param n_z - ... */
	inline Vector3Real(const Vector2<T> &n_xy, T n_z)
		: Vector3<T>(n_xy, n_z)
	{}

	/* === */

	/**
	 * @brief ...
	 * @param other - ... */
	inline Vector3Real(const Vector3Real<T> &other)
		: Vector3<T>(other.x, other.y, other.z)
	{}

	/**
	 * @brief ...
	 * @param other - ... */
	inline Vector3Real(const Vector3<T> &other)
		: Vector3<T>(other.x, other.y, other.z)
	{}

	/**
	 * @brief ...
	 * @return ... */
	inline T LengthSquare() const
	{
		return (Vector3<T>::x * Vector3<T>::x + 
				Vector3<T>::y * Vector3<T>::y + 
				Vector3<T>::z * Vector3<T>::z);
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
		return Vector3Real<T>(
				Vector3<T>(Vector3<T>::x, Vector3<T>::y, Vector3<T>::z) / n);
	}

	/* NOTE:
	 * I've decided not to implement Rotate() method. Instead rotation will 
	 * be made via matrices. */
};

/* Typedefs: */

typedef Vector3<unsigned> Vector3U;
typedef Vector3<size_t> Vector3S;
typedef Vector3<int> Vector3I;
typedef Vector3<long> Vector3L;
typedef Vector3Real<float> Vector3F;
typedef Vector3Real<double> Vector3D;

} /* namespace ATD */


