#pragma once
#if defined(_WIN32) && !defined(NOMINMAX)
	#define NOMINMAX
#endif

#include <malloc.h>
#include <swl.h>
#include <GLES2\gl2.h>

bool gluCompileShader(GLuint program, GLenum type, const char *source);
bool gluLinkProgram(GLuint program);
