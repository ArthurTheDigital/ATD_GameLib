/**
 * @file     
 * @brief    Tag system.
 * @details  License: GPL v3.$
 * @author   ArthurTheDigital (arthurthedigital@gmail.com)
 * @since    $Id: $ */

#include <ATD/Core/Tag.hpp>

#include <string.h>


/* Tag constants */

const std::string ATD::Tag::DELIMITER = ", ";


/* Tag::Expression constants */

const std::array<std::string, static_cast<unsigned>(ATD::Tag::Expression::Operator::COUNT)> ATD::Tag::Expression::OPERATOR_STR = {
	"|", 
	"&", 
	"!"
};

const std::array<unsigned, static_cast<unsigned>(ATD::Tag::Expression::Operator::COUNT)> ATD::Tag::Expression::OPERATOR_ARGC = {
	2, 
	2, 
	1
};


/* Tag::Expression */

ATD::Tag::Expression::Expression(const char *expression)
	: m_oper(Operator::COUNT) /* If m_oper == Operator::COUNT, that means, 
							   * there is no operator, but tag only. */
	, m_args()
	, m_tag()
	, m_expressionLen(0)
{
	bool operMatch = false;

	/* Check, whether any of existing operators matches the beginning of the expression */
	for (unsigned operIndex = 0; operIndex < static_cast<unsigned>(Operator::COUNT); operIndex++) {
		if (std::string(expression, OPERATOR_STR[operIndex].size()) == OPERATOR_STR[operIndex]) {

			/* "operIndex" is a valid operator index here */
			m_oper = 
				operIndex == 0 ? Operator::ANY : 
				operIndex == 1 ? Operator::ALL :
				Operator::EXC;

			m_expressionLen = OPERATOR_STR[operIndex].size();
			for (unsigned arg = 0; arg < OPERATOR_ARGC[operIndex]; arg++) {
				m_args.push_back(
						Expression(
							&expression[
								m_expressionLen + 
								arg ? DELIMITER.size() : 0
								]
							)
						);

				m_expressionLen += 
					m_args.back().m_expressionLen + 
					arg ? DELIMITER.size() : 0;
			}

			operMatch = true;
			break;
		}
	}

	if (!operMatch) {
		/* Expression contains a single tag only */

		/* Try to find the first delimiter */
		for (const char *expDelimiter = expression; expDelimiter != nullptr; expDelimiter = ::strchr(expDelimiter, DELIMITER[0])) {
			if (::strncmp(expDelimiter, DELIMITER.c_str(), DELIMITER.size()) == 0) {
				/* Here "expDelimiter" is guaranteed to be greater os same as "expression" */
				m_tag = std::string(expression, static_cast<size_t>(expDelimiter - expression));
				break;
			}
		}

		if (!m_tag.size()) {
			m_tag = std::string(expression);
		}
	}
}

ATD::Tag::Expression::Expression(const std::string &expression)
	: Expression(expression.c_str())
{}

bool ATD::Tag::Expression::Check(const ATD::Tag &tag) const
{
	switch (m_oper) {
		case Operator::ANY:
			{
				return (m_args[0].Check(tag) || m_args[1].Check(tag));
			}
			break;

		case Operator::ALL:
			{
				return (m_args[0].Check(tag) && m_args[1].Check(tag));
			}
			break;

		case Operator::EXC:
			{
				return !(m_args[0].Check(tag));
			}
			break;

		default:
			{
				/* Single-tag */
				if (m_tag.size()) {
					auto tagIter = tag.m_tags.find(m_tag);
					return (tagIter != tag.m_tags.end());
				} else {
					return true;
				}
			}
	};
}

ATD::Tag::Expression::operator std::string() const
{
	if (m_oper == Operator::COUNT) {
		return m_tag;
	}

	std::string result = OPERATOR_STR[static_cast<unsigned>(m_oper)];
	for (auto argIter = m_args.begin(); argIter != m_args.end(); argIter++) {
		result += (argIter != m_args.begin() ? DELIMITER : 
				std::string()
				) + static_cast<std::string>(*argIter);
	}
	return result;
}


/* Tag */

ATD::Tag::Tag(const std::string &tags)
	: m_tags()
{
	if (tags.size()) {
		size_t cursor = 0;
		size_t delimiterPos = 0;

		while ((delimiterPos = tags.find(DELIMITER, cursor)) != std::string::npos) {
			m_tags.insert(tags.substr(cursor, delimiterPos - cursor));
			cursor = delimiterPos + DELIMITER.size();
		}
		m_tags.insert(tags.substr(cursor));
	}
}

ATD::Tag::operator std::string() const
{
	std::string result;
	for (auto tagIter = m_tags.begin(); tagIter != m_tags.end(); tagIter++) {
		result += (tagIter != m_tags.begin() ? DELIMITER : std::string()) + *tagIter;
	}
	return result;
}


