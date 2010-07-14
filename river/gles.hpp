#pragma once
#ifdef _WIN32
	#define NOMINMAX
#endif

#include <malloc.h>
#include <swl.h>

bool gluCompileShader(GLuint program, GLenum type, const char *source);
bool gluLinkProgram(GLuint program);
