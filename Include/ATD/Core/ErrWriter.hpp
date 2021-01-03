
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

	virtual void OnNotify(const Debug::Line &line) override;
};

} /* namespace ATD */

