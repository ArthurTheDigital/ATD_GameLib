/**
* @file     
* @brief    Printf functions.
* @details  License: GPL v3.
* @author   ArthurTheDigital (arthurthedigital@gmail.com)
* @since    $Id: $ */

#include <ATD/Core/Printf.hpp>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdexcept>


std::string ATD::Printf(const char *format, ...)
{
	va_list args;

	::va_start(args, format);
	std::string result = VaPrintf(format, args);
	::va_end(args);

	return result;
}

std::string ATD::VaPrintf(const char *format, va_list args)
{
	std::string s;

	char *cString;
	int printfResult = ::vasprintf(&cString, format, args);
	if (printfResult == -1) {
		int errnoVal = errno;
		throw std::runtime_error(std::string("'::vasprintf()' failure: ") + std::string(::strerror(errnoVal)));
	}
	s = std::string(cString);
	::free(cString);

	return s;
}


