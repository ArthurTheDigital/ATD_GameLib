/**
 * @file      
 * @brief     AutoTest class for debug.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3
 * @since     $Id: $ */

#include <ATD/Core/AutoTest.hpp>


/* Auxiliary */

/* TODO: Get terminal len as done in Console/Graphics */
static const size_t _TERMINAL_LEN = 80;
static const size_t _RESULT_LEN = 5;
static const size_t _INDENT_LEN = 4;
static const size_t _NAME_LEN = _TERMINAL_LEN - _RESULT_LEN - _INDENT_LEN;

static const size_t _MULTIDOT_LEN = 3;
static const size_t _SHORTENED_NAME_LEN = _NAME_LEN - _MULTIDOT_LEN;

static std::string _shortenName(const std::string &name)
{
	if (name.size() > _NAME_LEN) {
		std::string shortened = std::string(_MULTIDOT_LEN, '.') + 
			name.substr(name.size() - _SHORTENED_NAME_LEN);

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
	, m_name(_shortenName(name))
	, m_control(control)
	, m_test()
	, m_failureExpected(failureExpected)
	, m_finished(false)
{
	if (m_control.size()) {
		attach(&debug, Debug::Level::DBGA, Tag::Expression("AUTOTEST"));
	}
	::fprintf(stdout, "\x1b[33m%s%s\x1b[0m", 
			m_name.c_str(), 
			std::string(_TERMINAL_LEN - m_name.size(), ' ').c_str()
			);
	IPRINTF("AUTOTEST_IMPL", "Autotest \"%s\" started", m_name.c_str());
}

ATD::AutoTest::~AutoTest()
{
	finish();
}

void ATD::AutoTest::onNotify(const Debug::Line &line)
{
	/* If there is no control string -> no comparison -> no need to stash 
	 * data inside test string */
	if (m_control.size()) {
		m_test += line.line;
	}
}

void ATD::AutoTest::finish(bool pass)
{
	if (!m_finished) {
		/* Goes first, so no exception can hit it */
		m_finished = true;

		/* The check, whether was attached is already included in "detach()" */
		detach();

		/* Check the test/control strings match */
		if (pass && m_control.size()) {
			pass = (m_control == m_test);
		}

		/* Print colourful result */
		std::string indent(_INDENT_LEN, ' ');
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

		IPRINTF("AUTOTEST_IMPL", "Autotest \"%s\" finished: %s", 
				m_name.c_str(), verdict.c_str());
	}
}


