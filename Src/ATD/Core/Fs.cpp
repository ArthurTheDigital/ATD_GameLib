/**
* @file     
* @brief    Fs functions implementation.
* @details  License: GPL v3.
* @author   ArthurTheDigital (arthurthedigital@gmail.com)
* @since    $Id: $ */

#include <ATD/Core/Fs.hpp>

#include <ATD/Core/Printf.hpp>

#include <dirent.h>
#include <errno.h>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <set>
#include <stdexcept>

#include <stdio.h>


/* Auxiliary */

static void PathBreakdown(const std::string &path, 
		const std::string &delimiter, 
		bool &absolute, 
#if defined _WIN32
		/* TODO: Add data for Windows absolute path. */
#endif
		size_t &reverseCount, 
		std::vector<std::string> &items)
{
	size_t start = 0;

	reverseCount = 0;
	items.clear();

	absolute = false;
	size_t firstDelimiterPos = path.find(delimiter, 0);
#if defined _WIN32
	if ((firstDelimiterPos == 2) && ('A' <= path[0] <= 'Z') && (path[1] == ':')) {
			absolute = true;
			/* TODO: Write Windows-specific data */
			start = delimiter.size() + 2;
		}
	}
#else
	if (firstDelimiterPos == 0) {
		absolute = true;
		start = delimiter.size();
	}
#endif

	size_t curr, prev = start;
	do {
		curr = path.find(delimiter, prev);
		/* ...avgfav/fesdfds/gfdgfd... 
		 *           ^      ^          
		 *         prev   curr
		 * */
		std::string newItem = 
			(curr != std::string::npos) ? 
			path.substr(prev, curr - prev) : 
			path.substr(prev);

		if (newItem.size() && newItem != ATD::Fs::Path::CURR_DIR) {
			if (newItem == ATD::Fs::Path::REVERSE_SHIFT) {
				if (items.size()) {
					items.pop_back();
				} else {
					reverseCount++;
				}
			} else {
				items.push_back(newItem);
			}
		}

		prev = curr + delimiter.size();
	} while (curr != std::string::npos);
}

static std::string PathAssemble(const std::string &delimiter, 
		bool absolute, 
#if defined _WIN32
		/* TODO: Add data for Windows absolute path. */
#endif
		size_t reverseCount, 
		const std::vector<std::string> &items)
{
	std::string root;
#if defined _WIN32
	/* TODO: Write actual Windows root data. */

	/* ZAGLUSHKA */
	root = "C:";
#endif

	std::string reversePath = reverseCount ? ATD::Fs::Path::REVERSE_SHIFT : std::string();
	for (size_t rcIter = 1; rcIter < reverseCount; rcIter++) {
		reversePath += delimiter + ATD::Fs::Path::REVERSE_SHIFT;
	}

	std::string directPath = items.size() ? items[0] : std::string();
	for (size_t itmIter = 1; itmIter < items.size(); itmIter++) {
		directPath += delimiter + items[itmIter];
	}

	std::string path = reversePath.size() ? 
		directPath.size() ? reversePath + delimiter + directPath : reversePath : 
		directPath.size() ? directPath : ATD::Fs::Path::CURR_DIR;

	if (absolute) {
		return root + delimiter + path;
	} else {
		return path;
	}
}


/* ATD::Fs::Stat */

ATD::Fs::Stat::Stat(const ATD::Fs::Path &path)
	: m_mode(0)
	, m_size(0)
{
	int statResult;
#if defined _WIN32
	struct _stat stBuf;
	statResult = ::_stat(path.Native().c_str(), &stBuf);
#else
	struct stat stBuf;
	statResult = ::stat(path.Native().c_str(), &stBuf);
#endif

	if (statResult == -1) {
		/* Nasty error happened. */
		int errnoVal = errno;
		throw std::runtime_error(
				Printf(
					"Cannot stat \"%s\" : %d %s", 
					path.Native().c_str(), 
					errnoVal, ::strerror(errnoVal)
					)
				);
	}

	m_mode = stBuf.st_mode;
	m_size = stBuf.st_size;
}

ATD::Fs::Stat::Stat()
	: Stat(ATD::Fs::Path())
{}

bool ATD::Fs::Stat::IsDir() const
{
	return S_ISDIR(m_mode);
}

bool ATD::Fs::Stat::IsReg() const
{
	return S_ISREG(m_mode);
}

bool ATD::Fs::Stat::IsLnk() const
{
	return S_ISLNK(m_mode);
}

size_t ATD::Fs::Stat::Size() const
{
	return static_cast<size_t>(m_size);
}


/* ATD::Fs::Path constants */

const std::string ATD::Fs::Path::COMMON_FILE_DELIMITER = "/";

#if defined _WIN32
const std::string ATD::Fs::Path::NATIVE_FILE_DELIMITER = "\\";
#else
const std::string ATD::Fs::Path::NATIVE_FILE_DELIMITER = Fs::Path::COMMON_FILE_DELIMITER;
#endif

const std::string ATD::Fs::Path::EXTENSION_DELIMITER = ".";

const std::string ATD::Fs::Path::REVERSE_SHIFT = "..";
const std::string ATD::Fs::Path::CURR_DIR = ".";

const size_t ATD::Fs::Path::PATH_MAX_GUESS_START = 1024;
const size_t ATD::Fs::Path::PATH_MAX_GUESS_POW2 = 4;


/* ATD::Fs::Path */

ATD::Fs::Path::Path(const std::string &path, 
		Fs::Path::Type type)
	: m_absolute(false)
#if defined _WIN32
	/* TODO: Construct Windows data */
#endif
	, m_reverseCount(0)
	, m_items()

	, m_preComputedNative()
	, m_preComputedCommon()
{
	PathBreakdown(path, 
			type == COMMON ? COMMON_FILE_DELIMITER : NATIVE_FILE_DELIMITER, 
			m_absolute, 
#if defined _WIN32
			/* TODO: Add data for Windows absolute path. */
#endif
			m_reverseCount, 
			m_items
			);

	if (m_absolute && m_reverseCount) {
		throw std::runtime_error(
				Printf(
					"Absolute path \"%s\" shall not contain reverse shifts", 
					Native().c_str()
					)
				);
	}
}

std::string ATD::Fs::Path::Common() const
{
	if (IsAbsolute()) {
		throw std::runtime_error(
				Printf(
					"No common version for absolute path \"%s\"", 
					Native().c_str()
					)
				);
	}

	if (COMMON_FILE_DELIMITER != NATIVE_FILE_DELIMITER) {
		if (!m_preComputedCommon.size()) {
			m_preComputedCommon = PathAssemble(COMMON_FILE_DELIMITER, 
					m_absolute, 
#if defined _WIN32
					/* TODO: Add data for Windows absolute path. */
#endif
					m_reverseCount, 
					m_items
					);
		}
		return m_preComputedCommon;
	} else {
		/* Small optimization to not store same cache twice */
		return Native();
	}
}

std::string ATD::Fs::Path::Native() const
{
	if (!m_preComputedNative.size()) {
		m_preComputedNative = PathAssemble(NATIVE_FILE_DELIMITER, 
				m_absolute, 
#if defined _WIN32
				/* TODO: Add data for Windows absolute path. */
#endif
				m_reverseCount, 
				m_items
				);
	}
	return m_preComputedNative;
}

bool ATD::Fs::Path::IsAbsolute() const
{
	return m_absolute;
}

ATD::Fs::Path ATD::Fs::Path::Joined(const Path &subPath) const
{
	if (subPath.IsAbsolute()) {
		throw std::runtime_error(Printf("Cannot join absolute path \"%s\"", subPath.Native().c_str()));
	}

	Path result;
	if (m_items.size() <= subPath.m_reverseCount) {
		size_t joinedRC = m_reverseCount + subPath.m_reverseCount - m_items.size();
		result = Path(m_absolute, joinedRC, subPath.m_items);
	} else {
		std::vector<std::string> joinedItems = m_items;
		for (size_t rcIter = 0; rcIter < subPath.m_reverseCount; rcIter++) {
			joinedItems.pop_back();
		}
		for (size_t itmIter = 0; itmIter < subPath.m_items.size(); itmIter++) {
			joinedItems.push_back(subPath.m_items[itmIter]);
		}
		result = Path(m_absolute, m_reverseCount, joinedItems);
	}
	return result;
}

ATD::Fs::Path ATD::Fs::Path::Relative(const ATD::Fs::Path &source) const
{
	Path srcAbs = source.Absolute();
	Path dstAbs = Absolute();

#if defined _WIN32
	// TODO: Throw exception in case of different Windows tomes
#endif

	size_t commonSize;
	for (commonSize = 0; 
			commonSize < srcAbs.m_items.size() && commonSize < dstAbs.m_items.size(); 
			commonSize++) {
		if (srcAbs.m_items[commonSize] != dstAbs.m_items[commonSize]) break;
	}

	size_t relRC = srcAbs.m_items.size() - commonSize;
	std::vector<std::string> relItems;
	for (size_t i = commonSize; i < dstAbs.m_items.size(); i++) {
		relItems.push_back(dstAbs.m_items[i]);
	}

	return Path(false, relRC, relItems);
}

ATD::Fs::Path ATD::Fs::Path::Absolute() const
{
	if (IsAbsolute()) {
		return *this;
	} else {
		std::string wdBuffer;
		for (size_t ipow = 0, pathMax = PATH_MAX_GUESS_START; 
				ipow < PATH_MAX_GUESS_POW2; 
				ipow++, pathMax *= 2) {
			wdBuffer = std::string(pathMax, '\0');
			char *getcwdResult = ::getcwd(&wdBuffer[0], pathMax);
			if (getcwdResult == nullptr) {
				int errnoVal = errno;
				if (errnoVal == ERANGE) {
					if (ipow + 1 != PATH_MAX_GUESS_POW2) {
						continue;
					} else {
						throw std::runtime_error(
								Printf(
									"Buffer of size \"%lu * 2^%lu\" is too small for work directory", 
									PATH_MAX_GUESS_START, PATH_MAX_GUESS_POW2
									)
								);
					}
				}
				throw std::runtime_error(
						Printf(
							"Failed to get work directory : %d %s", 
							errnoVal, ::strerror(errnoVal)
							)
						);
			}
		}

		/* ::getcwd returns absolute path to the work dir. And if the current 
		 * path is relative, it is assumed to be relative to work dir */
		return Path(wdBuffer, NATIVE).Joined(*this);
	}
}

ATD::Fs::Path ATD::Fs::Path::ParentDir() const
{
	size_t pdRC = m_reverseCount;
	std::vector<std::string> pdItems = m_items;

	if (pdItems.size()) {
		pdItems.pop_back();
	} else {
		pdRC++;
	}

	return Path(m_absolute, pdRC, pdItems);
}

std::string ATD::Fs::Path::Filename() const
{
	return m_items.size() ? m_items.back() : 
		m_reverseCount ? REVERSE_SHIFT : 
		CURR_DIR;
}

size_t ATD::Fs::Path::ExtensionFromList(const std::vector<std::string> &extensions) const
{
	::fprintf(stderr, "Extension resolv start\n");

	if (!m_items.size()) { return static_cast<size_t>(-1); }

	/* Get a lowercase filename. */
	/* Note:
	 * Since file extensions are all in English, there is no need to 
	 * lowercase the string UTF-8-correctly. Just lowercase the English 
	 * letters. */
	std::string filename = m_items.back();
	for (size_t i = 0; i < filename.size(); i++) {
		if (filename[i] >= 'A' && filename[i] <= 'Z') {
			filename[i] += 'a' - 'A';
		}
	}

	/* Tree-like reverse search for a best-match extension */
	size_t match = static_cast<size_t>(-1);
	std::set<size_t> candidates;
	for (size_t extIter = 0; extIter < extensions.size(); extIter++) {
		candidates.insert(extIter);
	}
	for (size_t fnRIter = 0; fnRIter < filename.size() && candidates.size() > 0; fnRIter++) {
		for (auto cIter = candidates.begin(); cIter != candidates.end();) {
			if (extensions[*cIter].size() > fnRIter) {
				if (extensions[*cIter][extensions[*cIter].size() - fnRIter - 1] != filename[filename.size() - fnRIter - 1]) {
					cIter = candidates.erase(cIter);
				} else {
					cIter++;
				}
			} else if (extensions[*cIter].size() == fnRIter) {
				if (filename[filename.size() - fnRIter - 1] == EXTENSION_DELIMITER[0]) {
					match = *cIter;
					cIter++;
				} else {
					cIter = candidates.erase(cIter);
				}
			} else {
				cIter = candidates.erase(cIter);
			}
		}
	}

	::fprintf(stderr, "Extension resolved\n");

	return match;
}

bool ATD::Fs::Path::Exists() const
{
	int statResult;
#if defined _WIN32
	struct _stat stBuf;
	statResult = ::_stat(Native().c_str(), &stBuf);
#else
	struct stat stBuf;
	statResult = ::stat(Native().c_str(), &stBuf);
#endif

	if (statResult == 0) {
		return true;
	} else {
		int errnoVal = errno;
		if (errnoVal != ENOENT) {
			/* Nasty error happened. */
			throw std::runtime_error(
					Printf(
						"Cannot stat \"%s\" : %d %s", 
						Native().c_str(), 
						errnoVal, ::strerror(errnoVal)
						)
					);
		}
	}
	return false;
}

ATD::Fs::Path ATD::Fs::Path::Linked(bool recursively) const
{
	Stat st(*this);
	if (!st.IsLnk()) {
		return *this;
	} else {
		std::string lnkbuffer(st.Size(), '\0');
		ssize_t readlinkResult = ::readlink(Native().c_str(), &lnkbuffer[0], lnkbuffer.size());
		int errnoVal = errno;

		if (readlinkResult == -1) {
			throw std::runtime_error(
					Printf(
						"Link \"%s\" cannot be read : %d %s", 
						Native().c_str(), 
						errnoVal, ::strerror(errnoVal)
						)
					);
		}

		Path linked(lnkbuffer, NATIVE);
		if (!linked.IsAbsolute()) {
			linked = Joined(linked);
		}

		if (recursively) {
			return linked.Linked();
		} else {
			return linked;
		}
	}
}

void ATD::Fs::Path::Mkdir() const
{
	std::vector<Path> pathCache;
	pathCache.push_back(*this);

	while (!pathCache.back().Exists()) {
		if (!pathCache.back().m_items.size()) {
			/* Reverse part of the path is inaccessible - mkdir is futile! */
			throw std::runtime_error(
					Printf(
						"Path \"%s\" is inaccessible -> Mkdir(\"%s\") is futile", 
						pathCache.back().Native().c_str(), 
						Native().c_str()
						)
					);
		}
		pathCache.push_back(pathCache.back().ParentDir());
	}

	/* Because, the last added path exists */
	for (auto pcRIter = pathCache.rbegin() + 1; pcRIter != pathCache.rend(); pcRIter++) {
		int mkdirResult = ::mkdir(pcRIter->Native().c_str(), 0666);
		if (mkdirResult == -1) {
			if (errno != EEXIST) {
				if (pcRIter != pathCache.rbegin() + 1) {
					throw std::runtime_error(
							Printf(
								"Partial: created \"%s\" /required \"%s\"", 
								pcRIter->Native().c_str(), 
								Native().c_str()
								)
							);
				}
			}
		}
	}
}

std::vector<ATD::Fs::Path> ATD::Fs::Path::Readdir(bool showEverything) const
{
	std::set<std::string> entryNames;
	DIR *dir;
	if (!(dir = ::opendir(Native().c_str()))) {
		int errnoVal = errno;
		throw std::runtime_error(
				Printf(
					"Cannot open \"%s\" as directory : %d %s",
					Native().c_str(), 
					errnoVal, 
					::strerror(errnoVal)
					)
				);
	} else {
		try
		{
			struct dirent *entry;
			while ((entry = ::readdir(dir))) {
				std::string entryName(entry->d_name, ::strnlen(entry->d_name, 256));
				if (entryName != CURR_DIR && entryName != REVERSE_SHIFT) {
					entryNames.insert(entryName);
				}
			}
		} catch (...) {
			/* FIXME: Looks like sweeping problem under carpet. */
		}
		::closedir(dir);
	}

	/* I decided to stash entry names as set and then copy them to vector - 
	 * because this guarantees the alphabetical order on every system. */

	std::vector<Path> paths;
	for (auto enIter = entryNames.begin(); 
			enIter != entryNames.end(); 
			enIter++) {
		Path path = Joined(*enIter);
		if (!showEverything) {
			try
			{
				Stat st(path);
				if (!(st.IsReg() || st.IsLnk() || st.IsDir())) {
					continue;
				}
			} catch (...) {
				/* If cannot stat, no need to show */
				continue;
			}
		}
		paths.push_back(path);
	}
	return paths;
}

void ATD::Fs::Path::Copy(const ATD::Fs::Path &destination) const
{
	if (!Exists()) {
		throw std::runtime_error(
				Printf(
					"No path \"%s\" to Move()", 
					Native().c_str()
					)
				);
	}
	if (destination.Exists()) {
		throw std::runtime_error(
				Printf(
					"Destination occupied \"%s\"", 
					destination.Native().c_str()
					)
				);
	}

	CopyImpl(destination);
}

void ATD::Fs::Path::Unlink() const
{
	if (Stat(*this).IsDir()) {
		std::vector<Path> contents = Readdir(true);
		for (auto cIter = contents.begin(); cIter != contents.end(); cIter++) {
			cIter->Unlink();
		}
		::rmdir(Native().c_str());
	} else {
		::unlink(Native().c_str());
	}
}

void ATD::Fs::Path::Move(const ATD::Fs::Path &destination) const
{
	if (!Exists()) {
		throw std::runtime_error(
				Printf(
					"No path \"%s\" to Move()", 
					Native().c_str()
					)
				);
	}
	if (destination.Exists()) {
		throw std::runtime_error(
				Printf(
					"Destination occupied \"%s\"", 
					destination.Native().c_str()
					)
				);
	}

	/* If fails, throws exception itself */
	destination.ParentDir().Mkdir();

	int renameResult = ::rename(Native().c_str(), destination.Native().c_str());
	if (renameResult == -1) {
		int errnoVal = errno;
		if (errnoVal != EXDEV) {
			throw std::runtime_error(
					Printf(
						"Cannot rename \"%s\" -> \"%s\" : %d %s", 
						Native().c_str(), 
						destination.Native().c_str(), 
						errnoVal, 
						::strerror(errnoVal)
						)
					);
		} else {
			/* Move between devices */
			CopyImpl(destination);
			Unlink();
		}
	}
}

ATD::Fs::Path::Path(bool absolute, 
		size_t reverseCount, 
		const std::vector<std::string> &items)
	: m_absolute(absolute)
	, m_reverseCount(reverseCount)
	, m_items(items)

	, m_preComputedNative()
	, m_preComputedCommon()
{}

void ATD::Fs::Path::CopyImpl(const ATD::Fs::Path &destination) const
{
	Stat st(*this);
	if (st.IsDir()) {
		destination.Mkdir();
		auto entries = Readdir();
		for (auto eIter = entries.begin(); eIter != entries.end(); eIter++) {
			eIter->CopyImpl(destination.Joined(eIter->Filename()));
		}
	} else if (st.IsLnk()) {
		/* Just copy the links themselves as "cp -R" does. 
		 * This may lead to broken links, however other 
		 * alternatives are more confusing for programmer. 
		 *
		 * I had an idea of having a "copy scope" - a dir tree 
		 * to be copied and every link, which does not drive 
		 * outside it, is preserved, every link with absolute path 
		 * is preserved either, but other links with relative path 
		 * shall use recalculated relative path instead. 
		 *
		 * This can be implemented, but it is more confusing for 
		 * programmer to use. */

		std::string lnkbuffer(st.Size(), '\0');

		if (::readlink(
					Native().c_str(), 
					&lnkbuffer[0], 
					lnkbuffer.size()
					) == -1) {
			int errnoVal = errno;
			throw std::runtime_error(
					Printf(
						"Cannot read link \"%s\" : %d %s", 
						Native().c_str(), 
						errnoVal, ::strerror(errnoVal)
						)
					);
		}

		if (::symlink(
				lnkbuffer.c_str(), 
				destination.Native().c_str()
				) == -1) {
			int errnoVal = errno;
			throw std::runtime_error(
					Printf(
						"Cannot create link \"%s\" : %d %s", 
						destination.Native().c_str(), 
						errnoVal, ::strerror(errnoVal)
						)
					);
		}
	} else {
		/* Regular file */
		std::string buffer(st.Size() + 1, '\0');
		FILE *file;
		if (!(file = fopen(Native().c_str(), "r"))) {
			int errnoVal = errno;
			throw std::runtime_error(
					Printf(
						"Cannot open \"%s\" for reading : %d %s", 
						Native().c_str(), 
						errnoVal, ::strerror(errnoVal)
						)
					);
		} else {
			size_t bytesRead = ::fread(&buffer[0], 1, st.Size(), file);
			int errnoVal = errno;
			::fclose(file);

			if (bytesRead != st.Size() && errnoVal) {
				throw std::runtime_error(
					Printf(
						"Incomplete read %lu/%lu from \"%s\" : %d %s", 
						bytesRead, st.Size(), 
						Native().c_str(), 
						errnoVal, ::strerror(errnoVal)
						)
					);
			}
		}

		if (!(file = fopen(destination.Native().c_str(), "w"))) {
			int errnoVal = errno;
			throw std::runtime_error(
					Printf(
						"Cannot open \"%s\" for writing : %d %s", 
						destination.Native().c_str(), 
						errnoVal, ::strerror(errnoVal)
						)
					);
		} else {
			size_t bytesWritten = ::fwrite(buffer.c_str(), 1, st.Size(), file);
			int errnoVal = errno;
			::fclose(file);

			if (bytesWritten != st.Size() && errnoVal) {
				throw std::runtime_error(
					Printf(
						"Incomplete write %lu/%lu to \"%s\" : %d %s", 
						bytesWritten, st.Size(), 
						destination.Native().c_str(), 
						errnoVal, ::strerror(errnoVal)
						)
					);
			}
		}
	}
}


/* Fs */

ATD::Fs::Fs(const Path &binPath)
	: m_binDir(binPath.ParentDir())
{}

ATD::Fs::Path ATD::Fs::BinDir() const
{
	return m_binDir;
}


