/**
* @file     
* @brief    AutoTest class for debug.
* @details  License: GPL v3.
* @author   ArthurTheDigital (arthurthedigital@gmail.com)
* @since    $Id: $ */

#include <ATD/Core/AutoTest.hpp>


/* Auxiliary */

/* TODO: Get terminal len as done in Console/Graphics */
static const size_t TERMINAL_LEN = 80;
static const size_t RESULT_LEN = 5;
static const size_t INDENT_LEN = 4;
static const size_t NAME_LEN = TERMINAL_LEN - RESULT_LEN - INDENT_LEN;

static const size_t MULDOT_LEN = 3;
static const size_t SHORTENED_NAME_LEN = NAME_LEN - MULDOT_LEN;

static std::string ShortenName(const std::string &name)
{
	if (name.size() > NAME_LEN) {
		std::string shortened = std::string(MULDOT_LEN, '.') + name.substr(name.size() - SHORTENED_NAME_LEN);
		return shortened;
	} else {
		return name;
	}
}


/* AutoTest */

ATD::AutoTest::AutoTest(const std::string &name, 
		const std::string &control, 
		bool failureExpected)
	: Debug::Observer()
	, m_name(ShortenName(name))
	, m_control(control)
	, m_test()
	, m_failureExpected(failureExpected)
	, m_finished(false)
{
	if (m_control.size()) {
		Attach(&debug, Debug::Level::DBGA, Tag::Expression("AUTOTEST"));
	}
	::fprintf(stdout, "\x1b[33m%s%s\x1b[0m", 
			m_name.c_str(), 
			std::string(TERMINAL_LEN - m_name.size(), ' ').c_str()
			);
	IPRINTF("AUTOTEST_IMPL", "Autotest \"%s\" started", m_name.c_str());
}

ATD::AutoTest::~AutoTest()
{
	Finish();
}

void ATD::AutoTest::OnNotify(const Debug::Line &line)
{
	/* If there is no control string -> no comparison -> no need to stash 
	 * data inside test string */
	if (m_control.size()) {
		m_test += line.line;
	}
}

void ATD::AutoTest::Finish(bool pass)
{
	if (!m_finished) {
		/* Goes first, so no exception can hit it */
		m_finished = true;

		/* The check, whether was Attached is already included in "Detach()" */
		Detach();

		/* Check the test/control strings match */
		if (pass && m_control.size()) {
			pass = (m_control == m_test);
		}

		/* Print colourful result */
		std::string indent(INDENT_LEN, ' ');
		std::string verdict = 
			m_failureExpected ? 
			pass ? "XPASS" : "XFAIL" : 
			pass ? " PASS" : " FAIL";

		std::string verdictColoured = 
			m_failureExpected ?
			pass ? "\x1b[35mXPASS\x1b[0m" : "\x1b[32mXFAIL\x1b[0m" : 
			pass ? "\x1b[32m PASS\x1b[0m" : "\x1b[31m FAIL\x1b[0m";

		::fprintf(stdout, "%s%s\n", indent.c_str(), verdictColoured.c_str());

		/* Clear strings to save memory (Remember, this may execute far 
		 * before the destructor) */
		m_control.clear();
		m_test.clear();

		IPRINTF("AUTOTEST_IMPL", "Autotest \"%s\" finished: %s", m_name.c_str(), verdict.c_str());
	}
}


