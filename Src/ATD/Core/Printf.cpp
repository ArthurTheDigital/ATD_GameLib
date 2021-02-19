/**
 * @file      
 * @brief     Printf functions.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Core/Printf.hpp>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdexcept>


std::string ATD::Aux::printf(const char *format, ...)
{
	va_list args;

	::va_start(args, format);
	std::string result = vaPrintf(format, args);
	::va_end(args);

	return result;
}

std::string ATD::Aux::vaPrintf(const char *format, va_list args)
{
	std::string s;

	char *cString;
	int printfResult = ::vasprintf(&cString, format, args);
	if (printfResult == -1) {
		int errnoVal = errno;
		throw std::runtime_error(
				std::string("'::vasprintf()' failure: ") + 
				std::string(::strerror(errnoVal))
				);
	}
	s = std::string(cString);
	::free(cString);

	return s;
}


