/**
 * @file      
 * @brief     Vector2 template and types.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Functions.hpp>

#include <math.h>

#include <stdexcept>


namespace ATD {

/**
 * @brief 2D vector template
 * @class ...
 *
 * T shall have constructor, taking 0 and support arithmetics. */
template<typename T>
class Vector2
{
public:
	/**
	 * @brief ... */
	inline Vector2()
		: Vector2(0, 0)
	{}

	/**
	 * @brief ...
	 * @param n_x - ...
	 * @param n_y - ... */
	inline Vector2(T n_x, T n_y)
		: x(n_x), y(n_y)
	{}

	/**
	 * @brief ...
	 * @param other - ... */
	inline Vector2(const Vector2<T> &other)
		: x(other.x), y(other.y)
	{}

	/* Comparators: */

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	inline bool operator==(const Vector2<T> &v) const
	{ return ((x == v.x) && (y == v.y)); }

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	inline bool operator!=(const Vector2<T> &v) const
	{ return ((x != v.x) || (y != v.y)); }

	/* Vector math: */

	/**
	 * @brief ...
	 * @param count - the number of 90deg clockwise rotations
	 * @return vector, rotated count times x 90 degrees clockwise
	 *
	 * Since Y axis is directed down, for the viewer it would be clockwise 
	 * rotation. */
	inline Vector2<T> RotatedCw90(unsigned count = 1) const
	{
		Vector2<T> r(*this);
		for (unsigned i = 0; i < count % 4; i++) {
			T bX = -r.y; T bY = r.x;
			r.x = bX; r.y = bY;
		}
		return r;
	}

	/**
	 * @brief Dot multiplication.
	 * @param v - ...
	 * @return ... */
	inline T Dot(const Vector2<T> &v) const
	{ return (x * v.x + y * v.y); }

	/**
	 * @brief Cross multiplication.
	 * @param v - ...
	 * @return ...
	 *
	 * Cross multiplication is defined for 3D vectors. The 3D result of such 
	 * multiplication lies within the straight line, perpendicular to the 
	 * area (in 2D case - say, computer screen).
	 *
	 * In 2D case, let's say, the area is a computer screen, X from left to 
	 * right, Y from up to down.
	 *
	 * Then the result, greater, than 0, shall represent the vector, 
	 * directed FROM viewer into screen, lesser, than 0, - TO viewer from 
	 * screen.
	 *                     
	 *  (axb=20)  (a=10;0) 
	 *   x--------->       
	 *   |                 
	 *   V(b=0, 2)         
	 *                     */
	inline T Cross(const Vector2<T> &v) const
	{ return (x * v.y - y * v.x); }

	/* Constant arithmetics: */

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	inline Vector2<T> operator+ (const Vector2<T> &v) const
	{ return Vector2<T>(x + v.x, y + v.y); }

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	inline Vector2<T> operator- (const Vector2<T> &v) const
	{ return Vector2<T>(x - v.x, y - v.y); }

	/**
	 * @brief ...
	 * @param a - ...
	 * @return ... */
	inline Vector2<T> operator* (const T &a) const
	{ return Vector2<T>(x * a, y * a); }

	/**
	 * @brief ...
	 * @param a - ...
	 * @return ... */
	inline Vector2<T> operator/ (const T &a) const
	{
		if (a == T(0)) { throw std::runtime_error("Division by zero"); }
		return Vector2<T>(x / a, y / a);
	}

	/* Modifying arithmetics: */

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	inline Vector2<T> &operator+= (const Vector2<T> &v)
	{ x += v.x; y += v.y; return *this; }

	/**
	 * @brief ...
	 * @param v - ...
	 * @return ... */
	inline Vector2<T> &operator-= (const Vector2<T> &v)
	{ x -= v.x; y -= v.y; return *this; }

	/**
	 * @brief ...
	 * @param a - ...
	 * @return ... */
	inline Vector2<T> &operator*= (const T &a)
	{ x *= a; y *= a; }

	/**
	 * @brief ...
	 * @param a - ...
	 * @return ... */
	inline Vector2<T> &operator/= (const T &a)
	{
		if (a == T(0)) { throw std::runtime_error("Division by zero"); }
		x /= a; y /= a; return *this;
	}

	/* === */

	/**
	 * @brief Cast to another typed vector2 */
	template<typename F>
	inline operator Vector2<F>() const
	{ return Vector2<F>(static_cast<F>(x), static_cast<F>(y)); }


	T x;
	T y;
};

/**
 * @brief 2D vector template for real values (aka float, double)
 * @class ... */
template<typename T>
class Vector2Real : public Vector2<T>
{
public:
	/**
	 * @brief ... */
	inline Vector2Real()
		: Vector2<T>()
	{}

	/**
	 * @brief ...
	 * @param n_x - ...
	 * @param n_y - ... */
	inline Vector2Real(T n_x, T n_y)
		: Vector2<T>(n_x, n_y)
	{}

	/**
	 * @brief ...
	 * @param other - ... */
	inline Vector2Real(const Vector2Real<T> &other)
		: Vector2<T>(other.x, other.y)
	{}

	/**
	 * @brief ...
	 * @param other - ... */
	inline Vector2Real(const Vector2<T> &other)
		: Vector2<T>(other.x, other.y)
	{}

	/**
	 * @brief ...
	 * @return ... */
	inline T LengthSquare() const
	{
		return (Vector2<T>::x * Vector2<T>::x + 
				Vector2<T>::y * Vector2<T>::y);
	}

	/**
	 * @brief ...
	 * @return ... */
	inline T Length() const
	{ return Root<T, 2>(LengthSquare()); }

	/**
	 * @brief ...
	 * @return vector of the same direction, but of 1. length */
	inline Vector2Real<T> Normalized() const
	{
		T n = Length();
		return Vector2Real<T>(Vector2<T>(Vector2<T>::x, Vector2<T>::y) / n);
	}

	/**
	 * @brief ...
	 * @param angleFrc - angle as a fraction of a circle clockwise
	 * @return ... */
	inline Vector2Real<T> RotatedCw(double angleFrc) const
	{
		/* M_PI defined in math.h. */
		double rads = (angleFrc - ::floor(angleFrc)) * 2. * M_PI;
		T aSin = static_cast<T>(::sin(rads));
		T aCos = static_cast<T>(::cos(rads));

		return Vector2Real<T>(Vector2<T>::x * aCos - Vector2<T>::y * aSin, 
				Vector2<T>::x * aSin + Vector2<T>::y * aCos);
	}
};

/* Typedefs: */

typedef Vector2<int8_t> Vector2I8;
typedef Vector2<uint8_t> Vector2U8;

typedef Vector2<unsigned> Vector2U;
typedef Vector2<size_t> Vector2S;
typedef Vector2<int> Vector2I;
typedef Vector2<long> Vector2L;
typedef Vector2Real<float> Vector2F;
typedef Vector2Real<double> Vector2D;

} /* namespace ATD */


