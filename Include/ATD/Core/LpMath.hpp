/**
 * @file      
 * @brief     Low precision fast math.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <stdint.h>


namespace ATD {

namespace Lp {

inline float sqrt(float x)
{
	/* Get fine starting precision by bit manipulation. */

	/* Get float as bits.
	 * According to IEEE 754:
	 *  1 |        8        |               23                |
	 *  0 | 0 0 0 0 1 0 1 0 | 1 1 0 0 0 0 0 0 0 0 0 0 0 ... 0 |
	 *  ^          ^                 ^
	 *  s         exp               mnt
	 * +/-  exponenta + 127    fractional part of mantissa 
	 *
	 * Sign bit (s) is assumed always 0 
	 * (because sqrt(negative) does not exist).
	 *
	 * x = (2 ^ (exp - 127)) * (1 + mnt / 2 ^ 23)
	 * */
	uint32_t xBits = *reinterpret_cast<uint32_t *>(&x);

	/* Log2(Sqrt(x)) = Log2(x) / 2
	 *
	 * Approximation idea:
	 * for 0 <= d <= 1
	 * Log2(1 + d) ~= d + D;   where the best D is 0.0430357
	 *
	 * Log2(x) = 
	 * Log2(2 ^ (exp - 127)) + Log2(1 + mnt / 2 ^ 23) ~= 
	 * exp - 127 + mnt / 2 ^ 23 + D
	 *
	 * xBits = exp * 2 ^ 23 + mnt ~= (Log2(x) + 127 - D) * 2 ^ 23
	 *
	 * xSqrtBits ~= 
	 * (Log2(xSqrt) + 127 - D) * 2 ^ 23 = 
	 * (Log2(x) / 2 + 127 - D) * 2 ^ 23 = 
	 * ((xBits / 2 ^ 23 - 127 + D) / 2 + 127 - D) * 2 ^ 23 = 
	 * xBits / 2 + (127 - D) * 0.5 * 2 ^ 23 = 
	 * xBits / 2 + 0x1FBD3EE7
	 * */
	uint32_t xSqrtBits = 0x1FBD3EE7 + (xBits >> 1);

	/* Now it is a good starting approximation. */
	float xSqrt = *reinterpret_cast<float *>(&xSqrtBits);

	/* Improve the approximation with Newton-Raphson method:
	 *
	 * Let y = Sqrt(x). This means, must be satisfied:
	 * f(y) = (y * y) - x = 0
	 *
	 * Iteration of Newton-Raphson:
	 * y1 = y0 - f(y0) / f'(y0) = 
	 * y0 - (y0 * y0 - x) / (2 * y0) = 
	 * (y0 * y0 + x) * 0.5 / y0 = 
	 * 0.5 * y0 + x * 0.5 / y0 */

	const float nr0_5 = 0.5f;
	const float x0_5 = x * 0.5f;

	xSqrt = nr0_5 * xSqrt + x0_5 / xSqrt; /* One step. */
	xSqrt = nr0_5 * xSqrt + x0_5 / xSqrt; /* One step. */

	return xSqrt;
}

/* @brief Get reversed square root fast, using Carmack's trick.
 * @param x - x must be > 0.f strictly!
 * @return reversed square root of x. If x was <= 0.f, the result is garbage. */
inline float rSqrt(float x)
{
	/* Get fine starting precision by bit manipulation. */

	/* Get float as bits.
	 *
	 * x = (2 ^ (exp - 127)) * (1 + mnt / 2 ^ 23)
	 * */
	uint32_t xBits = *reinterpret_cast<uint32_t *>(&x);

	/* Log2(RSqrt(x)) = - Log2(Sqrt(x)) = - Log2(x) / 2
	 *
	 * Log2(1 + d) ~= d + D
	 * Log2(x) = exp - 127 + mnt / 2 ^ 23 + D
	 * xBits = exp * 2 ^ 23 + mnt ~= (Log2(x) + 127 - D) * 2 ^ 23
	 *
	 * xRsBits ~= 
	 * (Log2(xRs) + 127 - D) * 2 ^ 23 = 
	 * (-Log2(x) / 2 + 127 - D) * 2 ^ 23 ~= 
	 * (-(xBits / 2 ^ 23 - 127 + D) / 2 + 127 - D) * 2 ^ 23 = 
	 * -xBits / 2 + (127 - D) * 1.5 * 2 ^ 23 = 
	 * -xBits / 2 + 0x5F37BCB5
	 * */
	uint32_t xRsBits = 0x5F37BCB5 - (xBits >> 1);

	/* Now it is a good starting approximation. */
	float xRs = *reinterpret_cast<float *>(&xRsBits);

	/* Improve the approximation with Newton-Raphson method:
	 *
	 * Let y = RSqrt(x). This means, must be satisfied:
	 * f(y) = 1 / (y * y) - x = 0
	 *
	 * Iteration of Newton-Raphson:
	 * y1 = y0 - f(y0) / f'(y0) = 
	 * y0 - (1 / y0^2 - x) / (-2/y0^3) = 
	 * y0 + (y0 - x * y0^2) * 0.5 = 
	 * y0 * (1.5 - x * 0.5 * y0 * y0) */

	const float nr1_5 = 1.5f;
	const float x0_5 = x * 0.5f;

	xRs = xRs * (nr1_5 - x0_5 * xRs * xRs); /* One step. */
	xRs = xRs * (nr1_5 - x0_5 * xRs * xRs); /* One step. */

	return xRs;
}

}

}


