/**
 * @file      
 * @brief     ErrWriter class for debug.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3
 * @since     $Id: $ */

#include <ATD/Core/ErrWriter.hpp>

#include <stdio.h>

#include <iomanip> /* std::put_time */
#include <sstream>


/* ATD::ErrWriter constants: */

#if defined _WIN32
const std::string ATD::ErrWriter::LINE_BREAKER = "\n\r";
#else
const std::string ATD::ErrWriter::LINE_BREAKER = "\n";
#endif


/* ATD::ErrWriter: */

ATD::ErrWriter::ErrWriter()
	: Debug::Observer()
{ attach(&debug); }

void ATD::ErrWriter::onNotify(const Debug::Line &line)
{
	try {
		/* Print line to a single string */
		std::time_t lineTimeT = std::chrono::system_clock::to_time_t(
				line.time);

		std::ostringstream oss;
		oss << std::put_time(
				std::localtime(&lineTimeT), 
				"%Y.%m.%d %H:%M:%S" /* System clock precision is seconds only
									 * FIXME: How do I get milliseconds? */
				) << " " << 
			line.file << ":" << 
			line.lnNumber << ":" << 
			line.function << " - " << 
			std::string(
				line.level == Debug::Level::ERRO ? "ERRO" : 
				line.level == Debug::Level::WARN ? "WARN" : 
				line.level == Debug::Level::INFO ? "INFO" : 
				line.level == Debug::Level::DEBG ? "DEBG" : 
				line.level == Debug::Level::DBG1 ? "DBG1" : 
				line.level == Debug::Level::DBG2 ? "DBG2" : 
				line.level == Debug::Level::DBG3 ? "DBG3" : 
				line.level == Debug::Level::DBG4 ? "DBG4" : 
				line.level == Debug::Level::DBG5 ? "DBG5" : 
				line.level == Debug::Level::DBG6 ? "DBG6" : 
				line.level == Debug::Level::DBG7 ? "DBG7" : 
				line.level == Debug::Level::DBG8 ? "DBG8" : 
				line.level == Debug::Level::DBG9 ? "DBG9" : 
				line.level == Debug::Level::DBGA ? "DBGA" : 
				"????"
				) << 
			"[" << static_cast<std::string>(line.tag) << "] - " <<
			line.line << 
			LINE_BREAKER;

		::fprintf(stderr, "%s", oss.str().c_str());
	} catch (const std::exception &e_err) {
		/* If any error happened, LogWriter shall detach()!
		 *
		 * Because, otherwise, an error during debug output will invoke 
		 * extra debug output, and trying to process that, LogWriter will 
		 * definetely face the SAME error.
		 * Endless loop. */
		detach();
		::fprintf(stderr, "CRITICAL ERROR: \"LogWriter\" failure: %s\n", e_err.what());
	} catch (...) {
		detach();
		::fprintf(stderr, "CRITICAL ERROR: \"LogWriter\" failure: ???\n");
	}
}


