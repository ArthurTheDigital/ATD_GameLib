/**
 * @file      
 * @brief     Mouse implementation.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Vector2.hpp>
#include <ATD/Window/KeyState.hpp>
#include <ATD/Window/Window.hpp>

#include <map>


namespace ATD {

/**
 * @brief ...
 * @class ...
 *
 * While the mouse button is pressed, mouse shall ignore all the other buttons
 * getting pressed. Until this button is released. */
class Mouse : public Window::Observer
{
public:
	/**
	 * @brief ... */
	enum ButtonId {
		NONE = 0, 
		LEFT, 
		RIGHT, 
		WHEEL, 
		EXTRA1, 
		EXTRA2, 
		COUNT
	};

	static const ButtonId INVALID = COUNT;

	/**
	 * @brief ...
	 * @class ... */
	class Event : public Window::Event
	{
	public:
		/**
		 * @brief ...
		 * @param n_type     - ...
		 * @param n_position - ... */
		inline Event(const Window::Event::Type &n_type = Window::Event::INVALID, 
				const Vector2I &n_position = Vector2I())
			: Window::Event(n_type)
		{ position() = n_position; }

		/**
		 * @brief ...
		 * @param other - ... */
		inline Event(const Event &other)
			: Window::Event(static_cast<const Window::Event &>(other))
		{}

		/**
		 * @brief ...
		 * @return ... */
		inline Vector2I &position()
		{ return *(reinterpret_cast<Vector2I *>(&data[0])); }

		/**
		 * @brief ...
		 * @return ... */
		inline const Vector2I &position() const
		{ return *(reinterpret_cast<const Vector2I *>(&data[0])); }

		/**
		 * @brief ...
		 * @return ... */
		inline Vector2I &scroll()
		{ return *(reinterpret_cast<Vector2I *>(&data[sizeof(Vector2I)])); }

		/**
		 * @brief ...
		 * @return ... */
		inline const Vector2I &scroll() const
		{
			return *(reinterpret_cast<const Vector2I *>(
						&data[sizeof(Vector2I)]));
		}

		/**
		 * @brief ...
		 * @return ... */
		inline ButtonId &button()
		{ return *(reinterpret_cast<ButtonId *>(&data[sizeof(Vector2I)])); }

		/**
		 * @brief ...
		 * @return ... */
		inline const ButtonId &button() const
		{
			return *(reinterpret_cast<const ButtonId *>(
						&data[sizeof(Vector2I)]));
		}
	};


	static const uint32_t PRESS_RELEASE_MASK;
	static const uint32_t POSITIONED_EVENT_MASK;
	static const uint32_t EVENT_MASK;


	/**
	 * @brief ...
	 * @param window - ... */
	Mouse(Window *window);

	/**
	 * @brief ...
	 * @return ... */
	inline const Vector2I &position() const
	{ return m_position; }

	/**
	 * @brief ...
	 * @return ... */
	inline const Vector2I &scroll() const
	{ return m_scroll; }

	/**
	 * @brief ...
	 * @return ... */
	inline const ButtonId &button() const
	{ return m_buttonId; }

	/**
	 * @brief ...
	 * @return ... */
	inline const KeyState &state() const
	{ return m_state; }

	// const KeyState &operator[](const ButtonId &button) const;

	// bool isPressed(const ButtonId &button) const;

	/**
	 * @brief ...
	 * @return ... */
	inline bool isValid() const
	{ return m_valid; }

	/**
	 * @brief ... */
	virtual void onPollStart() override;

	/**
	 * @brief ...
	 * @param event - ... */
	virtual void onNotify(const Window::Event &event) override;

private:
	Vector2I m_position;
	Vector2I m_scroll;
	// std::map<ButtonId, KeyState> m_buttonStates;
	ButtonId m_buttonId;
	KeyState m_state;

	/* FIXME: valid -> active? */
	bool m_valid; /* Whether he mouse is inside the window. */
};

} /* namespace ATD */


