

#include <ATD/Window/Keyboard.hpp>

#include <ATD/Core/Debug.hpp>
#include <ATD/Core/Printf.hpp>

#include <stdexcept>


/* ATD::Key constants: */

const std::map<ATD::Key::Code, std::string> ATD::Key::CODE_STR = {
	/* English letters: */
	{ATD::Key::A, "A"}, 
	{ATD::Key::B, "B"}, 
	{ATD::Key::C, "C"}, 
	{ATD::Key::D, "D"}, 
	{ATD::Key::E, "E"}, 
	{ATD::Key::F, "F"}, 
	{ATD::Key::G, "G"}, 
	{ATD::Key::H, "H"}, 
	{ATD::Key::I, "I"}, 
	{ATD::Key::J, "J"}, 
	{ATD::Key::K, "K"}, 
	{ATD::Key::L, "L"}, 
	{ATD::Key::M, "M"}, 
	{ATD::Key::N, "N"}, 
	{ATD::Key::O, "O"}, 
	{ATD::Key::P, "P"}, 
	{ATD::Key::Q, "Q"}, 
	{ATD::Key::R, "R"}, 
	{ATD::Key::S, "S"}, 
	{ATD::Key::T, "T"}, 
	{ATD::Key::U, "U"}, 
	{ATD::Key::V, "V"}, 
	{ATD::Key::W, "W"}, 
	{ATD::Key::X, "X"}, 
	{ATD::Key::Y, "Y"}, 
	{ATD::Key::Z, "Z"}, 

	/* Numbers: */
	{ATD::Key::N_0, "0"}, 
	{ATD::Key::N_1, "1"}, 
	{ATD::Key::N_2, "2"}, 
	{ATD::Key::N_3, "3"}, 
	{ATD::Key::N_4, "4"}, 
	{ATD::Key::N_5, "5"}, 
	{ATD::Key::N_6, "6"}, 
	{ATD::Key::N_7, "7"}, 
	{ATD::Key::N_8, "8"}, 
	{ATD::Key::N_9, "9"}, 

	/* Symbols from letter block: */
	{ATD::Key::L_BRACKET, "["}, 
	{ATD::Key::R_BRACKET, "]"}, 
	{ATD::Key::SEMICOLON, ";"}, 
	{ATD::Key::QUOTE, "'"}, 
	{ATD::Key::COMMA, ","}, 
	{ATD::Key::DOT, "."}, 
	{ATD::Key::SLASH, "/"}, 
	{ATD::Key::B_SLASH, "\\"}, 
	{ATD::Key::TILDE, "~"}, 
	{ATD::Key::HYPHEN, "-"}, 
	{ATD::Key::EQUAL, "="}, 

	/* Controls: */
	{ATD::Key::L_SHIFT, "LShift"}, 
	{ATD::Key::R_SHIFT, "RShift"}, 
	{ATD::Key::L_CTRL, "LCtrl"}, 
	{ATD::Key::R_CTRL, "RCtrl"}, 
	{ATD::Key::L_ALT, "LAlt"}, 
	{ATD::Key::R_ALT, "RAlt"}, 
	{ATD::Key::L_SUPER, "LSuper"}, 
	{ATD::Key::R_SUPER, "RSuper"}, 
	{ATD::Key::ESC, "Esc"}, 
	{ATD::Key::TAB, "Tab"}, 
	{ATD::Key::SPACE, "Space"}, 
	{ATD::Key::ENTER, "Enter"}, 

	/* Arrows: */
	{ATD::Key::UP, "▲"}, 
	{ATD::Key::DOWN, "▼"}, 
	{ATD::Key::LEFT, "◀"}, 
	{ATD::Key::RIGHT, "▶"}, 

	/* Numpad: */
	{ATD::Key::NP_0, "Num0"}, 
	{ATD::Key::NP_1, "Num1"}, 
	{ATD::Key::NP_2, "Num2"}, 
	{ATD::Key::NP_3, "Num3"}, 
	{ATD::Key::NP_4, "Num4"}, 
	{ATD::Key::NP_5, "Num5"}, 
	{ATD::Key::NP_6, "Num6"}, 
	{ATD::Key::NP_7, "Num7"}, 
	{ATD::Key::NP_8, "Num8"}, 
	{ATD::Key::NP_9, "Num9"}, 
	{ATD::Key::NP_SLASH, "Num/"}, 
	{ATD::Key::NP_STAR, "Num*"}, 
	{ATD::Key::NP_HYPHEN, "Num-"}, 
	{ATD::Key::NP_PLUS, "Num+"}, 
	{ATD::Key::NP_0, "Num."}, 
	{ATD::Key::NP_0, "NumEnter"}, 

	/* Page navigation: */
	{ATD::Key::HOME, "Home"}, 
	{ATD::Key::PG_UP, "PgUp"}, 
	{ATD::Key::PG_DOWN, "PgDn"}, 
	{ATD::Key::END, "End"}, 

	/* F line: */
	{ATD::Key::F_1, "F1"}, 
	{ATD::Key::F_2, "F2"}, 
	{ATD::Key::F_3, "F3"}, 
	{ATD::Key::F_4, "F4"}, 
	{ATD::Key::F_5, "F5"}, 
	{ATD::Key::F_6, "F6"}, 
	{ATD::Key::F_7, "F7"}, 
	{ATD::Key::F_8, "F8"}, 
	{ATD::Key::F_9, "F9"}, 
	{ATD::Key::F_10, "F10"}, 
	{ATD::Key::F_11, "F11"}, 
	{ATD::Key::F_12, "F12"}
};

const std::map<ATD::Key::Collective, std::string> ATD::Key::COLLECTIVE_STR = {
	{ATD::Key::SHIFT, "Shift"}, 
	{ATD::Key::CTRL, "Ctrl"}, 
	{ATD::Key::ALT, "Alt"}, 
	{ATD::Key::SUPER, "Super"}
};


/* ATD::Keyboard */

ATD::Keyboard::Keyboard(Window *window)
	: ATD::Window::Observer()
	, m_keyStates()
	, m_collectiveStates()
	, m_remap()
{
	for (unsigned keyCode = 0; keyCode <= static_cast<unsigned>(Key::COUNT); 
			keyCode++) {
		m_keyStates.insert(std::pair<Key::Code, KeyState>(
					static_cast<Key::Code>(keyCode), KeyState()));
	}
	for (unsigned keyCollective = static_cast<unsigned>(Key::COUNT) + 1; 
			keyCollective <= static_cast<unsigned>(Key::COL_COUNT); 
			keyCollective++) {
		m_collectiveStates.insert(std::pair<Key::Collective, KeyState>(
					static_cast<Key::Collective>(keyCollective), KeyState()));
	}

	Attach(window, 
			static_cast<uint32_t>(Window::Event::KEY_PRESS) | 
			static_cast<uint32_t>(Window::Event::KEY_RELEASE) | 
			static_cast<uint32_t>(Window::Event::FOCUS_OUT));
}

void ATD::Keyboard::Remap(const Key::Code &pressed, const Key::Code &read)
{
	if (pressed != Key::INVALID && read != Key::INVALID) {
		if (pressed != read) {
			m_remap.insert(std::pair<Key::Code, Key::Code>(pressed, read));
		} else {
			auto rIter = m_remap.find(pressed);
			if (rIter != m_remap.end()) {
				m_remap.erase(rIter);
			}
		}
	}
}

const ATD::KeyState &ATD::Keyboard::operator[](
		const ATD::Key::Code &code) const
{ return m_keyStates.at(code); }

bool ATD::Keyboard::IsPressed(const ATD::Key::Code &code) const
{ return m_keyStates.at(code).IsPressed(); }

bool ATD::Keyboard::IsPressed(const ATD::Key::Collective &code) const
{
	switch (code) {
		case Key::SHIFT:
			return (m_keyStates.at(Key::L_SHIFT).IsPressed() || 
					m_keyStates.at(Key::R_SHIFT).IsPressed());

		case Key::CTRL:
			return (m_keyStates.at(Key::L_CTRL).IsPressed() || 
					m_keyStates.at(Key::R_CTRL).IsPressed());

		case Key::ALT:
			return (m_keyStates.at(Key::L_ALT).IsPressed() || 
					m_keyStates.at(Key::R_ALT).IsPressed());

		case Key::SUPER:
			return (m_keyStates.at(Key::L_SUPER).IsPressed() || 
					m_keyStates.at(Key::R_SUPER).IsPressed());
		default:
			{}
	}
	return false;
}

void ATD::Keyboard::OnPollStart()
{
	/* Update key states. */
	for (auto &statePair : m_keyStates) {
		statePair.second.UpdatePrev();
	}
}

void ATD::Keyboard::OnNotify(const ATD::Window::Event &event)
{
	/* Update key states. */
	const Event *eventPtr = static_cast<const Event *>(&event);
	if (eventPtr->type == Window::Event::FOCUS_OUT) {
		/* Release all the keys, being pressed. */
		for (auto &statePair : m_keyStates) {
			statePair.second.UpdateCurr(false);
		}
	} else {
		auto remapIter = m_remap.find(eventPtr->KeyCode());
		Key::Code code = (remapIter != m_remap.end()) ? remapIter->second : 
			eventPtr->KeyCode();

		m_keyStates.at(code).UpdateCurr(
				(eventPtr->type == Window::Event::KEY_PRESS));

		/*
		// FIXME: Decide, what debug level should be here.
		IPRINTF("", "key %s %s", Key::CODE_STR.at(code).c_str(), 
				(eventPtr->type == Window::Event::KEY_PRESS) ? 
					"press" : 
					"release");
		*/
	}
}


