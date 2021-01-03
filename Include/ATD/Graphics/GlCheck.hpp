/**
 * @file
 * @brief Debug macro for OpenGl functions.
 * ... */

#pragma once

#include <ATD/Core/Debug.hpp>
#include <ATD/Graphics/Gl.hpp>

#include <map>
#include <string>


namespace ATD {

const std::map<Gl::Enum, std::string> GL_ERR_MAP = {
	{ Gl::INVALID_ENUM, "INVALID_ENUM" }, 
	{ Gl::INVALID_VALUE, "INVALID_VALUE" }, 
	{ Gl::INVALID_OPERATION, "INVALID_OPERATION" }, 
	{ Gl::STACK_OVERFLOW, "STACK_OVERFLOW" }, 
	{ Gl::STACK_UNDERFLOW, "STACK_UNDERFLOW" }, 
	{ Gl::OUT_OF_MEMORY, "OUT_OF_MEMORY" }, 
	{ Gl::INVALID_FRAMEBUFFER_OPERATION, "INVALID_FRAMEBUFFER_OPERATION" }
};

} /* namespace ATD */

#define GL_CHECK(tag, func) \
	do { \
		func; \
		ATD::Gl::Enum _glErrCode = ATD::gl._GetError(); \
		if (_glErrCode != ATD::Gl::NO_ERROR) { \
			std::string _glErrStr = "???"; \
			auto _glErrIter = ATD::GL_ERR_MAP.find(_glErrCode); \
			if (_glErrIter != ATD::GL_ERR_MAP.end()) { \
				_glErrStr = _glErrIter->second; \
			} \
			if (ATD::Debug::Level::ERRO <= ATD::Debug::LEVEL) { \
				ATD::debug.Printf(__FILE__, \
						__LINE__, \
						__PRETTY_FUNCTION__, \
						ATD::Debug::Level::ERRO, \
						tag, \
						"OpenGL function '%s' failure: %s", \
						#func, _glErrStr.c_str()); \
			} \
		} \
	} while (false)


