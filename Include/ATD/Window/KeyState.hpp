/**
 * @file      
 * @brief     Wrap around button state.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once


namespace ATD {

/**
 * @brief ...
 * @class ... */
class KeyState {
public:
	/**
	 * @brief ... */
	inline KeyState()
		: m_wasPressed(false)
		, m_isPressed(false)
	{}

	/**
	 * @brief ...
	 * @param wasPressed - ...
	 * @param isPressed  - ... */
	inline KeyState(bool wasPressed, bool isPressed)
		: m_wasPressed(wasPressed)
		, m_isPressed(isPressed)
	{}

	/**
	 * @brief ...
	 * @param other - ... */
	inline KeyState(const KeyState &other)
		: m_wasPressed(other.m_wasPressed)
		, m_isPressed(other.m_isPressed)
	{}

	/**
	 * @brief ...
	 * @return ... */
	inline bool isPressed() const
	{ return m_isPressed; }

	/**
	 * @brief ...
	 * @return ... */
	inline bool wasPressed() const
	{ return m_wasPressed; }

	/**
	 * @brief ...
	 * @return ... */
	inline bool isHeldStart() const
	{ return (!m_wasPressed && m_isPressed); }

	/**
	 * @brief ...
	 * @return ... */
	inline bool isHeld() const
	{ return (m_wasPressed && m_isPressed); }

	/**
	 * @brief ...
	 * @return ... */
	inline bool isReleased() const
	{ return (m_wasPressed && !m_isPressed); }

	/**
	 * @brief ...
	 * @return ... */
	inline bool isIdle() const
	{ return (!m_wasPressed && !m_isPressed); }

	/**
	 * @brief Update previous state only (based on current). */
	inline void updatePrev()
	{ m_wasPressed = m_isPressed; }

	/**
	 * @brief Update current state only.
	 * @param isPressed - ... */
	inline void updateCurr(bool isPressed)
	{ m_isPressed = isPressed; }

private:
	bool m_wasPressed;
	bool m_isPressed;
};

} /* namespace ATD */


