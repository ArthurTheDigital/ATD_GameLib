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

/**
 * @brief ...
 * @class ... */
class ATD::Window::WindowInternal
{
public:
	/**
	 * @brief ... */
	class FbResize;

	/**
	 * @brief ...
	 * @param size        - ...
	 * @param n_pixelSize - ... */
	WindowInternal(const ATD::Vector2S &size, 
			size_t n_pixelSize);

	/**
	 * @brief ... */
	~WindowInternal();

	/**
	 * @brief ...
	 * @param eventsX11    - ...
	 * @param eventsResult - ...
	 * @param winX11       - ... */
	void processX11Events(std::list<X11::XEvent> &eventsX11, 
			std::list<Window::Event> &eventsResult, 
			WindowX11 &winX11);

	/**
	 * @brief ...
	 * @param winX11 - ... */
	void display(WindowX11 &winX11);

	/**
	 * @brief ...
	 * @param winX11 - ...
	 * @return ... */
	Matrix3F coords2DTransformMatrix(const WindowX11 &winX11) const;


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


