/**
@file     
@brief    Math functions templates.
@details  License: GPL v3.
@author   ArthurTheDigital (arthurthedigital@gmail.com)
@since    $Id: $
*/

#pragma once

#include <stdexcept>


namespace ATD {

/**
 * @brief Calculate absolute value
 * @param a - ...
 * @return ... */
template<typename T>
inline T Abs(const T &a)
{ return (a < T(0)) ? -a : a; }

/**
 * @brief Root template for real values (aka float, double)
 * @param a - input value
 * @return K-degree root from a
 *
 * Newton's method implementation for finding root of the Kth degree. */
template<typename T, unsigned K>
T Root(const T &a)
{
	if (a == T(0.)) {
		throw std::runtime_error("Root from zero");
	} else if (a < T(0.)) {
		throw std::runtime_error("Root from negative");
	}

	if (K == 0) {
		throw std::runtime_error("Root of the 0 degree");
	} else if (K == 1) {
		return a;
	}

	const T precision(1.e-6);
	const T x0((a > T(1.)) ? T(1.) + (a - 1.) / static_cast<T>(K) : T(1.));

	T xPrev(0.);
	T xCurr(x0);
	for (; !(Abs(xPrev - xCurr) < xCurr * precision);) {
		T x_K(0.);
		T x_K_minus_1(1.);
		for (unsigned k = 1; k < K; k++) { x_K_minus_1 *= xCurr; }
		x_K = x_K_minus_1 * xCurr;

		xPrev = xCurr;
		xCurr = (a + x_K * static_cast<T>(K - 1)) / (static_cast<T>(K) * x_K_minus_1);
	}
	return xCurr;
}

} /* namespace ATD */


