/**
 * @file
 * @brief     X11 window data implementation.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Window/Window.hpp>

#include <ATD/Core/Vector2.hpp>

/* In order to avoid confusion */
namespace X11 {

#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>

/* Namespaced static function instead of macro */
static Window _RootWindow(Display *displayPtr, int screenId)
{ return RootWindow(displayPtr, screenId); }

} /* namespace X11 */


/* ATD::Window::WindowX11: */

class ATD::Window::WindowX11
{
public:
	WindowX11(const Vector2S &n_size, 
			const std::string &n_title);

	~WindowX11();


	/* Cached size, position and title. */
	Vector2S size;
	Vector2S position;
	std::string title;

	X11::Display *displayPtr = nullptr;
	X11::XVisualInfo *visualInfoPtr = nullptr;
	X11::XSetWindowAttributes windowAttributes = X11::XSetWindowAttributes();
	X11::Window window = 0;
	X11::GLXContext glRenderCtx = 0;
	int screenId = 0;

	/* FIXME: Shall I have input context? */
};


