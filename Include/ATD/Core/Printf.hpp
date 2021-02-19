/**
 * @file      
 * @brief     Printf functions.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <stdarg.h>

#include <string>


namespace ATD {

namespace Aux {

/**
 * @brief ...
 * @param format - C-style format
 * @return ... */
std::string printf(const char *format, ...);

/**
 * @brief ...
 * @param format - C-style format
 * @param args   - ...
 * @return ... */
std::string vaPrintf(const char *format, va_list args);

} /* namespace Aux */

} /* namespace ATD */


