#pragma once
#include "shader-state.hpp"

namespace River
{
	void ShaderState::alloc()
	{
		program = glCreateProgram();
		
		gluCompileShader(program, GL_VERTEX_SHADER, vertex_source);
		gluCompileShader(program, GL_FRAGMENT_SHADER, fragment_source);
	
		bind_attributes(program);

		gluLinkProgram(program);

		get_uniforms(program);
	}

	void ShaderState::free()
	{
		glDeleteProgram(program);
	}

	void ShaderState::use()
	{
		glUseProgram(program);
	}

	void ShaderState::get_uniforms(GLuint program)
	{
	}
};
