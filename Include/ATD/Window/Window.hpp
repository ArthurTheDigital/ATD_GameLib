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
 * @brief ...
 * @class ... */
class Window
{
public:

	/**
	 * @brief ...
	 * @class ... */
	class Event
	{
	public:
		/**
		 * @brief ... */
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


		/**
		 * @brief ...
		 * @param type - ... */
		inline Event(const Type &n_type = INVALID)
			: type(n_type)
		{
			for (size_t dIter = 0; dIter < DATA_SIZE; dIter++) {
				data[dIter] = 0x00;
			}
		}

		/**
		 * @brief ...
		 * @param other - ... */
		inline Event(const Event &other)
			: Event(other.type)
		{ ::memcpy(data, other.data, DATA_SIZE); }


		Type type;
		uint8_t data[DATA_SIZE];
	};

	/**
	 * @brief ...
	 * @class ... */
	class Observer
	{
	public:
		/**
		 * @brief ... */
		Observer();

		/**
		 * @brief ... */
		virtual ~Observer();

		/**
		 * @brief ...
		 * @param window        - ...
		 * @param eventTypeMask - ... */
		void attach(Window *window, 
				uint32_t eventTypeMask = 
					static_cast<uint32_t>(Event::INVALID));

		/**
		 * @brief ... */
		void detach();

		virtual void onPollStart() = 0;

		virtual void onNotify(const Event &event) = 0;

	private:
		Window *m_window;
	};

	/* @brief ...
	 *
	 * To avoid division by zero ;) */
	enum PixelSize {
		PIXEL_X1 = 1, 
		PIXEL_X2 = 2, 
		PIXEL_X3 = 3, 
		PIXEL_X4 = 4
	};

	/**
	 * @brief ...
	 * @param size      - ...
	 * @param position  - ...
	 * @param title     - ...
	 * @param pixelSize - ... */
	Window(const Vector2S &size, 
			const Vector2L &position, 
			const std::string &title, 
			const PixelSize &pixelSize = PIXEL_X1);

	/**
	 * @brief ... */
	virtual ~Window();

	/**
	 * @brief refresh inner Event queue
	 * @param keepEvents - whether to store the newfound events
	 *
	 * You may call poll() if you don't want to handle all window events 
	 * one-by-one and totally rely on Observers automation. */
	void poll(bool keepEvents = false);

	/**
	 * @brief get the latest event
	 * @param evt - link to the result event
	 * @return true if gathered event, otherwise false (no events yet) */
	bool pollEvent(Event &evt);

	/* The following methods cannot be inlined, because they use WindowInternal. */

	/**
	 * @brief ...
	 * @return ... */
	Vector2U size() const;

	/**
	 * @brief ...
	 * @return ... */
	bool hasFocus() const;

	/**
	 * @brief ...
	 * @return ... */
	bool isClosed() const;

	/**
	 * @brief ...
	 * @param shader2DPtr - ... */
	void setShader2D(Shader2D::Ptr shader2DPtr);

	/**
	 * @brief ...
	 * @param shader3DPtr - ... */
	void setShader3D(Shader3D::Ptr shader3DPtr);

	/**
	 * @brief ...
	 * @param projection2D - ... */
	void setProjection2D(const Projection2D &projection2D);

	/**
	 * @brief ...
	 * @param projection3D - ... */
	void setProjection3D(const Projection3D &projection3D);

	/**
	 * @brief ... */
	void clear();

	/**
	 * @brief ...
	 * @param drawable - ... */
	void draw(const ATD::FrameBuffer::Drawable &drawable);

	/**
	 * @brief ...
	 * @param vertices2D - ...
	 * @param transform  - ... */
	void draw(const VertexBuffer2D &vertices2D, 
			const Transform2D &transform);

	/* TODO: draw(const VertexBuffer3D &); */

	/**
	 * @brief ...
	 * @return ... */
	Texture::CPtr getColorTexture() const;

	/**
	 * @brief ... */
	void display();

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
	friend void Observer::attach(Window *, uint32_t);
	friend void Observer::detach();


	WindowX11 *m_x11; /* X11 data */
	WindowInternal *m_internal; /* Internal data */

	/* Observers data. */
	std::map<Observer *, uint32_t> m_observerPtrs;
	std::recursive_mutex m_observerPtrsLock;

	std::list<Event> m_events; /* Cached events. */
};

}


