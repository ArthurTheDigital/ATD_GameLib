

#include <ATD/Core/Debug.hpp>
#include <ATD/Core/ErrWriter.hpp>
#include <ATD/Core/Fs.hpp>
#include <ATD/Graphics/Gl.hpp>
#include <ATD/Graphics/Sprite.hpp>
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
		img.load(fs.binDir().joined(ATD::Fs::Path("TestTexture-0001.png")));

		ATD::Sprite spr(ATD::Texture::CPtr(new ATD::Texture(img)));

		while (1) {
			std::chrono::time_point<std::chrono::steady_clock> timeStart = 
				std::chrono::steady_clock::now();
			win.poll();

			{ /* Update the model. */
				ATD::Vector2D deltaOffset;
				if (kb[ATD::Key::UP].isPressed()) {
					deltaOffset += ATD::Vector2D(0., -SHIFT_STEP);
				}
				if (kb[ATD::Key::DOWN].isPressed()) {
					deltaOffset += ATD::Vector2D(0., SHIFT_STEP);
				}
				if (kb[ATD::Key::LEFT].isPressed()) {
					deltaOffset += ATD::Vector2D(-SHIFT_STEP, 0.);
				}
				if (kb[ATD::Key::RIGHT].isPressed()) {
					deltaOffset += ATD::Vector2D(SHIFT_STEP, 0.);
				}

				double deltaAngleFrc = 0.;
				if (kb[ATD::Key::Q].isPressed()) {
					deltaAngleFrc += ROTATE_STEP_FRC;
				}
				if (kb[ATD::Key::W].isPressed()) {
					deltaAngleFrc -= ROTATE_STEP_FRC;
				}

				const ATD::Transform2D transform = spr.transform();
				spr.setOffset(transform.offset() + deltaOffset);
				spr.setAngleFrc(transform.angleFrc() + deltaAngleFrc);
			}

			win.clear();

			win.draw(spr);

			win.display();

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


