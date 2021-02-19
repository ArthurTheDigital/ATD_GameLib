/**
@file     
@brief    A test, ensuring, that UTF8 parsing is correct.
@details  License: GPL v3.
@author   ArthurTheDigital (arthurthedigital@gmail.com)
@since    $Id: $
*/

#include <ATD/Core/AutoTest.hpp>
#include <ATD/Core/Debug.hpp>
#include <ATD/Core/ErrWriter.hpp>
#include <ATD/Core/LpMath.hpp>
#include <ATD/Core/Printf.hpp>

#include <cmath>
#include <string>


void testLpSqrt(float eps, bool failureExpected = false)
{
	ATD::AutoTest tester(ATD::Aux::printf("Lp::sqrt(9.f) eps = %gf:", eps), 
			std::string(), 
			failureExpected);

	const float x = 9.f;
	const float y = ::sqrt(x);

	const float yLp = ATD::Lp::sqrt(x);
	if (yLp < y + eps && yLp > y - eps) {
		tester.finish(true);
	} else {
		IPRINTF("", "Incorrect Sqrt(): %f instead of %f", 
				yLp, y);
		tester.finish(false);
	}
}

void testLpRSqrt(float eps, bool failureExpected = false)
{
	ATD::AutoTest tester(ATD::Aux::printf("Lp::rSqrt(9.f) eps = %gf:", eps), 
			std::string(), 
			failureExpected);

	const float x = 9.f;
	const float y = 1.f / ::sqrt(x);

	const float yLp = ATD::Lp::rSqrt(x);
	if (yLp < y + eps && yLp > y - eps) {
		tester.finish(true);
	} else {
		IPRINTF("", "Incorrect RSqrt(): %f instead of %f", 
				yLp, y);
		tester.finish(false);
	}
}

int main()
{
	/* ATD::ErrWriter err; // DEBUG */

	testLpSqrt(0.001f);
	testLpSqrt(0.0001f);
	testLpSqrt(0.00001f);
	testLpSqrt(0.000001f);
	testLpSqrt(0.0000001f, true);

	testLpRSqrt(0.001f);
	testLpRSqrt(0.0001f);
	testLpRSqrt(0.00001f);
	testLpRSqrt(0.000001f);
	testLpRSqrt(0.0000001f, true);

	return 0;
}


