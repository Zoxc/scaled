#pragma once
#include <malloc.h>
#include <GLES2/gl2.h>

bool gluCompileShader(GLuint program, GLenum type, const char *source);
bool gluLinkProgram(GLuint program);
