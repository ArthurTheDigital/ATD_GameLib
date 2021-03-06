/**
 * @file      
 * @brief     Terminal ansi keyboard.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <termios.h>

#include <map>
#include <memory>
#include <set>


namespace ATD {

namespace Ansi {

enum class Key
{
	NONE, 
	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, 
	UP, DOWN, RIGHT, LEFT, 
	ENTER, SPACE, PLUS, MINUS, 
	ERASE
};

/**
 * @brief ...
 * @class ... */
class Keyboard
{
public:
	typedef std::shared_ptr<const Keyboard> CPtr;

	static const std::map<Key, std::string> KEY_MAP;
	static const std::set<Key> ALL_KEYS;

	/**
	 * @brief ...
	 * @param keys - ... */
	Keyboard(const std::set<Key> &keys = ALL_KEYS);

	/**
	 * @brief ... */
	~Keyboard();

	/**
	 * @brief Set the keys to be expected
	 * @param keys - ... */
	void setFilter(const std::set<Key> &keys);

	/**
	 * @brief Get one key pressed
	 * @return ...
	 *
	 * Replaces 'm_unparsedInput' with new one */
	std::unique_ptr<Key> get() const;

	/**
	 * @brief ...
	 * @return ... */
	std::string getUnparsed() const;

private:
	struct termios m_termOld; /* Old terminal settings */
	int m_flagOld; /* Fd flags old */

	std::map<Key, std::string> m_keys; /* Expected keys */

	mutable std::string m_unparsedInput;
	mutable std::string m_input;
};

} /* namespace Ansi */

} /* namespace ATD */


