

#pragma once

#include <ATD/Core/Vector2.hpp>
#include <ATD/Window/KeyState.hpp>
#include <ATD/Window/Window.hpp>

#include <map>


namespace ATD {

class Mouse : public Window::Observer
{
public:
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

	class Event : public Window::Event
	{
	public:
		inline Event(
				const Window::Event::Type &n_type = Window::Event::INVALID, 
				const Vector2I &n_position = Vector2I())
			: Window::Event(n_type)
		{ Position() = n_position; }

		inline Event(const Event &other)
			: Window::Event(static_cast<const Window::Event &>(other))
		{}

		inline Vector2I &Position()
		{ return *(reinterpret_cast<Vector2I *>(&data[0])); }

		inline const Vector2I &Position() const
		{ return *(reinterpret_cast<const Vector2I *>(&data[0])); }

		inline Vector2I &Scroll()
		{ return *(reinterpret_cast<Vector2I *>(&data[sizeof(Vector2I)])); }

		inline const Vector2I &Scroll() const
		{
			return *(reinterpret_cast<const Vector2I *>(
						&data[sizeof(Vector2I)]));
		}

		inline ButtonId &Button()
		{ return *(reinterpret_cast<ButtonId *>(&data[sizeof(Vector2I)])); }

		inline const ButtonId &Button() const
		{
			return *(reinterpret_cast<const ButtonId *>(
						&data[sizeof(Vector2I)]));
		}
	};


	static const uint32_t POSITIONED_EVENT_MASK;
	static const uint32_t EVENT_MASK;


	Mouse(Window *window);

	const Vector2I &Position() const;

	const Vector2I &Scroll() const;

	const KeyState &operator[](const ButtonId &button) const;

	bool IsPressed(const ButtonId &button) const;

	bool IsValid() const;

	virtual void OnPollStart() override;

	virtual void OnNotify(const Window::Event &event) override;

private:
	Vector2I m_position;
	Vector2I m_scroll;
	std::map<ButtonId, KeyState> m_buttonStates;

	/* FIXME: valid -> active? */
	bool m_valid; /* Whether he mouse is inside the window. */
};

} /* namespace ATD */


