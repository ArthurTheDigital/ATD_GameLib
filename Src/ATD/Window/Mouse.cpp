/**
 * @file      
 * @brief     Mouse implementation.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Window/Mouse.hpp>


/* ATD::Mouse constants: */

const uint32_t ATD::Mouse::PRESS_RELEASE_MASK = 
	static_cast<uint32_t>(Window::Event::MOUSE_PRESS) | 
	static_cast<uint32_t>(Window::Event::MOUSE_RELEASE);

const uint32_t ATD::Mouse::POSITIONED_EVENT_MASK = 
	ATD::Mouse::PRESS_RELEASE_MASK |
	static_cast<uint32_t>(Window::Event::MOUSE_MOVE) | 
	static_cast<uint32_t>(Window::Event::MOUSE_SCROLL);

const uint32_t ATD::Mouse::EVENT_MASK = 
	ATD::Mouse::POSITIONED_EVENT_MASK | 
	static_cast<uint32_t>(Window::Event::MOUSE_IN) | 
	static_cast<uint32_t>(Window::Event::MOUSE_OUT);


/* ATD::Mouse: */

ATD::Mouse::Mouse(ATD::Window *window)
	: Window::Observer()
	, m_position()
	, m_scroll()
//	, m_buttonStates()
	, m_buttonId(NONE)
	, m_state()
	, m_valid(true)
{
//	for (unsigned sIter = 0; sIter <= static_cast<unsigned>(ButtonId::COUNT); 
//			sIter++) {
//		m_buttonStates.insert(std::pair<ButtonId, KeyState>(
//					static_cast<ButtonId>(sIter), KeyState()));
//	}

	attach(window, EVENT_MASK);
}

//const ATD::KeyState &ATD::Mouse::operator[](
//		const ATD::Mouse::ButtonId &button) const
//{ return m_buttonStates.at(button); }

//bool ATD::Mouse::isPressed(const ATD::Mouse::ButtonId &button) const
//{ return m_buttonStates.at(button).isPressed(); }

void ATD::Mouse::onPollStart()
{
	/* Nullify scroll. */
	m_scroll = Vector2I();

	/* Update button state. */
	m_state.updatePrev();
	if (m_state.isIdle()) { m_buttonId = NONE; }
	// for (auto &statePair : m_buttonStates) { statePair.second.updatePrev(); }
}

void ATD::Mouse::onNotify(const ATD::Window::Event &event)
{
	const Event *eventPtr = static_cast<const Event *>(&event);

	if (static_cast<uint32_t>(eventPtr->type) & POSITIONED_EVENT_MASK) {
		m_position = eventPtr->position();
		if (static_cast<uint32_t>(eventPtr->type) & PRESS_RELEASE_MASK) {

			if (m_buttonId == NONE) {
				if (eventPtr->type == Window::Event::MOUSE_PRESS) {
					m_buttonId = eventPtr->button();
					m_state.updateCurr(true);
				}
			} else {
				if ((eventPtr->type == Window::Event::MOUSE_RELEASE) && 
						(eventPtr->button() == m_buttonId)) {
					/* m_buttonId shall not be updated yet. */
					m_state.updateCurr(false);
				}
			}

			// m_buttonStates.at(eventPtr->button()).updateCurr(
			//		(eventPtr->type == Window::Event::MOUSE_PRESS));
		} else if (eventPtr->type == Window::Event::MOUSE_SCROLL) {
			m_scroll += eventPtr->scroll();
		}
	} else {
		if (eventPtr->type == Window::Event::MOUSE_IN) {
			m_valid = true;
		} else { /* MOUSE_OUT */
			/* Reset the button state. */
			m_state.updateCurr(false);

			//for (auto &statePair : m_buttonStates) {
			//	statePair.second.updateCurr(false);
			//}

			m_valid = false;
		}
	}
}


