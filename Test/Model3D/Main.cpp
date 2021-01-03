
#include "Camera.hpp"
#include "Model.hpp"

#include <ATD/Core/Debug.hpp>
#include <ATD/Core/ErrWriter.hpp>
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
		ATD::Fs::Path textureFilePath = fs.BinDir().Joined(
				ATD::Fs::Path("Dice-6-0001.png"));
		textureImg.Load(textureFilePath);

		Model model(textureImg);
		Camera cam;
		IPRINTF("", "start");

		while (1) {
			std::chrono::time_point<std::chrono::steady_clock> timeStart = 
				std::chrono::steady_clock::now();
			win.Poll();

			/* Camera: */

			{
				ATD::Vector3D center(0., 0., 5.);

				if (kb.IsPressed(ATD::Key::W)) {
					cam.RotateAround(ATD::Vector2D(ANGULAR_VELOCITY_FRC, 0.), 
							center);
				}
				if (kb.IsPressed(ATD::Key::S)) {
					cam.RotateAround(ATD::Vector2D(-ANGULAR_VELOCITY_FRC, 0.), 
							center);
				}
				if (kb.IsPressed(ATD::Key::A)) {
					cam.RotateAround(ATD::Vector2D(0., -ANGULAR_VELOCITY_FRC), 
							center);
				}
				if (kb.IsPressed(ATD::Key::D)) {
					cam.RotateAround(ATD::Vector2D(0., ANGULAR_VELOCITY_FRC), 
							center);
				}

				if (kb.IsPressed(ATD::Key::Q)) {
					cam.ChangeDistance(-RADIAL_VELOCITY, center);
				}
				if (kb.IsPressed(ATD::Key::E)) {
					cam.ChangeDistance(RADIAL_VELOCITY, center);
				}

				win.SetProjection3D(cam.GetProjection());
			}

			/* Rendering: */

			win.Clear();
			win.Draw(model);
			win.Display();

			/* Screenshot: */

			if (kb[ATD::Key::P].IsHeldStart()) {
				ATD::Texture::CPtr winColorTexture = win.GetColorTexture();
				ATD::Image::Ptr screenshot = winColorTexture->GetImage();
				ATD::Fs::Path screenshotFilePath = fs.BinDir().Joined(
						ATD::Fs::Path("Screenshot.png"));

				if (screenshotFilePath.Exists()) {
					screenshotFilePath.Unlink();
				}

				screenshot->Save(screenshotFilePath);
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


