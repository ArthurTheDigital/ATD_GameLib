/**
@file     
@brief    Terminal ANSI graphics test.
@details  License: GPL v3.
@author   ArthurTheDigital (arthurthedigital@gmail.com)
@since    $Id: $
*/

#include "Firework.hpp"

#include <ATD/Ansi/Graphics.hpp>
#include <ATD/Core/Printf.hpp>

#include <stdlib.h>
#include <time.h>

#include <list>


int main()
{
	ATD::Ansi::Screen screen;

	/* Canary line */
	::fprintf(stdout, "%s\n", std::string(screen.Size().x, '=').c_str());

	/*
	screen.Draw(8, 3, ATD::Ansi::Glyph(ATD::Unicode("@").Front(), 208));

	screen.Draw(8, 4, ATD::Ansi::Glyph(ATD::Unicode("X").Front(), 127));

	screen.Draw(8, 5, ATD::Ansi::Image(10, 1, 
				ATD::Ansi::Glyph(ATD::Unicode("8").Front(), 127)));

	screen.Draw(8, 7, ATD::Ansi::Image(ATD::Printf("%s %d", "Test text", 1), 172));

	screen.Display();
	*/

	std::list<Firework> fireworks;

	while (1) {
		size_t w = screen.Size().x > 9 ? screen.Size().x - 9 : 1;
		size_t h = screen.Size().y > 5 ? screen.Size().y - 5 : 1;

		if (::rand() % 10 < 2) {
			size_t newFireworks = 1 + static_cast<size_t>(::rand() % 3);
			for (size_t iF = 0; iF < newFireworks; iF++) {
				unsigned char color = 160 + ::rand() % (177 - 160);
				fireworks.push_back(
						Firework(
							static_cast<long>(::rand() % static_cast<int>(w)), 
							static_cast<size_t>(::rand() % static_cast<int>(h)), 
							screen.Size().y - 1, 
							color, 
							color - 2
							)
						);
			}
		}

		screen.Clear();

		// /*
		size_t particlesTotal = 0;

		for (auto fIter = fireworks.begin(); fIter != fireworks.end();) {
			if (fIter->Finished()) {
				fIter = fireworks.erase(fIter);
			} else {
				particlesTotal += 1 + fIter->ParticlesNum();
				screen.Draw(*fIter);
				fIter->Update();
				fIter++;
			}
		}

		screen.Draw(ATD::Vector2L(8, 20), 
				ATD::Ansi::Image(ATD::Printf(" Particles total: %lu ", particlesTotal)));
		// DEBUG */

		/* screen.Draw(ATD::Vector2L(8, 18), ATD::Ansi::Glyph(ATD::Unicode("@").Front(), 170)); */

		screen.Display();

		struct timespec sleepTime = {.tv_sec = 0, .tv_nsec = 80000000};
		// struct timespec sleepTime = {.tv_sec = 1, .tv_nsec = 0};
		::nanosleep(&sleepTime, nullptr);
	}

	return 0;
}


