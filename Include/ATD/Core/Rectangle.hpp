/**
 * @file      
 * @brief     Rectangle template and types.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/MinMax.hpp>
#include <ATD/Core/Vector2.hpp>

#include <array>
#include <vector>


namespace ATD {

/**
 * @brief ... */
enum class Align {
	LOWER, 
	CENTER, 
	UPPER
};

/**
 * @brief ...
 * @class ... */
template<typename T>
class Rectangle
{
public:
	/**
	 * @brief ... */
	inline Rectangle()
		: Rectangle<T>(0, 0, 0, 0)
	{}

	/**
	 * @brief ...
	 * @param n_x - ...
	 * @param n_y - ...
	 * @param n_w - ...
	 * @param n_h - ... */
	inline Rectangle(const T &n_x, const T &n_y, const T &n_w, const T &n_h)
		: x(n_x), y(n_y), w(n_w), h(n_h)
	{}

	/**
	 * @brief ...
	 * @param size - ... */
	inline Rectangle(const Vector2<T> &size)
		: Rectangle<T>(Vector2<T>(), size)
	{}

	/**
	 * @brief ...
	 * @param pos - ...
	 * @param size - ... */
	inline Rectangle(const Vector2<T> &pos, const Vector2<T> &size)
		: x(pos.x), y(pos.y), w(size.x), h(size.y)
	{}

	/**
	 * @brief ...
	 * @param r - ... */
	inline Rectangle(const Rectangle<T> &r)
		: x(r.x), y(r.y), w(r.w), h(r.h)
	{}

	/**
	 * @brief ...
	 * @param vertexes - ... */
	inline Rectangle(const std::vector<Vector2<T>> &vertexes)
		: Rectangle((vertexes.size() ? vertexes.front() : Vector2<T>()), Vector2<T>())
	{
		for (size_t iV = 1; iV < vertexes.size(); iV++) {
			Rectangle<T> ir = Including(vertexes[iV]);

			x = ir.x;
			y = ir.y;
			w = ir.w;
			h = ir.h;
		}
	}

	/**
	 * @brief ...
	 * @return position of top left corner (may not be same as 
	 *         Vector2<T>(x, y)) */
	inline Vector2<T> Pos() const
	{ return Vector2<T>(Min<T>(x, x + w), Min<T>(y, y + h)); }

	/**
	 * @brief ...
	 * @return absolute size width/height of rectangle */
	inline Vector2<T> Size() const
	{ return Vector2<T>(Abs<T>(w), Abs<T>(h)); }

	/**
	 * @brief ...
	 * @return list of vertexes of rectangle, arranged counterclockwise
	 *
	 * +-----------------> 
	 * |   0        3   (X)
	 * |                   
	 * |                   
	 * |   1        2      
	 * V(Y)                
	 *                     */
	inline std::vector<Vector2<T>> Vertices() const
	{
		std::vector<Vector2<T>> vertexes;
		vertexes.push_back(Vector2<T>(x, y));
		vertexes.push_back(Vector2<T>(x, y + h));
		vertexes.push_back(Vector2<T>(x + w, y + h));
		vertexes.push_back(Vector2<T>(x + w, y));
		return vertexes;
	}

	/**
	 * @brief ...
	 * @param outer - ...
	 * @return ... */
	inline Rectangle<T> Clamped(const Rectangle<T> &outer) const
	{
		Vector2<T> oPos = outer.Pos();
		Vector2<T> oSize = outer.Size();

		T cX = Clamp<T>(x, oPos.x, oPos.x + oSize.x);
		T cX1 = Clamp<T>(x + w, oPos.x, oPos.x + oSize.x);

		T cY = Clamp<T>(y, oPos.y, oPos.y + oSize.y);
		T cY1 = Clamp<T>(y + h, oPos.y, oPos.y + oSize.y);

		return Rectangle<T>(cX, cY, cX1 - cX, cY1 - cY);
	}

	/**
	 * @brief ...
	 * @param outer - ...
	 * @param alignX - ...
	 * @param alignY - ...
	 * @return ... */
	inline Rectangle<T> AlignClamped(const Rectangle<T> &outer, 
			const Align &alignX, 
			const Align &alignY) const
	{
		T nW = Min<T>(w, outer.w);
		T nH = Min<T>(h, outer.h);

		T nX = outer.x + (
				(alignX == Align::LOWER) ? 0 : 
				(alignX == Align::CENTER) ? (outer.w - nW) / 2 : 
				outer.w - nW
				);

		T nY = outer.y + (
				(alignY == Align::LOWER) ? 0 : 
				(alignY == Align::CENTER) ? (outer.h - nH) / 2 : 
				outer.h - nH
				);

		return Rectangle<T>(nX, nY, nW, nH);
	}

	/**
	 * @brief ...
	 * @param point - ...
	 * @return true if point is inside the rectangle */
	inline bool Contains(const Vector2<T> &point) const
	{ return ((Min<T>(x, x + w) <= point.x) && (point.x < Max<T>(x, x + w)) && 
			((Min<T>(y, y + h) <= point.y) && (point.y < Max<T>(y, y + h)))); }

	/**
	 * @brief ...
	 * @param point - point to be included
	 * @return stretched rectangle, including both previous rectangle and the 
	 *         point */
	inline Rectangle<T> Including(const Vector2<T> &point) const
	{
		T iX = w < 0 ? Max<T>(x, point.x) : Min<T>(x, point.x);
		T iX1 = w < 0 ? Min<T>(x + w, point.x) : Max<T>(x + w, point.x);

		T iY = h < 0 ? Max<T>(y, point.y) : Min<T>(y, point.y);
		T iY1 = h < 0 ? Min<T>(y + h, point.y) : Max<T>(y + h, point.y);

		return Rectangle<T>(iX, iY, iX1 - iX, iY1 - iY);
	}

	/**
	 * @brief ...
	 * @return ... */
	template<typename F>
	inline operator Rectangle<F>() const
	{ return Rectangle<F>(static_cast<F>(x), static_cast<F>(y), static_cast<F>(w), static_cast<F>(h)); }


	T x;
	T y;
	T w;
	T h;
};

/* Typedefs: */

typedef Rectangle<long> RectL;
typedef Rectangle<double> RectD;

} /* namespace ATD */


