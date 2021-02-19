/**
 * @file      
 * @brief     Terminal ansi keyboard.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Ansi/Keyboard.hpp>

#include <ATD/Core/Printf.hpp>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <stdexcept>


/* ATD::Ansi::Keyboard constants */

const std::map<ATD::Ansi::Key, std::string> ATD::Ansi::Keyboard::KEY_MAP = {
	{ATD::Ansi::Key::A,     "a"}, 
	{ATD::Ansi::Key::B,     "b"}, 
	{ATD::Ansi::Key::C,     "c"}, 
	{ATD::Ansi::Key::D,     "d"}, 
	{ATD::Ansi::Key::E,     "e"}, 
	{ATD::Ansi::Key::F,     "f"}, 
	{ATD::Ansi::Key::G,     "g"}, 
	{ATD::Ansi::Key::H,     "h"}, 
	{ATD::Ansi::Key::I,     "i"}, 
	{ATD::Ansi::Key::J,     "j"}, 
	{ATD::Ansi::Key::K,     "k"}, 
	{ATD::Ansi::Key::L,     "l"}, 
	{ATD::Ansi::Key::M,     "m"}, 
	{ATD::Ansi::Key::N,     "n"}, 
	{ATD::Ansi::Key::O,     "o"}, 
	{ATD::Ansi::Key::P,     "p"}, 
	{ATD::Ansi::Key::Q,     "q"}, 
	{ATD::Ansi::Key::R,     "r"}, 
	{ATD::Ansi::Key::S,     "s"}, 
	{ATD::Ansi::Key::T,     "t"}, 
	{ATD::Ansi::Key::U,     "u"}, 
	{ATD::Ansi::Key::V,     "v"}, 
	{ATD::Ansi::Key::W,     "w"}, 
	{ATD::Ansi::Key::X,     "x"}, 
	{ATD::Ansi::Key::Y,     "y"}, 
	{ATD::Ansi::Key::Z,     "z"}, 
	{ATD::Ansi::Key::UP,    "\033[A"}, 
	{ATD::Ansi::Key::DOWN,  "\033[B"}, 
	{ATD::Ansi::Key::RIGHT, "\033[C"}, 
	{ATD::Ansi::Key::LEFT,  "\033[D"}, 
	{ATD::Ansi::Key::ENTER, "\n"}, 
	{ATD::Ansi::Key::SPACE, " "}, 
	{ATD::Ansi::Key::PLUS,  "+"}, 
	{ATD::Ansi::Key::MINUS, "-"}, 
	{ATD::Ansi::Key::ERASE, "\x7f"}
};

const std::set<ATD::Ansi::Key> ATD::Ansi::Keyboard::ALL_KEYS = {
	ATD::Ansi::Key::A, 
	ATD::Ansi::Key::B, 
	ATD::Ansi::Key::C, 
	ATD::Ansi::Key::D, 
	ATD::Ansi::Key::E, 
	ATD::Ansi::Key::F, 
	ATD::Ansi::Key::G, 
	ATD::Ansi::Key::H, 
	ATD::Ansi::Key::I, 
	ATD::Ansi::Key::J, 
	ATD::Ansi::Key::K, 
	ATD::Ansi::Key::L, 
	ATD::Ansi::Key::M, 
	ATD::Ansi::Key::N, 
	ATD::Ansi::Key::O, 
	ATD::Ansi::Key::P, 
	ATD::Ansi::Key::Q, 
	ATD::Ansi::Key::R, 
	ATD::Ansi::Key::S, 
	ATD::Ansi::Key::T, 
	ATD::Ansi::Key::U, 
	ATD::Ansi::Key::V, 
	ATD::Ansi::Key::W, 
	ATD::Ansi::Key::X, 
	ATD::Ansi::Key::Y, 
	ATD::Ansi::Key::Z, 
	ATD::Ansi::Key::UP, 
	ATD::Ansi::Key::DOWN, 
	ATD::Ansi::Key::RIGHT, 
	ATD::Ansi::Key::LEFT, 
	ATD::Ansi::Key::ENTER, 
	ATD::Ansi::Key::SPACE, 
	ATD::Ansi::Key::PLUS, 
	ATD::Ansi::Key::MINUS, 
	ATD::Ansi::Key::ERASE
};


/* ATD::Ansi::Keyboard */

ATD::Ansi::Keyboard::Keyboard(const std::set<Key> &keys)
	: m_termOld()
	, m_flagOld()
	, m_keys()
	, m_unparsedInput()
	, m_input()
{
	/* Set terminal settings */
	struct termios termNew;

	if (::tcgetattr(::fileno(stdin), &m_termOld) == -1) {
		int errnoVal = errno;
		throw std::runtime_error(
				Aux::printf(
					"'tcgetattr(fileno(stdin), ..)' failure: %d %s", 
					errnoVal, ::strerror(errnoVal)
					));
	}

	if (::tcgetattr(::fileno(stdin), &termNew) == -1) {
		int errnoVal = errno;
		throw std::runtime_error(
				Aux::printf(
					"'tcgetattr(fileno(stdin), ..)' failure: %d %s", 
					errnoVal, ::strerror(errnoVal)
					));
	}

	termNew.c_lflag &= ~ICANON & ~ECHO;

	if (::tcsetattr(::fileno(stdin), TCSANOW, &termNew) == -1) {
		int errnoVal = errno;
		throw std::runtime_error(
				Aux::printf(
					"'tcsetattr(fileno(stdin), ..)' failure: %d %s", 
					errnoVal, ::strerror(errnoVal)
					));
	}

	/* Set fd flags */
	if ((m_flagOld = ::fcntl(::fileno(stdin), F_GETFL)) == -1) {
		int errnoVal = errno;
		throw std::runtime_error(
				Aux::printf(
					"'fcntl(fileno(stdin), F_GETFL, ..)' failure: %d %s", 
					errnoVal, ::strerror(errnoVal)
					));
	}

	if (::fcntl(::fileno(stdin), F_SETFL, m_flagOld | O_NONBLOCK) == -1) {
		int errnoVal = errno;
		throw std::runtime_error(
				Aux::printf(
					"'fcntl(fileno(stdin), F_SETFL, .. | O_NONBLOCK)' failure: %d %s", 
					errnoVal, ::strerror(errnoVal)
					));
	}

	/* Remember the optimized keys */
	for (auto &key : keys) {
		m_keys.insert(std::pair<Key, std::string>(key, KEY_MAP.at(key)));
	}
}

ATD::Ansi::Keyboard::~Keyboard()
{
	::tcsetattr(::fileno(stdin), TCSAFLUSH, &m_termOld);
	::fcntl(::fileno(stdin), F_SETFL, m_flagOld);
}

void ATD::Ansi::Keyboard::setFilter(const std::set<Key> &keys)
{
	std::map<Key, std::string> n_keys;
	for (auto &key : keys) {
		n_keys.insert(std::pair<Key, std::string>(key, KEY_MAP.at(key)));
	}

	m_keys = n_keys;
}

std::unique_ptr<ATD::Ansi::Key> ATD::Ansi::Keyboard::get() const
{
	/* Note:
	 * The great assumption here: key string codes do not come separated. */
	int c;
	while (1) {
		if ((c = ::fgetc_unlocked(stdin)) != EOF) {
			m_input.append(1, static_cast<char>(c));
		} else {
			int errnoVal = errno;
			if (errno == EAGAIN) {
				/* FIXME: ::clearerr_unlocked()? */
				break;
			} else {
				throw std::runtime_error(
						Aux::printf(
							"'fgetc(stdin)' failure: %d %s", 
							errnoVal, ::strerror(errno)
							));
			}
		}
	}

	m_unparsedInput = "";
	for (size_t inIter = 0; inIter < m_input.size(); inIter++) {
		for (auto knIter = m_keys.begin(); knIter != m_keys.end(); knIter++) {
			if (m_input.compare(inIter, knIter->second.size(), knIter->second) == 0) {
				m_input = m_input.substr(inIter + knIter->second.size());
				return std::unique_ptr<Key>(new Key(knIter->first));
			}
		}
		/* Nothing matches at this iterator, skip one symbol */
		m_unparsedInput += std::string(1, m_input[inIter]);
	}
	m_input = "";
	return std::unique_ptr<Key>(nullptr);
}

std::string ATD::Ansi::Keyboard::getUnparsed() const
{
	std::string result = m_unparsedInput;
	m_unparsedInput = "";
	return result;
}


