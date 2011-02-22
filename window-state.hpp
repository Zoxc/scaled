#pragma once
#include "river/scene/shader-state.hpp"

class WindowState:
	public River::ShaderState
{
public:
	WindowState();
	
	GLuint texture_uniform;

	void bind_attributes(GLuint program);
	void get_uniforms(GLuint program);
	void use();
};
