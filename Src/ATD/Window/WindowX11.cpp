/**
 * @file      
 * @brief     X11 window data implementation.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Window/WindowX11.hpp>

#include <atomic>

#define IGNORE_UNUSED(x) (void)(x)


/* ATD::Window::WindowX11 auxiliary: X11 init: */

static std::atomic<bool> _isX11Init(false);

static void _x11Init()
{
	if (!_isX11Init.load()) {
		/* Perform init */

		/* Ignore all IO X-errors */
		X11::XSetIOErrorHandler(
				[](X11::Display *displayPtr) -> int {
					IGNORE_UNUSED(displayPtr);
					/* FIXME: Shall I fire my own event here? */
					return 0;
				}
				);

		_isX11Init.store(true);
	}
}


/* ATD::Window::WindowX11 auxiliary: Shared X11 display access: */

static X11::Display *_sharedDisplayPtr = nullptr;
static unsigned long _sharedDisplayRc = 0; /* Rc - reference counter. */
static std::recursive_mutex _sharedDisplayMtx; /* Mutex. */

static X11::Display *_openSharedDisplay()
{
	std::lock_guard<std::recursive_mutex> lock(_sharedDisplayMtx);
	if (!_sharedDisplayRc) {
		_sharedDisplayPtr = X11::XOpenDisplay(nullptr);
		if (!_sharedDisplayPtr) {
			throw std::runtime_error("Failed to open X11 shared display!");
		}
	}
	_sharedDisplayRc++;
	return _sharedDisplayPtr;
}

static void _closeSharedDisplay()
{
	std::lock_guard<std::recursive_mutex> lock(_sharedDisplayMtx);
	if (_sharedDisplayRc) {
		_sharedDisplayRc--;
	}
	if (!_sharedDisplayRc && _sharedDisplayPtr) {
		X11::XCloseDisplay(_sharedDisplayPtr);
		_sharedDisplayPtr = nullptr;
	}
}


/* ATD::Window::WindowX11: */

ATD::Window::WindowX11::WindowX11(const ATD::Vector2S &n_size, 
		const std::string &n_title)
	: size(n_size)
	, position(ATD::Vector2L())
	, title(n_title)
{
	/* Make sure, X11 is initialized. */
	_x11Init();

	/* Obtain the root window (with 'precursors'). */
	displayPtr = _openSharedDisplay();
	screenId = X11::XDefaultScreen(displayPtr);
	X11::Window windowRoot = X11::_rootWindow(displayPtr, screenId);

	/* Get proper XVisualInfo from glAttributes. */
	ATD::Gl::Int glAttributes[] = {
		GLX_RGBA, 
		GLX_DEPTH_SIZE, 24, 
		GLX_DOUBLEBUFFER, 
		None
	};
	visualInfoPtr = X11::glXChooseVisual(displayPtr, 
			screenId, glAttributes);
	if (!visualInfoPtr) {
		throw std::runtime_error("'glXChooseVisual(..)' failure");
	}

	/* Color map. */
	windowAttributes.colormap = X11::XCreateColormap(
			displayPtr, 
			windowRoot, 
			visualInfoPtr->visual, 
			AllocNone /* Macro */
			);

	/* Event mask. */
	windowAttributes.event_mask = ExposureMask | /* Window shown */
		KeyPressMask | KeyReleaseMask | /* Keyboard */
		ButtonPressMask | ButtonReleaseMask | PointerMotionMask | /* Mouse */
		EnterWindowMask | LeaveWindowMask | /* Mouse entered/left */
		FocusChangeMask | /* Focus */
		StructureNotifyMask; /* Window resize and ?move */

	/* Create window. */
	window = X11::XCreateWindow(
			displayPtr, 
			windowRoot, 
			static_cast<int>(position.x), static_cast<int>(position.y), 
			static_cast<int>(size.x), static_cast<int>(size.y), 
			0, /* Border width */
			visualInfoPtr->depth, 
			InputOutput, 
			visualInfoPtr->visual, 
			CWColormap | CWEventMask, /* Which attributes to be read */
			&windowAttributes
			);

	/* OpenGL context. */
	glRenderCtx = X11::glXCreateContext(displayPtr, 
			visualInfoPtr, nullptr, GL_TRUE);
	X11::glXMakeCurrent(displayPtr, window, glRenderCtx);

	ATD::gl.viewport(0, 0, size.x, size.y);
	ATD::gl.enable(ATD::Gl::TEXTURE_2D);

	/* Window title. */
	X11::XStoreName(displayPtr, window, title.c_str());

	/* Clear & show the window. */
	X11::XClearWindow(displayPtr, window);
	X11::XMapRaised(displayPtr, window);
}

ATD::Window::WindowX11::~WindowX11()
{
	X11::glXDestroyContext(displayPtr, glRenderCtx);

	X11::XDestroyWindow(displayPtr, window);

	_closeSharedDisplay();
}


