//

#pragma once
#ifndef SINI_GL_CONTEXT_H
#define SINI_GL_CONTEXT_H

#include "SDL.h"
#include <iostream>
#include <exception>

namespace sini { namespace gl {

	// Wrapper for SDL_GLprofile
	// https://wiki.libsdl.org/SDL_GLprofile
	enum class GLProfile : Uint32 {
		CORE			= SDL_GL_CONTEXT_PROFILE_CORE,
		COMPATIBILITY	= SDL_GL_CONTEXT_PROFILE_COMPATIBILITY,
		ES				= SDL_GL_CONTEXT_PROFILE_ES
	};

	class GLContext {
	public:
		SDL_GLContext handle;

		// Constructors
		GLContext() noexcept = delete;
		GLContext(const GLContext&) noexcept = delete;
		GLContext& operator= (const GLContext&) noexcept = delete;
		GLContext(SDL_Window* win_ptr, int gl_major_version, int gl_minor_version, GLProfile profile) noexcept;
		
		// Destructor
		~GLContext() noexcept;
	};

}} // namespace gl, namespace sini

#endif // !SINI_GL_CONTEXT_H