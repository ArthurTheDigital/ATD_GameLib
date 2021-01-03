/**
* @file     
* @brief    Printf functions.
* @details  License: GPL v3.
* @author   ArthurTheDigital (arthurthedigital@gmail.com)
* @since    $Id: $ */

#pragma once

#include <stdarg.h>

#include <string>


namespace ATD {

/**
 * @brief ...
 * @param format - C-style format
 * @return ... */
std::string Printf(const char *format, ...);

/**
 * @brief ...
 * @param format - C-style format
 * @param args   - ...
 * @return ... */
std::string VaPrintf(const char *format, va_list args);

} /* namespace ATD */


