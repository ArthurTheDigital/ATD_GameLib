/**
 * @file      
 * @brief     ErrWriter class for debug.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Debug.hpp>

#include <string>


namespace ATD {

/**
 * @brief debug output to stderr
 * @class ... */
class ErrWriter : public Debug::Observer
{
public:
	static const std::string LINE_BREAKER;


	ErrWriter();

	virtual void onNotify(const Debug::Line &line) override;
};

} /* namespace ATD */

