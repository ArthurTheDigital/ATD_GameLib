/**
 * @file      
 * @brief     LogWriter class for debug.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3
 * @since     $Id: $ */

#include <ATD/Core/LogWriter.hpp>

#include <iomanip> /* std::put_time */
#include <sstream>
#include <stdexcept>


/* Auxiliary */

/* E.g.:
 * "SomeFile.docx" + "LABEL" = "SomeFileLABEL.docx"
 * "SomeFileNoExt" + "LABEL" = "SomeFileNoExtLABEL"
 * ".SomeFileHidden" + "LABEL" = ".SomeFileHiddenLABEL" */
static std::string _labelFilename(const std::string &filename, 
		const std::string &label)
{
	size_t extensionDelimiter = filename.rfind(".", 1);
	std::string name = filename.substr(0, extensionDelimiter);
	std::string extension = (extensionDelimiter > filename.size()) ? 
		std::string() : filename.substr(extensionDelimiter);

	return name + label + extension;
}

static ATD::Fs::Path _labelPath(const ATD::Fs::Path &path, 
		const std::string &label)
{
	return path.parentDir().joined(ATD::Fs::Path(_labelFilename(path.filename(), label)));
}


/* LogWriter constants */

#if defined _WIN32
const std::string ATD::LogWriter::LINE_BREAKER = "\n\r";
#else
const std::string ATD::LogWriter::LINE_BREAKER = "\n";
#endif

#if defined LOG_WRITER_DFT_FILE_PATH
const std::string ATD::LogWriter::DFT_FILE_PATH = std::string(LOG_WRITER_DFT_FILE_PATH);
#else
const std::string ATD::LogWriter::DFT_FILE_PATH = std::string(".Output.log");
#endif

#if defined LOG_WRITER_DFT_FILE_LIMIT
const size_t ATD::LogWriter::DFT_FILE_LIMIT = LOG_WRITER_DFT_FILE_LIMIT;
#else
const size_t ATD::LogWriter::DFT_FILE_LIMIT = 1024 * 1024 * 4; /* 4 Mbytes */
#endif

#if defined LOG_WRITER_DISPOSABLE_FRACTION
const float ATD::LogWriter::DISPOSABLE_FRACTION = LOG_WRITER_DISPOSABLE_FRACTION;
#else
const float ATD::LogWriter::DISPOSABLE_FRACTION = 0.5f;
#endif

#if defined LOG_WRITER_MAX_LINE_LENGTH
const size_t ATD::LogWriter::MAX_LINE_LENGTH = LOG_WRITER_MAX_LINE_LENGTH;
#else
const size_t ATD::LogWriter::MAX_LINE_LENGTH = 1024;
#endif


/* LogWriter::File declaration */

class ATD::LogWriter::File
{
public:
	/**
	 * @brief ... */
	explicit File();

	/**
	 * @brief ...
	 * @param path     - work-dir-relative path
	 * @param truncate - whether to truncate the file
	 *
	 * Automatically opens an existing file. 
	 * If not present, tries to create it. */
	explicit File(const Fs::Path &path, bool truncate = false);

	/**
	 * @brief ...
	 * @param other -  ... */
	explicit File(const File &other) = delete;

	/**
	 * @brief ...
	 * @param other -  ... */
	explicit File(File &&other);

	/**
	 * @brief ...
	 *
	 * Automatically closes the file */
	~File();

	/**
	 * @brief ...
	 * @param ...
	 *
	 * Copying is forbidden */
	File &operator =(const File &other) = delete;

	/**
	 * @brief ...
	 * @param ...
	 *
	 * Moving is allowed */
	File &operator =(File &&other);

	/**
	 * @brief ...
	 * @param line - a line to write - without '\n' */
	void write(const std::string &line);

	/**
	 * @brief Get size of the file
	 * @return ... */
	size_t size();

	/**
	 * @brief Read the entire content of the file into a single string
	 * @return ... */
	std::string readWhole();

	/**
	 * @brief ... */
	void close();

private:
	FILE *m_file;
};


/* LogWriter::File */

ATD::LogWriter::File::File()
	: m_file(nullptr)
{}

ATD::LogWriter::File::File(const Fs::Path &path, 
		bool truncate)
	: m_file(nullptr)
{
	/* If fails, throws exception itself */
	path.parentDir().mkDir();

	if (truncate) {
		FILE *dummyFile = ::fopen(path.native().c_str(), "wb");
		if (dummyFile) {
			::fclose(dummyFile);
		} else {
			throw std::runtime_error(
					std::string("failed to truncate file \"") + 
					path.native() + 
					std::string("\"")
					);
		}
	}

	/**
	 * man 3 fopen
	 *
	 * a+
	 * The initial file position for reading is at  the beginning of the file, 
	 * but output is always appended to the end of the file.
	 *
	 * That means, ::fseek() and ::ftell() are referred only to the read 
	 * position. Write position is not affected and is always at the end of 
	 * the file.
	 * */
	m_file = ::fopen(path.native().c_str(), "a+b");
}

ATD::LogWriter::File::File(ATD::LogWriter::File &&other)
	: m_file(other.m_file)
{
	other.m_file = nullptr;
}

ATD::LogWriter::File::~File()
{
	close();
}

ATD::LogWriter::File& ATD::LogWriter::File::operator =(ATD::LogWriter::File &&other)
{
	if (m_file) {
		::fclose(m_file);
	}

	m_file = other.m_file;
	other.m_file = nullptr;

	return *this;
}

void ATD::LogWriter::File::write(const std::string &line)
{
	if (m_file) {
		size_t wrResult = ::fwrite(line.c_str(), 1, line.size(), m_file);
		if (wrResult != 1 * line.size()) {
			throw std::runtime_error("failed to write"); // TODO: printf %lu/%lu
		}
	}
}

size_t ATD::LogWriter::File::size()
{
	size_t size = 0;
	if (m_file) {
		::fseek(m_file, 0, SEEK_END);
		long ftellResult = ::ftell(m_file);
		if (ftellResult == -1) {
			throw std::runtime_error("failed to get size");
		} else {
			size = static_cast<size_t>(ftellResult);
		}
	}
	return size;
}

std::string ATD::LogWriter::File::readWhole()
{
	size_t fileSize = size();
	std::string fileString(fileSize + 1, '\0');
	if (m_file) {
		::fseek(m_file, 0, SEEK_SET);
		size_t rdResult = ::fread(&fileString[0], 1, fileSize, m_file);
		if (rdResult < fileSize) {
			throw std::runtime_error("failed to read"); // TODO: printf %lu/%lu
		}
	}
	fileString.resize(fileSize);
	return fileString;
}

void ATD::LogWriter::File::close()
{
	if (m_file) {
		::fclose(m_file);
	}
}


/* LogWriter */

ATD::LogWriter::LogWriter(const Fs::Ptr &fs, 
		const Fs::Path &filePath, 
		size_t fileLimit)
	: Debug::Observer()
	, m_fs(fs)
	, m_filePath(filePath)
	, m_filePathOLD(_labelPath(filePath, "OLD"))
	, m_filePathNEW(_labelPath(filePath, "NEW"))
	, m_fileLimit(fileLimit)
{
	/* Write a "session delimiter" for a better .log file readability */
	writeImpl(LINE_BREAKER + 
			std::string("====.==.== ==:==:== New log session") + 
			LINE_BREAKER
			);

	/* LogWriter shall receive everything from debug */
	attach(&debug);
}

void ATD::LogWriter::onNotify(const Debug::Line &line)
{
	try {
		/* Print line to a single string */
		std::time_t lineTimeT = std::chrono::system_clock::to_time_t(line.time);

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

		writeImpl(oss.str());
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

void ATD::LogWriter::writeImpl(const std::string &data)
{
	File f(m_fs->binDir().joined(m_filePath));
	if (f.size() + data.size() >= m_fileLimit) {
		std::string oldLogContent = f.readWhole();
		f.close();

		/* Calculate disposable volume */
		size_t maxDisposable = m_fileLimit * DISPOSABLE_FRACTION; /* Limit */
		size_t disposableBorder = (maxDisposable > MAX_LINE_LENGTH) ? 
			maxDisposable - MAX_LINE_LENGTH : 0; /* Initial guess */
		{
			size_t curr, prev = disposableBorder;
			while ((curr = oldLogContent.find(LINE_BREAKER, prev)) != std::string::npos) {
				if (curr > maxDisposable) { break; }
				prev = curr + LINE_BREAKER.size();
			}
			disposableBorder = prev;
		}

		/* Truncate the new .log file and fill it partially */
		f = static_cast<File &&>(File(m_fs->binDir().joined(m_filePathOLD), true));
		f.write(". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . "
				". . . . . . . . . . . . . . .\n"); /* To show, that .log is cropped */
		f.write(oldLogContent.substr(disposableBorder));
		f.close();

		/* Rename the files */
		{
			Fs::Path buffer;

			buffer = m_filePathOLD;
			buffer.move(m_filePathNEW);

			buffer = m_filePath;
			buffer.move(m_filePathOLD);

			buffer = m_filePathNEW;
			buffer.move(m_filePath);
		}

		f = static_cast<File &&>(File(m_fs->binDir().joined(m_filePath)));
	}
	f.write(data);
}


