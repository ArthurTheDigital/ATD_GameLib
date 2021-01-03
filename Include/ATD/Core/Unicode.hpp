/**
 * @file     
 * @brief    Unicode processing.
 * @details  License: GPL v3.
 * @author   ArthurTheDigital (arthurthedigital@gmail.com)
 * @since    $Id: $ */

#pragma once

#include <ATD/Core/Vector2.hpp>

#include <string>
#include <vector>


namespace ATD {

// TODO: Redesign!

/**
 * @brief ...
 * @class ... */
class Unicode
{
public:
	typedef uint32_t Glyph;


	static const Glyph NEW_LINE;
	static const Glyph SPACE;

	static std::string StrFromGlyph(const Glyph &glyph);


	/**
	 * @brief ...
	 * @param str - utf8 string */
	Unicode(const std::string &str = std::string());

	Unicode(const Unicode &other);

	/**
	 * @brief Checks, whether the constructor passed with no errors.
	 * @throw If there were any errors.
	 *
	 * In most cases errors, while creating Unicode from UTF-8 string, shall 
	 * be ignored. That's why there is no such exception thrown in 
	 * constructor. */
	void CheckFailure() const;

	Glyph &operator[](size_t index);

	const Glyph &operator[](size_t index) const;

	Glyph &Front();

	const Glyph &Front() const;

	size_t Size() const;

	bool operator==(const Unicode &other) const;

	bool operator!=(const Unicode &other) const;

	bool operator<(const Unicode &other) const;

	bool operator>(const Unicode &other) const;

	Unicode &operator+=(const Unicode &other);

	Unicode operator+(const Unicode &other) const;

	Unicode &operator=(const Unicode &other);

	/**
	 * @brief Get the UTF-8 string.
	 * @return ... */
	std::string Str() const;

	/**
	 * @brief Get UTF-8 string, describing glyph with index.
	 * @param index - ...
	 * @return ... */
	std::string Str(size_t index) const;

	Unicode Copy(size_t first, size_t length) const;

	Unicode &Erase(size_t first, size_t length);

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


