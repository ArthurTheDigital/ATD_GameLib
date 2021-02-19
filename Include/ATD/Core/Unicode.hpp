/**
 * @file      
 * @brief     Unicode processing.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Vector2.hpp>

#include <string>
#include <vector>


namespace ATD {

/**
 * @brief ...
 * @class ... */
class Unicode
{
public:
	typedef uint32_t Glyph;


	static const Glyph NEW_LINE;
	static const Glyph SPACE;

	static std::string strFromGlyph(const Glyph &glyph);


	/**
	 * @brief ...
	 * @param str - utf8 string */
	Unicode(const std::string &str = std::string());

	/**
	 * @brief ...
	 * @param other - ... */
	Unicode(const Unicode &other);

	/**
	 * @brief Checks, whether the constructor passed with no errors.
	 * @throw If there were any errors.
	 *
	 * In most cases errors, while creating Unicode from UTF-8 string, shall 
	 * be ignored. That's why there is no such exception thrown in 
	 * constructor. */
	void checkFailure() const;

	/**
	 * @brief ...
	 * @param index - ...
	 * @return ... */
	Glyph &operator[](size_t index);

	/**
	 * @brief ...
	 * @param index - ...
	 * @return ... */
	const Glyph &operator[](size_t index) const;

	/**
	 * @brief ...
	 * @return ... */
	Glyph &front();

	/**
	 * @brief ...
	 * @return ... */
	const Glyph &front() const;

	/**
	 * @brief ...
	 * @return ... */
	size_t size() const;

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	bool operator==(const Unicode &other) const;

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	bool operator!=(const Unicode &other) const;

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	bool operator<(const Unicode &other) const;

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	bool operator>(const Unicode &other) const;

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	Unicode &operator+=(const Unicode &other);

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	Unicode operator+(const Unicode &other) const;

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	Unicode &operator=(const Unicode &other);

	/**
	 * @brief Get the UTF-8 string.
	 * @return ... */
	std::string str() const;

	/**
	 * @brief Get UTF-8 string, describing glyph with index.
	 * @param index - ...
	 * @return ... */
	std::string str(size_t index) const;

	/**
	 * @brief ...
	 * @param first  - ...
	 * @param length - ...
	 * @return ... */
	Unicode copy(size_t first, size_t length) const;

	/**
	 * @brief ...
	 * @param first  - ...
	 * @param length - ...
	 * @return ... */
	Unicode &erase(size_t first, size_t length);

protected:
	/**
	 * @brief ...
	 * @param ... */
	Unicode(bool fail, const std::basic_string<Glyph> &glyphs);


	bool m_fail;
	std::basic_string<Glyph> m_glyphs;
	/* FIXME: Shall I cache UTF-8 string, once Str() method is called? */
};

} /* namespace ATD */


