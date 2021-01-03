
#include <ATD/Window/Mouse.hpp>


/* ATD::Mouse constants: */

const uint32_t ATD::Mouse::POSITIONED_EVENT_MASK = 
	static_cast<uint32_t>(Window::Event::MOUSE_MOVE) | 
	static_cast<uint32_t>(Window::Event::MOUSE_PRESS) | 
	static_cast<uint32_t>(Window::Event::MOUSE_RELEASE) | 
	static_cast<uint32_t>(Window::Event::MOUSE_SCROLL);

const uint32_t ATD::Mouse::EVENT_MASK = ATD::Mouse::POSITIONED_EVENT_MASK | 
	static_cast<uint32_t>(Window::Event::MOUSE_IN) | 
	static_cast<uint32_t>(Window::Event::MOUSE_OUT);


/* ATD::Mouse: */

ATD::Mouse::Mouse(ATD::Window *window)
	: Window::Observer()
	, m_position()
	, m_scroll()
	, m_buttonStates()
	, m_valid(true)
{
	for (unsigned sIter = 0; sIter <= static_cast<unsigned>(ButtonId::COUNT); 
			sIter++) {
		m_buttonStates.insert(std::pair<ButtonId, KeyState>(
					static_cast<ButtonId>(sIter), KeyState()));
	}

	Attach(window, EVENT_MASK);
}

const ATD::Vector2I &ATD::Mouse::Position() const
{ return m_position; }

const ATD::Vector2I &ATD::Mouse::Scroll() const
{ return m_scroll; }

const ATD::KeyState &ATD::Mouse::operator[](
		const ATD::Mouse::ButtonId &button) const
{ return m_buttonStates.at(button); }

bool ATD::Mouse::IsPressed(const ATD::Mouse::ButtonId &button) const
{ return m_buttonStates.at(button).IsPressed(); }

bool ATD::Mouse::IsValid() const
{ return m_valid; }

void ATD::Mouse::OnPollStart()
{
	/* Nullify scroll. */
	m_scroll = Vector2I();

	/* Update button states. */
	for (auto &statePair : m_buttonStates) { statePair.second.UpdatePrev(); }
}

void ATD::Mouse::OnNotify(const ATD::Window::Event &event)
{
	const Event *eventPtr = static_cast<const Event *>(&event);

	if (static_cast<uint32_t>(eventPtr->type) & POSITIONED_EVENT_MASK) {
		m_position = eventPtr->Position();
		if (eventPtr->type == Window::Event::MOUSE_PRESS || 
				eventPtr->type == Window::Event::MOUSE_RELEASE) {
			m_buttonStates.at(eventPtr->Button()).UpdateCurr(
					(eventPtr->type == Window::Event::MOUSE_PRESS));
		} else if (eventPtr->type == Window::Event::MOUSE_SCROLL) {
			m_scroll += eventPtr->Scroll();
		}
	} else {
		if (eventPtr->type == Window::Event::MOUSE_IN) {
			m_valid = true;
		} else { /* MOUSE_OUT */
			/* Reset all the button states. */
			for (auto &statePair : m_buttonStates) {
				statePair.second.UpdateCurr(false);
			}

			m_valid = false;
		}
	}
}


