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
#include <ATD/Core/Unicode.hpp>

#include <string>


template<typename T>
std::string StrBits(const T &x)
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
		ATD::AutoTest tester("UTF8 1 byte:", utf1.c_str());
		ATD::Unicode unicode1(utf1);
		if (unicode1.Front() != uniVal1) {
			IPRINTF("", "Incorrect Unicode: %s instead of %s", 
					StrBits<uint32_t>(unicode1.Front()).c_str(), 
					StrBits<uint32_t>(uniVal1).c_str());

			tester.Finish(false);
		}
		IPRINTF("AUTOTEST", unicode1.Str().c_str());
	}

	{
		/* Unicode 00000429 in 00000080-000007FF */
		std::string utf2 = "Щ";
		uint32_t uniVal2 = 0x00000429;
		ATD::AutoTest tester("UTF8 2 bytes:", utf2.c_str());
		ATD::Unicode unicode2(utf2);
		if (unicode2.Front() != uniVal2) {
			IPRINTF("", "Incorrect Unicode: %s instead of %s", 
					StrBits<uint32_t>(unicode2.Front()).c_str(), 
					StrBits<uint32_t>(uniVal2).c_str());

			tester.Finish(false);
		}
		IPRINTF("AUTOTEST", unicode2.Str().c_str());

		IPRINTF("", "UTF: %s %s instead of %s %s", 
				StrBits<char>(unicode2.Str()[0]).c_str(), 
				StrBits<char>(unicode2.Str()[1]).c_str(), 
				StrBits<char>(utf2[0]).c_str(), 
				StrBits<char>(utf2[1]).c_str());
	}

	{
		/* Unicode 00002592 in 00000800-0000FFFF */
		std::string utf3 = "▒";
		uint32_t uniVal3 = 0x00002592;
		ATD::AutoTest tester("UTF8 3 bytes:", utf3.c_str());
		ATD::Unicode unicode3(utf3);
		if (unicode3.Front() != uniVal3) {
			IPRINTF("", "Incorrect Unicode: %s instead of %s", 
					StrBits<uint32_t>(unicode3.Front()).c_str(), 
					StrBits<uint32_t>(uniVal3).c_str());

			tester.Finish(false);
		}
		IPRINTF("AUTOTEST", unicode3.Str().c_str());
	}

	{
		/* Unicode 00010149 in 00010000-0010FFFF */
		std::string utf4 = "𐅉";
		uint32_t uniVal4 = 0x00010149;
		ATD::AutoTest tester("UTF8 4 bytes:", utf4.c_str());
		ATD::Unicode unicode4(utf4);
		if (unicode4.Front() != uniVal4) {
			IPRINTF("", "Incorrect Unicode: %s instead of %s", 
					StrBits<uint32_t>(unicode4.Front()).c_str(), 
					StrBits<uint32_t>(uniVal4).c_str());

			tester.Finish(false);
		}
		IPRINTF("AUTOTEST", unicode4.Str().c_str());
	}

	return 0;
}


