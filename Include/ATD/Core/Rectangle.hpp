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
	 * @param n_w - rectangle's width, must be >= 0
	 * @param n_h - rectangle's height, must be >= 0 */
	inline Rectangle(const T &n_x, const T &n_y, const T &n_w, const T &n_h)
		: x(n_x), y(n_y), w(n_w), h(n_h)
	{}

	/**
	 * @brief ...
	 * @param size - rectangle's size, both components, shall be >= 0 */
	inline Rectangle(const Vector2<T> &size)
		: Rectangle<T>(Vector2<T>(), size)
	{}

	/**
	 * @brief ...
	 * @param pos - ...
	 * @param size - rectangle's size, both components, shall be >= 0 */
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
			Rectangle<T> ir = including(vertexes[iV]);

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
	inline Vector2<T> pos() const
	{ return Vector2<T>(x, y); }

	/**
	 * @brief ...
	 * @return absolute size width/height of rectangle */
	inline Vector2<T> size() const
	{ return Vector2<T>(w, h); }

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
	inline std::vector<Vector2<T>> vertices() const
	{
		std::vector<Vector2<T>> _vertices;
		_vertices.push_back(Vector2<T>(x, y));
		_vertices.push_back(Vector2<T>(x, y + h));
		_vertices.push_back(Vector2<T>(x + w, y + h));
		_vertices.push_back(Vector2<T>(x + w, y));
		return _vertices;
	}

	/**
	 * @brief ...
	 * @param outer - ...
	 * @return ... */
	inline Rectangle<T> clamped(const Rectangle<T> &outer) const
	{
		Vector2<T> oPos = outer.pos();
		Vector2<T> oSize = outer.size();

		T cX = clamp<T>(x, oPos.x, oPos.x + oSize.x);
		T cX1 = clamp<T>(x + w, oPos.x, oPos.x + oSize.x);

		T cY = clamp<T>(y, oPos.y, oPos.y + oSize.y);
		T cY1 = clamp<T>(y + h, oPos.y, oPos.y + oSize.y);

		return Rectangle<T>(cX, cY, cX1 - cX, cY1 - cY);
	}

	/**
	 * @brief ...
	 * @param outer - ...
	 * @param alignX - ...
	 * @param alignY - ...
	 * @return ... */
	inline Rectangle<T> alignClamped(const Rectangle<T> &outer, 
			const Align &alignX, 
			const Align &alignY) const
	{
		T nW = min<T>(w, outer.w);
		T nH = min<T>(h, outer.h);

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
	inline bool contains(const Vector2<T> &point) const
	{ return ((x <= point.x) && (point.x < x + w) && 
			((y <= point.y) && (point.y < y + h))); }

	/**
	 * @brief ...
	 * @param point - point to be included
	 * @return stretched rectangle, including both previous rectangle and the 
	 *         point */
	inline Rectangle<T> including(const Vector2<T> &point) const
	{
		T iX = w < 0 ? max<T>(x, point.x) : min<T>(x, point.x);
		T iX1 = w < 0 ? min<T>(x + w, point.x) : max<T>(x + w, point.x);

		T iY = h < 0 ? max<T>(y, point.y) : min<T>(y, point.y);
		T iY1 = h < 0 ? min<T>(y + h, point.y) : max<T>(y + h, point.y);

		return Rectangle<T>(iX, iY, iX1 - iX, iY1 - iY);
	}

	/**
	 * @brief ...
	 * @return ... */
	template<typename F>
	inline operator Rectangle<F>() const
	{
		return Rectangle<F>(static_cast<F>(x), static_cast<F>(y), 
				static_cast<F>(w), static_cast<F>(h));
	}


	/* FIXME: Shall I try union? Setters/getters? */
	T x;
	T y;
	T w;
	T h;
};

/* Typedefs: */

typedef Rectangle<long> RectL;
typedef Rectangle<double> RectD;

} /* namespace ATD */


