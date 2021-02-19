/**
@file     
@brief    A demo of test.
@details  License: GPL v3.
@author   ArthurTheDigital (arthurthedigital@gmail.com)
@since    $Id: $
*/

#include <ATD/Core/AutoTest.hpp>
#include <ATD/Core/Debug.hpp>


int main(int argc, char** argv)
{
	{
		ATD::AutoTest tester("PASS test:", "right");
		IPRINTF("AUTOTEST", "right");
	}

	{
		ATD::AutoTest tester("FAIL test:", "right");
		IPRINTF("AUTOTEST", "wrong");
	}

	{
		ATD::AutoTest tester("forced PASS test:");
		tester.finish(true);
	}

	{
		ATD::AutoTest tester("forced FAIL test:");
		tester.finish(false);
	}

	{
		ATD::AutoTest tester("unexpected PASS test:", 
				"wrong", 
				true);

		IPRINTF("AUTOTEST", "wrong");
	}

	{
		ATD::AutoTest tester("expected FAIL test:", 
				"wrong", 
				true);

		IPRINTF("AUTOTEST", "right");
	}

	return 0;
}


