/**
@file     
@brief    A test, ensuring, that UTF8 parsing is correct.
@details  License: GPL v3.
@author   ArthurTheDigital (arthurthedigital@gmail.com)
@since    $Id: $
*/

#include <ATD/Core/AutoTest.hpp>
#include <ATD/Core/Debug.hpp>
#include <ATD/Core/ErrWriter.hpp>
#include <ATD/Core/Printf.hpp>
#include <ATD/Core/Unicode.hpp>

#include <string>


template<typename T>
std::string bitsToStr(const T &x)
{
	std::string str = "0x";
	for (size_t bRIndex = 0; bRIndex < sizeof(T) * 8; bRIndex++) {
		const size_t bIndex = sizeof(T) * 8 - 1 - bRIndex;
		str += (x & (1 << bIndex)) ? "1" : "0";
	}
	return str;
}

int main()
{
	/* ATD::ErrWriter err; // DEBUG */

	{
		/* Unicode 00000041 in 00000000-0000007F */
		std::string utf1 = "A";
		uint32_t uniVal1 = 0x00000041;
		ATD::AutoTest tester("UTF8 1 byte:", utf1);
		ATD::Unicode unicode1(utf1);
		if (unicode1.front() != uniVal1) {
			IPRINTF("", "Incorrect Unicode: %s instead of %s", 
					bitsToStr<uint32_t>(unicode1.front()).c_str(), 
					bitsToStr<uint32_t>(uniVal1).c_str());

			tester.finish(false);
		}
		IPRINTF("AUTOTEST", unicode1.str().c_str());
	}

	{
		/* Unicode 00000429 in 00000080-000007FF */
		std::string utf2 = "Щ";
		uint32_t uniVal2 = 0x00000429;
		ATD::AutoTest tester("UTF8 2 bytes:", utf2);
		ATD::Unicode unicode2(utf2);
		if (unicode2.front() != uniVal2) {
			IPRINTF("", "Incorrect Unicode: %s instead of %s", 
					bitsToStr<uint32_t>(unicode2.front()).c_str(), 
					bitsToStr<uint32_t>(uniVal2).c_str());

			tester.finish(false);
		}
		IPRINTF("AUTOTEST", unicode2.str().c_str());

		/* IPRINTF("", "UTF: %s %s instead of %s %s", 
				bitsToStr<char>(unicode2.str()[0]).c_str(), 
				bitsToStr<char>(unicode2.str()[1]).c_str(), 
				bitsToStr<char>(utf2[0]).c_str(), 
				bitsToStr<char>(utf2[1]).c_str()); // DEBUG */
	}

	{
		/* Unicode 00002592 in 00000800-0000FFFF */
		std::string utf3 = "▒";
		uint32_t uniVal3 = 0x00002592;
		ATD::AutoTest tester("UTF8 3 bytes:", utf3);
		ATD::Unicode unicode3(utf3);
		if (unicode3.front() != uniVal3) {
			IPRINTF("", "Incorrect Unicode: %s instead of %s", 
					bitsToStr<uint32_t>(unicode3.front()).c_str(), 
					bitsToStr<uint32_t>(uniVal3).c_str());

			tester.finish(false);
		}
		IPRINTF("AUTOTEST", unicode3.str().c_str());
	}

	{
		/* Unicode 00010149 in 00010000-0010FFFF */
		std::string utf4 = "𐅉";
		uint32_t uniVal4 = 0x00010149;
		ATD::AutoTest tester("UTF8 4 bytes:", utf4);
		ATD::Unicode unicode4(utf4);
		if (unicode4.front() != uniVal4) {
			IPRINTF("", "Incorrect Unicode: %s instead of %s", 
					bitsToStr<uint32_t>(unicode4.front()).c_str(), 
					bitsToStr<uint32_t>(uniVal4).c_str());

			tester.finish(false);
		}
		IPRINTF("AUTOTEST", unicode4.str().c_str());
	}

	{
		/* Unicode from Utf8 and back. */
		std::string utf = "ЩЕГОЛ";
		ATD::AutoTest tester("Unicode string from UTF8 and back:", utf);
		ATD::Unicode unicode(utf);
		std::string utf2 = unicode.str();
		IPRINTF("AUTOTEST", utf2.c_str());
	}

	{
		/* Unicode size. */
		std::string utf = "ЩЕГОЛ";
		ATD::AutoTest tester("Unicode size:", ATD::Aux::printf("%d", 5));
		ATD::Unicode unicode(utf);
		IPRINTF("AUTOTEST", "%d", unicode.size());
	}

	return 0;
}


