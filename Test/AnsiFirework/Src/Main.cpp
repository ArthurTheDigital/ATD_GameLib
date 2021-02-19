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
	::fprintf(stdout, "%s\n", std::string(screen.size().x, '=').c_str());

	/*
	screen.draw(8, 3, ATD::Ansi::Glyph(ATD::Unicode("@").front(), 208));

	screen.draw(8, 4, ATD::Ansi::Glyph(ATD::Unicode("X").front(), 127));

	screen.draw(8, 5, ATD::Ansi::Image(10, 1, 
				ATD::Ansi::Glyph(ATD::Unicode("8").front(), 127)));

	screen.draw(8, 7, ATD::Ansi::Image(ATD::Aux::printf("%s %d", "Test text", 1), 172));

	screen.display();
	*/

	std::list<Firework> fireworks;

	while (1) {
		size_t w = screen.size().x > 9 ? screen.size().x - 9 : 1;
		size_t h = screen.size().y > 5 ? screen.size().y - 5 : 1;

		if (::rand() % 10 < 2) {
			size_t newFireworks = 1 + static_cast<size_t>(::rand() % 3);
			for (size_t iF = 0; iF < newFireworks; iF++) {
				unsigned char color = 160 + ::rand() % (177 - 160);
				fireworks.push_back(
						Firework(
							static_cast<long>(::rand() % static_cast<int>(w)), 
							static_cast<size_t>(::rand() % static_cast<int>(h)), 
							screen.size().y - 1, 
							color, 
							color - 2
							)
						);
			}
		}

		screen.clear();

		// /*
		size_t particlesTotal = 0;

		for (auto fIter = fireworks.begin(); fIter != fireworks.end();) {
			if (fIter->finished()) {
				fIter = fireworks.erase(fIter);
			} else {
				particlesTotal += 1 + fIter->particlesNum();
				screen.draw(*fIter);
				fIter->update();
				fIter++;
			}
		}

		screen.draw(ATD::Vector2L(8, 20), 
				ATD::Ansi::Image(
					ATD::Aux::printf(" Particles total: %lu ", particlesTotal)
					));
		// DEBUG */

		/* screen.Draw(ATD::Vector2L(8, 18), ATD::Ansi::Glyph(ATD::Unicode("@").front(), 170)); */

		screen.display();

		struct timespec sleepTime = {.tv_sec = 0, .tv_nsec = 80000000};
		// struct timespec sleepTime = {.tv_sec = 1, .tv_nsec = 0};
		::nanosleep(&sleepTime, nullptr);
	}

	return 0;
}


