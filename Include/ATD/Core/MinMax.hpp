/**
@file     
@brief    Min/Max and Clamp functions.
@details  License: GPL v3.
@author   ArthurTheDigital (arthurthedigital@gmail.com)
@since    $Id: $
*/

#pragma once


namespace ATD {

/**
 * @brief ...
 * @param n1 - ...
 * @param n2 - ...
 * @return ... */
template<typename T>
inline T Min(const T &n1, const T &n2)
{ return (n1 < n2) ? n1 : n2; }

/**
 * @brief ...
 * @param n1 - ...
 * @param n2 - ...
 * @return ... */
template<typename T>
inline T Max(const T &n1, const T &n2)
{ return (n1 > n2) ? n1 : n2; }

/**
 * @brief ...
 * @param n      - value to be clamped
 * @param nLower - lower bound of n value
 * @param nUpper - upper bound of n value
 * @return clamped value */
template<typename T>
inline T Clamp(const T &n, const T &nLower, const T &nUpper)
{ return Max<T>(Min<T>(n, nUpper), nLower); }

} /* namespace ATD */


