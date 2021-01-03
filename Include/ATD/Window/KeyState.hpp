/**
 * */

#pragma once


namespace ATD {

class KeyState {
public:
	inline KeyState()
		: m_wasPressed(false)
		, m_isPressed(false)
	{}

	inline KeyState(bool wasPressed, bool isPressed)
		: m_wasPressed(wasPressed)
		, m_isPressed(isPressed)
	{}

	inline bool IsPressed() const
	{ return m_isPressed; }

	inline bool WasPressed() const
	{ return m_wasPressed; }

	inline bool IsHeldStart() const
	{ return (!m_wasPressed && m_isPressed); }

	inline bool IsHeld() const
	{ return (m_wasPressed && m_isPressed); }

	inline bool IsReleased() const
	{ return (m_wasPressed && !m_isPressed); }

	/**
	 * @brief Update previous state only (based on current). */
	inline void UpdatePrev()
	{ m_wasPressed = m_isPressed; }

	/**
	 * @brief Update current state only.
	 * @param isPressed - ... */
	inline void UpdateCurr(bool isPressed)
	{ m_isPressed = isPressed; }

private:
	bool m_wasPressed;
	bool m_isPressed;
};

}


