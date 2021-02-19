
#include "Camera.hpp"
#include "Model.hpp"

#include <ATD/Core/Debug.hpp>
#include <ATD/Core/ErrWriter.hpp>
#include <ATD/Core/Fs.hpp>
#include <ATD/Core/Vector2.hpp>
#include <ATD/Core/Vector3.hpp>
#include <ATD/Graphics/Shader.hpp>
#include <ATD/Window/Keyboard.hpp>
#include <ATD/Window/Window.hpp>

#include <stdio.h>

#include <chrono>
#include <thread>


const double ANGULAR_VELOCITY_FRC = 0.005;
const double RADIAL_VELOCITY = 0.05;


/* === */

int main(int argc, char **argv)
{
	ATD::ErrWriter errDebug; /* Enable debug output to stderr. */
	ATD::Fs fs(ATD::Fs::Path(argv[0], ATD::Fs::Path::NATIVE));
	try {
		ATD::Window win(ATD::Vector2S(600, 600), 
				ATD::Vector2L(200, 200), 
				"Test"
				);

		ATD::Keyboard kb(&win);

		ATD::Image textureImg;
		ATD::Fs::Path textureFilePath = fs.binDir().joined(
				ATD::Fs::Path("Dice-6-0001.png"));
		textureImg.load(textureFilePath);

		Model model(textureImg);
		Camera cam;
		IPRINTF("", "start");

		while (1) {
			std::chrono::time_point<std::chrono::steady_clock> timeStart = 
				std::chrono::steady_clock::now();
			win.poll();

			/* Camera: */

			{
				ATD::Vector3D center(0., 0., 5.);

				if (kb.isPressed(ATD::Key::W)) {
					cam.rotateAround(ATD::Vector2D(ANGULAR_VELOCITY_FRC, 0.), 
							center);
				}
				if (kb.isPressed(ATD::Key::S)) {
					cam.rotateAround(ATD::Vector2D(-ANGULAR_VELOCITY_FRC, 0.), 
							center);
				}
				if (kb.isPressed(ATD::Key::A)) {
					cam.rotateAround(ATD::Vector2D(0., -ANGULAR_VELOCITY_FRC), 
							center);
				}
				if (kb.isPressed(ATD::Key::D)) {
					cam.rotateAround(ATD::Vector2D(0., ANGULAR_VELOCITY_FRC), 
							center);
				}

				if (kb.isPressed(ATD::Key::Q)) {
					cam.changeDistance(-RADIAL_VELOCITY, center);
				}
				if (kb.isPressed(ATD::Key::E)) {
					cam.changeDistance(RADIAL_VELOCITY, center);
				}

				win.setProjection3D(cam.getProjection());
			}

			/* Rendering: */

			win.clear();
			win.draw(model);
			win.display();

			/* Screenshot: */

			if (kb[ATD::Key::P].isHeldStart()) {
				ATD::Texture::CPtr winColorTexture = win.getColorTexture();
				ATD::Image::Ptr screenshot = winColorTexture->getImage();
				ATD::Fs::Path screenshotFilePath = fs.binDir().joined(
						ATD::Fs::Path("Screenshot.png"));

				if (screenshotFilePath.exists()) {
					screenshotFilePath.unlink();
				}

				screenshot->save(screenshotFilePath);
			}

			/* Timing: */

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


