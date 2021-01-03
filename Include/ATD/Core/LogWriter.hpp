/**
* @file     
* @brief    LogWriter class for debug.
* @details  License: GPL v3.
* @author   ArthurTheDigital (arthurthedigital@gmail.com)
* @since    $Id: $ */

#pragma once

#include <ATD/Core/Debug.hpp>
#include <ATD/Core/Fs.hpp>

#include <stdio.h>

#include <string>


namespace ATD {

/**
 * @class LogWriter
 * @brief Writes debug output to .log file
 *
 * Uses 3 filenames: dir/xxx.log, dir/xxxNEW.log, dir/xxxOLD.log 
 * However, there should be only 2 of them on disk.
 *
 * Each of them does not exceed "m_fileLimit" bytes. When the current 
 * xxx.log is full, LogWriter drops "m_fileLimit" * DISPOSABLE_FRACTION bytes 
 * and rotate the files.
 *
 * Rotation needed to keep one as a backup. Just in case. */
class LogWriter : public Debug::Observer
{
public:
	static const std::string LINE_BREAKER;
	static const std::string DFT_FILE_PATH;
	static const size_t DFT_FILE_LIMIT;
	static const float DISPOSABLE_FRACTION;
	static const size_t MAX_LINE_LENGTH;

	/**
	 * @brief ...
	 * @param filePath - bin-relative path */
	LogWriter(const Fs::Ptr &fs, 
			const Fs::Path &filePath = Fs::Path(std::string(DFT_FILE_PATH)), 
			size_t fileLimit = DFT_FILE_LIMIT);

	/**
	 * @brief ... */
	virtual ~LogWriter() = default;

	/**
	 * @brief ...
	 * @param line - structured debug line */
	virtual void OnNotify(const Debug::Line &line) override;

private:
	/**
	 * @class File
	 * @brief A wrapper for FILE *, used in LogWriter only.
	 *
	 * It is not supposed to be an universal FILE * wrapper, just a wrapper
	 * for LogWriter only, with specific functions. */
	class File;


	/**
	 * @brief ...
	 * @param data - ... */
	void WriteImpl(const std::string &data);


	Fs::Ptr m_fs;
	Fs::Path m_filePath;
	Fs::Path m_filePathOLD;
	Fs::Path m_filePathNEW;
	size_t m_fileLimit;
};

} /* namespace ATD */


