/**
 * @file      
 * @brief     Debug class for debug + macros.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Tag.hpp>

#include <chrono>
#include <map>
#include <mutex>


namespace ATD {

/**
 * @class Debug.
 * @brief ... */
class Debug
{
public:
	/**
	 * @class Level.
	 * @brief Debug level. */
	enum class Level
	{
		ERRO = 0x00, /* Error */
		WARN = 0x01, /* Warning */
		INFO = 0x02, /* Info */
		DEBG = 0x10, /* Basic debug level */
		DBG1 = 0x11, 
		DBG2 = 0x12, 
		DBG3 = 0x13, 
		DBG4 = 0x14, 
		DBG5 = 0x15, 
		DBG6 = 0x16, 
		DBG7 = 0x17, 
		DBG8 = 0x18, 
		DBG9 = 0x19, 
		DBGA = 0x1A
	};

	/**
	 * @class Line
	 * @brief ... */
	struct Line
	{
	public:
		/**
		 * @brief ...
		 * @param n_level    - ...
		 * @param n_tag      - ...
		 * @param n_time     - ...
		 * @param n_file     - source file, where debug output invoked
		 * @param n_function - "pretty function", where debug output invoked
		 * @param n_lnNumber - line number, where debug output invoked
		 * @param n_line     - debug payload */
		inline Line(Level n_level, 
				const Tag &n_tag, 
				const std::chrono::system_clock::time_point &n_time, 
				const std::string &n_file, 
				const std::string &n_function, 
				unsigned n_lnNumber, 
				const std::string &n_line)
			: level(n_level)
			, tag(n_tag)
			, time(n_time)
			, file(n_file)
			, function(n_function)
			, lnNumber(n_lnNumber)
			, line(n_line)
		{}


		Level level;
		Tag tag;

		std::chrono::system_clock::time_point time;
		std::string file;
		std::string function;
		unsigned lnNumber;

		std::string line;
	};

	/**
	 * @class Observer
	 * @brief Debug observer interface
	 *
	 * I think, it is a good base for writing excellent debugging tools.
	 *
	 * Example:
	 * 1. I need to print debug to file.log - 
	 * I create "class LogWriter : public Debug::Observer", attach() it. 
	 * It gets the Line structure, formats it into a string and writes it.
	 *
	 * 2. Now I need a class "AutoTest : public Debug::Observer" , which stacks 
	 * all the debug output with the "AUTOTEST" tag into a single line and 
	 * then compares it to the control line. Despite it gets the whole Line 
	 * structure, it does not need "time" or "prettyFunction" fields. And all 
	 * the test printfs are still printed to the .log file, so I can easily 
	 * figure out, why a certain test does not pass.
	 *
	 * 3. I can write a "class ColouredTerminalWriter : public Debug::Observer", 
	 * which will check the level of given lines and paint errors - in red, 
	 * warnings - yellow, infos - default and debug - grey. Just for fun.
	 *
	 * 4. I can write a "class ResourceTracker : public Debug::Observer", 
	 * which will track resource loading (printfs with tag "RESOURCE") and it's 
	 * errors.
	 *
	 * 5. Now this is an interesting case: imagine, if I need to debug stats 
	 * of an enemy in a video game, and these stats depend on the enemy/player 
	 * position (debugging AI of a guy with multiple states). I can create a 
	 * text field, which will also be ": public Debug::Observer", on an in-game 
	 * screen, nearby the enemy.
	 *
	 * 6. If go further, this flexible pattern allows me to pass debug lines to 
	 * web UI. Handy, if you want a comfortable server for your game.
	 * */
	class Observer
	{
	public:
		/**
		 * @brief ... */
		Observer();

		/**
		 * @brief ... */
		virtual ~Observer();

		/**
		 * @brief ...
		 * @param debug - pointer to the debug object to be attached to
		 * @param level - max debug level to be tracked
		 * @param tags  - a set of tags to track, lines not having common tags 
		 *                are ignored, empty tags allows all lines */
		void attach(Debug *debug, 
				Level level = Level::DBGA, 
				const Tag::Expression &tagExp = Tag::Expression());

		/**
		 * @brief ... */
		void detach();

		/**
		 * @brief On notify callback - what to do when a new line obtained
		 *
		 * No need to check level/tags inside, as they are already checked before
		 *
		 * @param line - the debug line with meta information */
		virtual void onNotify(const Line &line) = 0;

	private:
		Debug *m_debug;
		Level m_level;
	};


#if defined DEBUG_LEVEL
#	if DEBUG_LEVEL == 0x00
	static const Level LEVEL = Level::DEBG;
#	elif DEBUG_LEVEL == 0x01
	static const Level LEVEL = Level::DBG1;
#	elif DEBUG_LEVEL == 0x02
	static const Level LEVEL = Level::DBG2;
#	elif DEBUG_LEVEL == 0x03
	static const Level LEVEL = Level::DBG3;
#	elif DEBUG_LEVEL == 0x04
	static const Level LEVEL = Level::DBG4;
#	elif DEBUG_LEVEL == 0x05
	static const Level LEVEL = Level::DBG5;
#	elif DEBUG_LEVEL == 0x06
	static const Level LEVEL = Level::DBG6;
#	elif DEBUG_LEVEL == 0x07
	static const Level LEVEL = Level::DBG7;
#	elif DEBUG_LEVEL == 0x08
	static const Level LEVEL = Level::DBG8;
#	elif DEBUG_LEVEL == 0x09
	static const Level LEVEL = Level::DBG9;
#	else
	static const Level LEVEL = Level::DBGA;
#	endif
#else
	static const Level LEVEL = Level::INFO;
#endif


	/**
	 * @brief ... */
	Debug();

	/**
	 * @brief ... */
	virtual ~Debug();

	/**
	 * @brief Obtain the line and pass it to all proper Observers
	 * @param file      - source filename
	 * @param lnNumber  - the number of line, from which the debug is printed
	 * @param function  - function name (the "pretty" one)
	 * @param level     - debug level
	 * @param tags      - tags (in one string), separated by ", "
	 * @param format    - the format of the debug output, given by user
	 *
	 * Be careful with C-formatted arguments. You can easily shoot your leg 
	 * with SEGFAULT. */
	void printf(const char *file, 
			unsigned lnNumber, 
			const char *function, 
			Level level, 
			const char *tag, 
			const char *format, 
			...);

private:
	typedef std::map<Level, std::map<Observer *, Tag::Expression>> ObserverMap;

	ObserverMap m_observers;
	mutable std::recursive_mutex m_lock;

friend void Observer::attach(Debug *, Level, const Tag::Expression &);
friend void Observer::detach();
};

extern Debug debug;

} /* namespace ATD */


#define DPRINTFx(level, tag, format, args...) \
	do { \
		if (level <= ATD::Debug::LEVEL) { \
			ATD::debug.printf( \
					__FILE__, \
					__LINE__, \
					__PRETTY_FUNCTION__, \
					level, \
					tag, \
					format, ##args \
					); \
		} \
	} while(0)


#define EPRINTF(tag, format, args...) \
	DPRINTFx(ATD::Debug::Level::ERRO, tag, format, ##args)
#define WPRINTF(tag, format, args...) \
	DPRINTFx(ATD::Debug::Level::WARN, tag, format, ##args)
#define IPRINTF(tag, format, args...) \
	DPRINTFx(ATD::Debug::Level::INFO, tag, format, ##args)
#define DPRINTF(tag, format, args...) \
	DPRINTFx(ATD::Debug::Level::DEBG, tag, format, ##args)

#define DPRINTF_1(tag, format, args...) \
	DPRINTFx(ATD::Debug::Level::DBG1, tag, format, ##args)
#define DPRINTF_2(tag, format, args...) \
	DPRINTFx(ATD::Debug::Level::DBG2, tag, format, ##args)
#define DPRINTF_3(tag, format, args...) \
	DPRINTFx(ATD::Debug::Level::DBG3, tag, format, ##args)
#define DPRINTF_4(tag, format, args...) \
	DPRINTFx(ATD::Debug::Level::DBG4, tag, format, ##args)
#define DPRINTF_5(tag, format, args...) \
	DPRINTFx(ATD::Debug::Level::DBG5, tag, format, ##args)
#define DPRINTF_6(tag, format, args...) \
	DPRINTFx(ATD::Debug::Level::DBG6, tag, format, ##args)
#define DPRINTF_7(tag, format, args...) \
	DPRINTFx(ATD::Debug::Level::DBG7, tag, format, ##args)
#define DPRINTF_8(tag, format, args...) \
	DPRINTFx(ATD::Debug::Level::DBG8, tag, format, ##args)
#define DPRINTF_9(tag, format, args...) \
	DPRINTFx(ATD::Debug::Level::DBG9, tag, format, ##args)
#define DPRINTF_A(tag, format, args...) \
	DPRINTFx(ATD::Debug::Level::DBGA, tag, format, ##args)


