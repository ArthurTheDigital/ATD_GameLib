/**
 * @file      
 * @brief     Min/Max, Abs and Clamp function templates.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once


namespace ATD {

/**
 * @brief ...
 * @param n1 - ...
 * @param n2 - ...
 * @return ... */
template<typename T>
inline T min(const T &n1, const T &n2)
{ return (n1 < n2) ? n1 : n2; }

/**
 * @brief ...
 * @param n1 - ...
 * @param n2 - ...
 * @return ... */
template<typename T>
inline T max(const T &n1, const T &n2)
{ return (n1 > n2) ? n1 : n2; }

/**
 * @brief ...
 * @param n      - value to be clamped
 * @param nLower - lower bound of n value
 * @param nUpper - upper bound of n value
 * @return clamped value */
template<typename T>
inline T clamp(const T &n, const T &nLower, const T &nUpper)
{ return max<T>(min<T>(n, nUpper), nLower); }

/**
 * @brief Calculate absolute value
 * @param a - ...
 * @return ... */
template<typename T>
inline T abs(const T &a)
{ return (a < T(0)) ? -a : a; }

} /* namespace ATD */


