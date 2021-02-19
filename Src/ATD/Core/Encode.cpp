/**
 * @file     
 * @brief    Encode wraps implementation.
 * @details  License: GPL v3.
 * @author   ArthurTheDigital (arthurthedigital@gmail.com)
 * @since    $Id: $ */

#include <ATD/Core/Encode.hpp>


/* ATD::Encode::Hex auxiliary */

static std::string _strToHex(const std::string &input, 
		bool lowercase)
{
	std::string result;
	for (size_t i = 0; i < input.size(); i++) {
		char high = static_cast<char>(
				static_cast<unsigned char>(input[i]) / 0x10);
		char low = static_cast<char>(
				static_cast<unsigned char>(input[i]) % 0x10);

		result.append(
				1, 
				high < 10 ? high + '0' : 
					lowercase ? high - 10 + 'a' : 
					high - 10 + 'A'
				);

		result.append(
				1, 
				low < 10 ? low + '0' : 
					lowercase ? low - 10 + 'a' : 
					low - 10 + 'A'
				);
	}
	return result;
}

static std::string _hexToStr(const std::string &input)
{
	std::string result;
	for (size_t i = 0; i + 1 < input.size(); i += 2) {
		char high = 
			(input[i] >= 'A' && input[i] <= 'F') ? input[i] - 'A' + 10 :
				(input[i] >= 'a' && input[i] <= 'f') ? input[i] - 'a' + 10 :
				input[i] - 0;

		char low = 
			(input[i + 1] >= 'A' && input[i + 1] <= 'F') ? 
				input[i + 1] - 'A' + 10 :
				(input[i + 1] >= 'a' && input[i + 1] <= 'f') ? 
				input[i + 1] - 'a' + 10 :
				input[i + 1] - 0;

		result.append(1, high * 0x80 + low);
	}
	return result;
}


/* ATD::Encode::Hex */

ATD::Encode::Hex::Hex(const std::string &content, 
		ATD::Encode::Mode mode, 
		bool lowercase)
	: m_str(mode == Mode::ENCODE ? _strToHex(content, lowercase) : 
			_hexToStr(content))
	, m_raw(content)
{}

std::string ATD::Encode::Hex::str() const
{
	return m_str;
}

std::string ATD::Encode::Hex::raw() const
{
	return m_raw;
}


/* ATD::Encode::CScreen constants */

const char ATD::Encode::CScreen::SCREEN_SYM = '\\';

/* first - unscreened
 * second - behind screen */
const std::vector<std::pair<char, char>> ATD::Encode::CScreen::SCREEN_RULES = {
	{'\\', '\\'}, 
	{'\"', '\"'}, 
	{' ', ' '}, 
	{'\t', 't'}, 
	{'\n', 'n'}, 
	{'\v', 'v'}, 
	{'\f', 'f'}, 
	{'\r', 'r'}, 
	{'\b', 'b'}, 
	{'\a', 'a'}
};


/* ATD::Encode::CScreen auxiliary */

static std::string _strToCScreen(const std::string &input)
{
	std::string result;
	for (size_t i = 0; i < input.size(); i++) {
		bool gotRule = false;
		for (auto &rule : ATD::Encode::CScreen::SCREEN_RULES) {
			if (rule.first == input[i]) {
				result.append(1, ATD::Encode::CScreen::SCREEN_SYM);
				result.append(1, rule.second);
				gotRule = true;
				break;
			}
		}
		if (!gotRule) {
			result.append(1, input[i]);
		}
	}
	return result;
}

static std::string _cScreenToStr(const std::string &input)
{
	std::string result;
	for (size_t i = 0; i < input.size();) {
		if (input[i] == ATD::Encode::CScreen::SCREEN_SYM) {
			if (i + 1 < input.size()) {
				bool gotRule = false;
				for (auto &rule : ATD::Encode::CScreen::SCREEN_RULES) {
					if (rule.second == input[i + 1]) {
						result.append(1, rule.first);
						gotRule = true;
						break;
					}
				}
				if (!gotRule) {
					result.append(1, input[i + 1]);
				}
				i += 2;
			} else {
				i++;
			}
		} else {
			result.append(1, input[i]);
			i++;
		}
	}
	return result;
}


/* ATD::Encode::CScreen */

ATD::Encode::CScreen::CScreen(const std::string &content, 
		ATD::Encode::Mode mode)
	: m_str(mode == Mode::ENCODE ? _strToCScreen(content) : 
			_cScreenToStr(content))
	, m_raw(content)
{}

std::string ATD::Encode::CScreen::str() const
{
	return m_str;
}

std::string ATD::Encode::CScreen::raw() const
{
	return m_raw;
}


/* ATD::Encode::Base64 auxiliary */

static char _bitsToB64(uint8_t bits)
{
	char bc = (bits < 0x1A) ? 'A' + bits : 
		(bits < 0x34) ? 'a' + (bits - 0x1A) : 
		(bits < 0x3E) ? '0' + (bits - 0x34) : 
		(bits == 0x3E) ? '+' : 
		'/';
	return bc;
}

static uint8_t _b64ToBits(char b64)
{
	uint8_t bits = (b64 >= 'A' && b64 <= 'Z') ? 0x00 + (b64 - 'A') : 
		(b64 >= 'a' && b64 <= 'z') ? 0x1A + (b64 - 'a') : 
		(b64 >= '0' && b64 <= '9') ? 0x34 + (b64 - '0') : 
		(b64 == '+') ? 0x3E : 
		0x3F;
	return bits;
}

static std::string _strToBase64(const std::string &str)
{
	std::string base64;
	for (size_t cIter = 0; cIter < str.size(); cIter += 3) {
		size_t bLen = 2;
		uint32_t bits = static_cast<uint32_t>(str[cIter + 0]) << 16;
		if (cIter + 1 < str.size()) {
			bLen += 1;
			bits += static_cast<uint32_t>(str[cIter + 1]) << 8;
			if (cIter + 2 < str.size()) {
				bLen += 1;
				bits += static_cast<uint32_t>(str[cIter + 2]);
			}
		}
		for (size_t bIter = 0; bIter < bLen; bIter++) {
			uint32_t bits6 = (bits >> ((3 - bIter) * 6)) & 0x0000003F;
			base64.append(1, _bitsToB64(static_cast<uint8_t>(bits6)));
		}
	}

	/* Base64 suffix */
	base64.append((3 - str.size() % 3) % 3, '=');

	return base64;
}

static std::string _base64ToStr(const std::string &base64)
{
	size_t suffixLen = 0;
	for (; base64[base64.size() - suffixLen - 1] == '='; suffixLen++) {}
	size_t baseSize = base64.size() - suffixLen;

	std::string result;
	for (size_t bIter = 0; bIter < baseSize; bIter += 4) {
		size_t cLen = 0;
		uint32_t bits = static_cast<uint32_t>(_b64ToBits(base64[bIter])) << 18;
		if (bIter + 1 < baseSize) {
			cLen += 1;
			bits += static_cast<uint32_t>(_b64ToBits(base64[bIter + 1])) << 12;
			if (bIter + 2 < baseSize) {
				cLen += 1;
				bits += static_cast<uint32_t>(
						_b64ToBits(base64[bIter + 2])) << 6;
				if (bIter + 3 < baseSize) {
					cLen += 1;
					bits += static_cast<uint32_t>(
							_b64ToBits(base64[bIter + 3]));
				}
			}
		}
		for (size_t cIter = 0; cIter < cLen; cIter++) {
			char c = static_cast<char>(
					static_cast<uint8_t>((bits >> ((2 - cIter) * 8)) & 
						0x000000FF));
			result.append(1, c);
		}
	}

	return result;
}


/* ATD::Encode::Base64 */

ATD::Encode::Base64::Base64(const std::string &content, 
		ATD::Encode::Mode mode)
	: m_str(mode == Mode::ENCODE ? _strToBase64(content) : _base64ToStr(content))
	, m_raw(content)
{}

std::string ATD::Encode::Base64::str() const
{
	return m_str;
}

std::string ATD::Encode::Base64::raw() const
{
	return m_raw;
}


