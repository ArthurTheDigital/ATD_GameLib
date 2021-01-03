/**
 * @file
 * @brief     Window class implementation.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Vector2.hpp>
#include <ATD/Graphics/FrameBuffer.hpp>

#include <string.h>

#include <list>
#include <map>
#include <mutex>


namespace ATD {

/**
 * @class ...
 * @brief ... */
class Window
{
public:

	class Event
	{
	public:
		enum Type {
			INVALID       = 0x00000000, 
			RESIZE        = 1 << 0, 
			CLOSE         = 1 << 1, /* TODO: Implement CLOSE event. */
			FOCUS_IN      = 1 << 2, 
			FOCUS_OUT     = 1 << 3, 
			MOUSE_IN      = 1 << 4, 
			MOUSE_OUT     = 1 << 5, 
			MOUSE_MOVE    = 1 << 6, 
			MOUSE_PRESS   = 1 << 7, 
			MOUSE_RELEASE = 1 << 8, 
			MOUSE_SCROLL  = 1 << 9, 
			KEY_PRESS     = 1 << 10, 
			KEY_RELEASE   = 1 << 11
		};

		static const size_t DATA_SIZE = 16;


		inline Event(const Type &n_type = INVALID)
			: type(n_type)
		{
			for (size_t dIter = 0; dIter < DATA_SIZE; dIter++) {
				data[dIter] = 0x00;
			}
		}

		inline Event(const Event &other)
			: Event(other.type)
		{ ::memcpy(data, other.data, DATA_SIZE); }


		Type type;
		uint8_t data[DATA_SIZE];
	};

	class Observer
	{
	public:
		Observer();

		virtual ~Observer();

		void Attach(Window *window, 
				uint32_t eventTypeMask = 
					static_cast<uint32_t>(Event::INVALID));

		void Detach();

		virtual void OnPollStart() = 0;

		virtual void OnNotify(const Event &event) = 0;

	private:
		Window *m_window;
	};

	/* To avoid division by zero ;) */
	enum PixelSize {
		PIXEL_X1 = 1, 
		PIXEL_X2 = 2, 
		PIXEL_X3 = 3, 
		PIXEL_X4 = 4
	};

	Window(const Vector2S &size, 
			const Vector2L &position, 
			const std::string &title, 
			const PixelSize &pixelSize = PIXEL_X1);

	virtual ~Window();

	/**
	 * @brief refresh inner Event queue
	 * @param keepEvents - whether to store the newfound events
	 *
	 * You may call Poll() if you don't want to handle all window events 
	 * one-by-one and totally rely on Observers automation. */
	void Poll(bool keepEvents = false);

	/**
	 * @brief get the latest event
	 * @param evt - link to the result event
	 * @return true if gathered event, otherwise false (no events yet) */
	bool PollEvent(Event &evt);

	Vector2U Size() const;

	bool HasFocus() const;

	bool IsClosed() const;

	void SetShader2D(Shader2D::Ptr shader2DPtr);

	void SetShader3D(Shader3D::Ptr shader3DPtr);

	void SetProjection2D(const Projection2D &projection2D);

	void SetProjection3D(const Projection3D &projection3D);

	void Clear();

	void Draw(const ATD::FrameBuffer::Drawable &drawable);

	void Draw(const VertexBuffer2D &vertices2D, 
			const Transform2D &transform);

	/* TODO: Draw(const VertexBuffer3D &); */

	Texture::CPtr GetColorTexture() const;

	void Display();

private:
	/* X11 has nasty defines in it's header files, which conflict with 
	 * my names, though I keep all my names inside namespaces. Hence I've 
	 * decided to not include X11 in headers and use it internally only. */

	/**
	 * @brief X11 data of the window */
	class WindowX11;

	/**
	 * @brief ... */
	class WindowInternal;

	/* Non-copyable. */
	Window(const Window &other) = delete;

	/* Accessible from Observer. */
	friend void Observer::Attach(Window *, uint32_t);
	friend void Observer::Detach();


	WindowX11 *m_x11; /* X11 data */
	WindowInternal *m_internal; /* Internal data */

	/* Observers data. */
	std::map<Observer *, uint32_t> m_observerPtrs;
	std::recursive_mutex m_observerPtrsLock;

	std::list<Event> m_events; /* Cached events. */
};

}


