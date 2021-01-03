/**
* @file     
* @brief    Debug class for debug + macros.
* @details  License: GPL v3.
* @author   ArthurTheDigital (arthurthedigital@gmail.com)
* @since    $Id: $ */

#include <ATD/Core/Debug.hpp>

#include <ATD/Core/Printf.hpp>

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

#include <set>


/* Debug::Observer */

ATD::Debug::Observer::Observer()
	: m_debug(nullptr)
{}

ATD::Debug::Observer::~Observer()
{
	Detach();
}

void ATD::Debug::Observer::Attach(Debug *debug, 
		Debug::Level level, 
		const Tag::Expression &tagExp)
{
	Detach();
	if (debug) {
		std::lock_guard<std::recursive_mutex> lock(debug->m_lock);

		/* Set pending attach */
		debug->m_observers[level][this] = tagExp;

		m_debug = debug;
		m_level = level;
	}
}

void ATD::Debug::Observer::Detach()
{
	if (m_debug) {
		std::lock_guard<std::recursive_mutex> lock(m_debug->m_lock);

		auto lvIter = m_debug->m_observers.find(m_level);
		if (lvIter != m_debug->m_observers.end()) {

			auto obsIter = lvIter->second.find(this);
			if (obsIter != lvIter->second.end()) {

				lvIter->second.erase(obsIter);
				if (!lvIter->second.size()) {
					m_debug->m_observers.erase(lvIter);
				}
			}
		}

		m_debug = nullptr;
	}
}


/* Debug */

ATD::Debug::Debug()
	: m_observers()
	, m_lock()
{}

ATD::Debug::~Debug()
{
	std::set<Observer *> observers;
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);
		for (auto &lvPair : m_observers) {
			for (auto &obsPair : lvPair.second) {
				observers.insert(obsPair.first);
			}
		}
	}
	for (auto &observer : observers) { observer->Detach(); }
}

void ATD::Debug::Printf(const char *file, 
		unsigned lnNumber, 
		const char *function, 
		Level level, 
		const char *tag, 
		const char *format, 
		...)
{
	/* Fill the line */
	va_list args;
	::va_start(args, format);
	std::string payload = VaPrintf(format, args);
	::va_end(args);

	Line line(level, 
			Tag(std::string(tag)), 
			std::chrono::system_clock::now(), 
			std::string(file), 
			std::string(function), 
			lnNumber, 
			payload
			);

	/* Notify Observers */
	{
		std::vector<Observer *> shallBeNotified;
		{
			/* No one shall Attach()/Detach(), while we are iterating, 
			 * and no one shall Detach() while we are notifying.
			 *
			 * If Detach() is called from OnNotify(), this is still fine, 
			 * because our mutex is recursive, and will be locked by the 
			 * same thread, that locked it for notifying. */
			std::lock_guard<std::recursive_mutex> lock(m_lock);

			/* Nothing in this cycle can trigger Attach()/Detach() */
			for (auto &lvIter : m_observers) {
				if (lvIter.first >= line.level) {
					for (auto &obsIter : lvIter.second) {
						if (obsIter.second.Check(line.tag)) {
							shallBeNotified.push_back(obsIter.first);
						}
					}
				}
			}
		}

		/* If triggers Detach(), that only of the node, that already 
		 * have been processed */
		for (auto &obsIter : shallBeNotified) {
			obsIter->OnNotify(line);
		}
	}
}


/* Global variable - OH NOES!*/

ATD::Debug ATD::debug;


