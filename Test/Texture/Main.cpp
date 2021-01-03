

#include "Sprite.hpp"

#include <ATD/Core/Debug.hpp>
#include <ATD/Core/ErrWriter.hpp>
#include <ATD/Core/Fs.hpp>
#include <ATD/Graphics/Gl.hpp>
#include <ATD/Window/Keyboard.hpp>
#include <ATD/Window/Window.hpp>

#include <chrono>
#include <thread>


const double SHIFT_STEP = 1.;
const double ROTATE_STEP_FRC = 0.005;


int main(int argc, char **argv)
{
	ATD::ErrWriter dbgStderr; /* Enable debug output stderr. */
	ATD::Fs fs(ATD::Fs::Path(argv[0], ATD::Fs::Path::NATIVE)); /* FS. */

	try {
		ATD::Window win(ATD::Vector2S(600, 600), ATD::Vector2L(200, 200), 
				"Test");

		ATD::Keyboard kb(&win);

		ATD::Image img;
		img.Load(fs.BinDir().Joined(ATD::Fs::Path("TestTexture-0001.png")));
		Sprite spr(img);

		while (1) {
			std::chrono::time_point<std::chrono::steady_clock> timeStart = 
				std::chrono::steady_clock::now();
			win.Poll();

			{ /* Update the model. */
				if (kb[ATD::Key::UP].IsPressed()) {
					spr.Shift(ATD::Vector2D(0., -SHIFT_STEP));
				}
				if (kb[ATD::Key::DOWN].IsPressed()) {
					spr.Shift(ATD::Vector2D(0., SHIFT_STEP));
				}
				if (kb[ATD::Key::LEFT].IsPressed()) {
					spr.Shift(ATD::Vector2D(-SHIFT_STEP, 0.));
				}
				if (kb[ATD::Key::RIGHT].IsPressed()) {
					spr.Shift(ATD::Vector2D(SHIFT_STEP, 0.));
				}

				if (kb[ATD::Key::Q].IsPressed()) {
					spr.RotateCw(ROTATE_STEP_FRC);
				}
				if (kb[ATD::Key::W].IsPressed()) {
					spr.RotateCw(-ROTATE_STEP_FRC);
				}
			}

			win.Clear();

			win.Draw(spr);

			win.Display();

			std::chrono::time_point<std::chrono::steady_clock> timeEnd = 
				std::chrono::steady_clock::now();

			std::chrono::milliseconds frameDuration(
					std::chrono::duration_cast<std::chrono::milliseconds>(
						timeEnd.time_since_epoch()).count() - 
					std::chrono::duration_cast<std::chrono::milliseconds>(
						timeStart.time_since_epoch()).count()
					);
			const std::chrono::milliseconds frameDurationNorm(1000/60);
			if (frameDuration < frameDurationNorm) {
				std::this_thread::sleep_for(frameDurationNorm - 
						frameDuration);
			}
		}
	} catch (const std::exception &e_err) {
		::fprintf(stderr, "%s\n", e_err.what());
	}
}


