#pragma once
#include <malloc.h>
#include <swl.h>

bool gluCompileShader(GLuint program, GLenum type, const char *source);
bool gluLinkProgram(GLuint program);
