/**
 * @file      
 * @brief     Keyboard implementation.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Window/KeyState.hpp>
#include <ATD/Window/Window.hpp>

#include <map>
#include <set>
#include <string>


namespace ATD {

namespace Key
{
	/**
	 * @brief ... */
	enum Code
	{
		/* English letters: */
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, 
		X, Y, Z, 

		/* Numbers: */
		N_0, N_1, N_2, N_3, N_4, N_5, N_6, N_7, N_8, N_9, 

		/* Symbols from letter block: */
		L_BRACKET, R_BRACKET, SEMICOLON, QUOTE, COMMA, DOT, SLASH, B_SLASH, 
		TILDE, HYPHEN, EQUAL, 

		/* Controls: */
		L_SHIFT, R_SHIFT, L_CTRL, R_CTRL, L_ALT, R_ALT, L_SUPER, R_SUPER, 
		ESC, TAB, SPACE, ENTER, /* FIXME: MENU, PAUSE */

		/* Arrows: */
		UP, DOWN, LEFT, RIGHT, 

		/* Numpad: */
		NP_0, NP_1, NP_2, NP_3, NP_4, NP_5, NP_6, NP_7, NP_8, NP_9, 
		NP_SLASH, NP_STAR, NP_HYPHEN, NP_PLUS, NP_DOT, NP_ENTER, 

		/* Page navigation: */
		HOME, PG_UP, PG_DOWN, END, 

		/* F line: */
		F_1, F_2, F_3, F_4, F_5, F_6, F_7, F_8, F_9, F_10, F_11, F_12, 
		/* FIXME: F_13, ... F_15 */

		COUNT
	};

	/**
	 * @brief ... */
	enum Collective
	{
		SHIFT = static_cast<unsigned>(COUNT) + 1, 
		CTRL, 
		ALT, 
		SUPER, 

		COL_COUNT
	};

	const Code INVALID = COUNT;

	extern const std::map<Code, std::string> CODE_STR;
	extern const std::map<Collective, std::string> COLLECTIVE_STR;
}


/**
 * @brief ...
 * @class ... */
class Keyboard : public Window::Observer
{
public:
	/**
	 * @brief ...
	 * @class ... */
	class Event : public Window::Event
	{
	public:
		/**
		 * @brief ...
		 * @param n_type    - ...
		 * @param n_keyCode - ... */
		inline Event(
				const Window::Event::Type &n_type = Window::Event::INVALID, 
				const Key::Code &n_keyCode = Key::INVALID)
			: Window::Event(n_type)
		{ keyCode() = n_keyCode; }

		/**
		 * @brief ...
		 * @param other - ... */
		inline Event(const Event &other)
			: Event(other.type, other.keyCode())
		{}

		/**
		 * @brief ...
		 * @return ... */
		inline Key::Code &keyCode()
		{ return (*reinterpret_cast<Key::Code *>(&data[0])); }

		/**
		 * @brief ...
		 * @return ... */
		inline const Key::Code &keyCode() const
		{ return (*reinterpret_cast<const Key::Code *>(&data[0])); }
	};


	/**
	 * @brief ...
	 * @param window - ... */
	Keyboard(Window *window);

	/**
	 * @brief ...
	 * @param pressed - key code actually pressed
	 * @param read    - key code read by game */
	void remap(const Key::Code &pressed, const Key::Code &read);

	/**
	 * @brief ...
	 * @param code - ...
	 * @return ... */
	inline const KeyState &operator[](const Key::Code &code) const
	{ return m_keyStates.at(code); }

	/**
	 * @brief ...
	 * @param code - ...
	 * @return ... */
	inline bool isPressed(const Key::Code &code) const
	{ return m_keyStates.at(code).isPressed(); }

	/**
	 * @brief ...
	 * @param code - ...
	 * @return ... */
	bool isPressed(const Key::Collective &code) const;

	/**
	 * @brief ... */
	virtual void onPollStart() override;

	/**
	 * @brief ...
	 * @param event - constant link to the Event
	 *
	 * event.type is already supposed to be checked by Window, calling this 
	 * method, no additional check needed. */
	virtual void onNotify(const Window::Event &event) override;


private:
	std::map<Key::Code, KeyState> m_keyStates;
	std::map<Key::Collective, KeyState> m_collectiveStates;

	std::map<Key::Code, Key::Code> m_remap;
};

} /* namespace ATD */


