/**
* @file     
* @brief    Fs functions implementation.
* @details  License: GPL v3.
* @author   ArthurTheDigital (arthurthedigital@gmail.com)
* @since    $Id: $ */

#pragma once

#include <memory>
#include <string>
#include <vector>


namespace ATD {

/**
 * @class Fs
 * @brief ...
 *
 * Idea:
 * There are two types of path - native - the platform-specific one - 
 * ../me/file on UNIX and ..\\me\\file on Windows; 
 * and common - UNIX path
 *
 * To be clear - only relative path can be native/common. Absolute 
 * path is always native. */
class Fs 
{
public:
	typedef std::shared_ptr<Fs> Ptr;

	class Path; /* Needed inside Stat class */

	/**
	 * @class Stat
	 * @brief stat structure */
	class Stat
	{
	public:
		/**
		 * @brief Constructor, stats the given path to gather the result
		 * @param path - path to stat
		 *
		 * May throw exceptions. */
		Stat(const Path &path);

		/**
		 * @brief Default constructor from default path */
		Stat();

		/**
		 * @brief ...
		 * @return ... */
		bool IsDir() const;

		/**
		 * @brief ...
		 * @return ... */
		bool IsReg() const;

		/**
		 * @brief ...
		 * @return ... */
		bool IsLnk() const;

		/**
		 * @brief ...
		 * @return ... */
		size_t Size() const;

	private:
		mode_t m_mode;
		off_t m_size;
	};

	/**
	 * @class Path
	 * @brief Relative path handler */
	class Path
	{
	public:
		static const std::string COMMON_FILE_DELIMITER;
		static const std::string NATIVE_FILE_DELIMITER;
		static const std::string EXTENSION_DELIMITER;

		static const std::string REVERSE_SHIFT;
		static const std::string CURR_DIR;

		static const size_t PATH_MAX_GUESS_START;
		static const size_t PATH_MAX_GUESS_POW2;

		/**
		 * @brief ... */
		enum Type {
			COMMON, 
			NATIVE
		};

		/**
		* @brief ...
		*
		* Example:
		* On Windows Path(dir1/file.txt) == Path(dir1\\file.txt, NATIVE)
		*
		* @param path - ...
		* @param type - ...
		* @return ... */
		Path(const std::string &path = CURR_DIR, 
				Type type = COMMON);

		/**
		* @brief Get common path
		* @return ... */
		std::string Common() const;

		/**
		* @brief Get native path
		* @return ... */
		std::string Native() const;

		/**
		 * @brief Whether the path is absolute
		 * @return ... */
		bool IsAbsolute() const;

		/**
		* @brief ...
		* @param subPath - ...
		* @return ... */
		Path Joined(const Path &subPath) const;

		/**
		 * @brief Calculates path to the same object, but relative to the 
		 *        source directory.
		 * @param source - source directory path
		 * @return ... */
		Path Relative(const Path &source = Path(CURR_DIR)) const;

		/**
		 * @brief ...
		 * @return ... */
		Path Absolute() const;

		/**
		* @brief ...
		* @return ... */
		Path ParentDir() const;

		/**
		 * @brief ...
		 * @return a filename of the object, without parent dirs */
		std::string Filename() const;

		/**
		 * @brief Selects a suitable (case-insensitive match) extension from 
		 *        an array of given ones
		 * @param extensions - list of lower-case(!) extensions to choose from
		 * @return index of the extension in the list, that matches, if no 
		 *         one does, returns size_t(-1) */
		size_t ExtensionFromList(const std::vector<std::string> &extensions) const;

		/**
		* @brief ...
		* @return ... */
		bool Exists() const;

		/**
		 * @brief Assumes current path as symlink and unwinds it
		 * @return - path pointed by symlink */
		Path Linked(bool recursively = false) const;

		/**
		 * @brief Recursively creates dir with a path
		 *
		 * If there already is a dir with such path, it is still a success. */
		void Mkdir() const;

		/**
		 * @brief Reads the contents of directory
		 * @param showEverything - if false, returns only regular files, 
		 *                         symlinks and directories, otherwise 
		 *                         displays everything statable
		 * @return ... */
		std::vector<Path> Readdir(bool showEverything = false) const;

		/**
		 * @brief Copies path recursively (like "cp -R")
		 * @param destination - destination path */
		void Copy(const Path &destination) const;

		/**
		 * @brief Unlinks the path recursively */
		void Unlink() const;

		/**
		 * @brief Move file/dir if present to the new path recursively
		 * @param destination - ...
		 *
		 * If moving between devices takes place, device or socket files 
		 * are getting lost! */
		void Move(const Path &destination) const;

	private:
		/**
		 * @brief ...
		 * @param absolute     - ...
		 * @param reverseCount - ...
		 * @param items        - ... */
		Path(bool absolute, 
#if defined _WIN32
				/* TODO: +extra data for Windows absolute path */
#endif
				size_t reverseCount, 
				const std::vector<std::string> &items);

		/**
		 * @brief ...
		 * @param destination - ...
		 *
		 * Relies on destination to be already unoccupied */
		void CopyImpl(const Path &destination) const;


		/* Idea:
		 * Unoptimized path: "../../d1/d2/../d3/d4/../file"
		 * Optimized:        "../../d1/d3/file"
		 *
		 * I assume, it is better to use Optimized paths only.
		 *
		 * Optimized path can be splitted into "reverse" and "direct". 
		 * Reverse part can be stored as a number of reverse shifts.
		 * */
		bool m_absolute;
#if defined _WIN32
		/* TODO: Add extra data for Windows absolute path. */
#endif
		size_t m_reverseCount;
		std::vector<std::string> m_items;

		mutable std::string m_preComputedNative;
		mutable std::string m_preComputedCommon;
	};


	/**
	 * @brief ...
	 *
	 * Usage:
	 * Fs os(Fs::Path(argv[0], Fs::Path::NATIVE))
	 *
	 * @param binPath - work-dir-relative path to binary */
	Fs(const Path &binPath);

	/**
	 * @brief ...
	 * @return ... */
	Path BinDir() const;

	/* Idea:
	 * Since resources are not hard-coded into game, the game just remembers
	 * the binary-relative path to resource (a relative path, seen from 
	 * the directory with game binary file)
	 *
	 * A nice way to access those resources is
	 * os.BinDir().Joined("bin/relative/path_to/file.txt").Native()
	 * */

private:
	Path m_binDir;
};

} /* namespace ATD */


