/**
 * @file      
 * @brief     AutoTest class for debug.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Debug.hpp>

#include <string>


namespace ATD {

/**
 * @class ...
 * @brief ... */
class AutoTest : public Debug::Observer
{
public:
	/**
	 * @brief ...
	 * @param name            - the name of the test (as shown in output)
	 * @param control         - the control string
	 * @param failureExpected - ...
	 *
	 * Automatically Attaches to the debug. */
	AutoTest(const std::string &name, 
			const std::string &control = std::string(), 
			bool failureExpected = false);

	/**
	 * @brief ... */
	~AutoTest();

	/**
	 * @brief ...
	 * @param line - ... */
	virtual void onNotify(const Debug::Line &line) override;

	/**
	 * @brief Force test to finish and print output.
	 * @param pass - whether the test is considered passed
	 *
	 * Even if pass == true, but the test has a control string set, 
	 * the success/failure still depend on matching control string. */
	void finish(bool pass = true);

private:
	std::string m_name;
	std::string m_control;
	std::string m_test;
	bool m_failureExpected;
	bool m_finished;
};

} /* namespace ATD */


