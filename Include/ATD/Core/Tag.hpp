/**
 * @file     
 * @brief    Tag system.
 * @details  License: GPL v3.$
 * @author   ArthurTheDigital (arthurthedigital@gmail.com)
 * @since    $Id: $ */

#pragma once

#include <array>
#include <set>
#include <string>
#include <vector>


namespace ATD {

/**
 * @class Tag
 * @brief A set of tags
 *
 * Tags are std::strings, but 
 * - without ", " inside of them, 
 * - not starting with "!" or "|" or "&".
 * */
class Tag
{
public:
	static const std::string DELIMITER;

	/**
	 * @class ...
	 * @brief ... */
	class Expression
	{
	public:

		/**
		 * @brief ... */
		enum class Operator
		{
			ANY,  /* || */
			ALL,  /* && */
			EXC,  /* !  'EXC' - for Exclude */
			COUNT /* Not a valid operator, but the number of them. */
		};

		static const std::array<std::string, static_cast<unsigned>(Operator::COUNT)> OPERATOR_STR;

		static const std::array<unsigned, static_cast<unsigned>(Operator::COUNT)> OPERATOR_ARGC;

		/**
		 * @brief ...
		 * @param expression - tag expression in reverse Polish notation
		 *
		 * Examples:
		 * &&tag1, tag2, tag3 
		 * ||tag1, tag2, |tag3, tag4 */
		Expression(const char *expression = "\0");

		/**
		 * @brief ...
		 * @param expression - ... */
		Expression(const std::string &expression);

		/**
		 * @brief ... */
		virtual ~Expression() = default;

		/**
		 * @brief Check the tags with expression
		 * @param tag - ...
		 * @return true, if matches, false otherwise
		 *
		 * Note: Empty expression matchs any tag. */
		bool Check(const Tag &tag) const;

		/**
		 * @brief ... */
		virtual operator std::string() const;

	private:
		/* Either has operator+args or tag. Never both. */

		Operator m_oper;
		std::vector<Expression> m_args;

		std::string m_tag;

		/* Parsing optimisation. */
		size_t m_expressionLen;
	};

	/**
	 * @brief ...
	 * @param tags - a set of tags, separated by ", " */
	Tag(const std::string &tags = std::string());

	/**
	 * @brief ... */
	virtual ~Tag() = default;

	/**
	 * @brief ... */
	virtual operator std::string() const;

private:
	std::set<std::string> m_tags;

friend bool Expression::Check(const Tag &tag) const;
};

} /* namespace ATD */


