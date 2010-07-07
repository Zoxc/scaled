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
		
		scene_uniform = glGetUniformLocation(program, "scene");
		
		get_uniforms(program);
	}

	void ShaderState::free()
	{
		glDeleteProgram(program);
	}

	void ShaderState::size(int width, int height)
	{
		glUseProgram(program);
		glUniform2f(scene_uniform, width / 2.0f, height / 2.0f);
	}

	void ShaderState::use()
	{
		glUseProgram(program);
	}

	void ShaderState::get_uniforms(GLuint program)
	{
	}
};
