/**
@file     
@brief    Terminal ANSI keyboard test.
@details  License: GPL v3.
@author   ArthurTheDigital (arthurthedigital@gmail.com)
@since    $Id: $
*/


#include "Editor.hpp"

#include <stdlib.h>
#include <time.h>


int main()
{
	ATD::Ansi::Screen screen;
	ATD::Ansi::Keyboard keyboard;

	Editor editor;

	/* Canary line */
	::fprintf(stdout, "%s\n", std::string(screen.Size().x, '=').c_str());

	while (1) {
		screen.Clear();

		editor.Update(keyboard);
		screen.Draw(editor);
		screen.Display();

		struct timespec sleepTime = {.tv_sec = 0, .tv_nsec = 80000000};
		// struct timespec sleepTime = {.tv_sec = 1, .tv_nsec = 0};
		::nanosleep(&sleepTime, nullptr);
	}

	return 0;
}


