/**
 * @file
 * @brief     Internal window data implementation.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Window/Window.hpp>

#include <ATD/Window/WindowX11.hpp>

#include <list>


/* ATD::Window::WindowInternal: */

class ATD::Window::WindowInternal
{
public:
	class FbResize;

	WindowInternal(const ATD::Vector2S &size, 
			size_t n_pixelSize);

	~WindowInternal();

	void ProcessX11Events(std::list<X11::XEvent> &eventsX11, 
			std::list<Window::Event> &eventsResult, 
			WindowX11 &winX11);

	void Display(WindowX11 &winX11);

	Matrix3F Coords2DTransformMatrix(const WindowX11 &winX11) const;


	bool hasFocus;
	bool isClosed;

	size_t pixelSize;
	Align alignX;
	Align alignY;

	FrameBuffer::Ptr frameBufferPtr; /* Size == frameBuffer.Size() */
	VertexBuffer2D::Ptr verticesPtr;
	Shader2D shader;
	Transform2D transform; /* Pixel-perfect upscale transform. */

	/* FrameBuffer resize truly happens only after Display()! */
	FbResize *fbResizePtr;
};


