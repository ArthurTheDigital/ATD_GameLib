/**
 * @file      
 * @brief     Encode wraps.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <string>
#include <utility>
#include <vector>


namespace ATD {
namespace Encode {

/**
 * @brief ...
 * @class ... */
enum class Mode {
	ENCODE, 
	DECODE
};

/**
 * @brief ...
 * @class ... */
class Hex
{
public:
	/**
	 * @brief ...
	 * @param content   - string to encode/decode
	 * @param mode      - whether to encode or decode string
	 * @param lowercase - set true for lowercase hex letters
	 *
	 * If decoding from uneven hex string, last symbol is ignored. */
	Hex(const std::string &content, 
			Mode mode = Mode::ENCODE, 
			bool lowercase = false);

	/**
	 * @brief ...
	 * @return modifyed string */
	std::string str() const;

	/**
	 * @brief ...
	 * @return unmodifyed string */
	std::string raw() const;

private:
	std::string m_str;
	std::string m_raw;
};

/**
 * @brief ...
 * @class ... */
class CScreen
{
public:
	static const char SCREEN_SYM;
	static const std::vector<std::pair<char, char>> SCREEN_RULES;

	/**
	 * @brief ...
	 * @param content   - string to encode/decode
	 * @param mode      - whether to encode or decode string
	 *
	 * ... */
	CScreen(const std::string &content, 
			Mode mode = Mode::ENCODE);

	/**
	 * @brief ...
	 * @return modifyed string */
	std::string str() const;

	/**
	 * @brief ...
	 * @return unmodifyed string */
	std::string raw() const;

private:
	std::string m_str;
	std::string m_raw;
};

/**
 * @brief ...
 * @class ... */
class Base64
{
public:
	/**
	 * @brief ...
	 * @param content - ...
	 * @param mode    - ...
	 *
	 * ... */
	Base64(const std::string &content, 
			Mode mode = Mode::ENCODE);

	/**
	 * @brief ...
	 * @return modifyed string */
	std::string str() const;

	/**
	 * @brief ...
	 * @return unmodifyed string */
	std::string raw() const;

private:
	std::string m_str;
	std::string m_raw;
};

} /* namespace Encode */
} /* namespace ATD */


