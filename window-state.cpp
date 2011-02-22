#include "river/scene/scene.hpp"
#include "window-state.hpp"

static const char* vertex_shader = "precision highp float;\
	uniform vec2 scene;\
	attribute vec2 point;\
	attribute vec2 tex_coord;\
	varying vec2 v_tex_coord;\
	void main(void)\
	{\
		gl_Position = vec4(point.x / scene.x - 1.0, 1.0 - point.y / scene.y, 0.0, 1.0);\
		v_tex_coord = tex_coord;\
	}";
	
static const char* fragment_shader = "precision highp float;\
	varying vec2 v_tex_coord;\
	uniform lowp sampler2D texture;\
	void main(void)\
	{\
		gl_FragColor = texture2D(texture, v_tex_coord);\
		gl_FragColor.a = 0.5;\
	}";

WindowState::WindowState() : ShaderState(vertex_shader, fragment_shader)
{
}

void WindowState::bind_attributes(GLuint program)
{
	glBindAttribLocation(program, 0, "point");
	glBindAttribLocation(program, 1, "tex_coord");
}

void WindowState::get_uniforms(GLuint program)
{
	texture_uniform = glGetUniformLocation(program, "texture");
		
	ShaderState::use();
}

void WindowState::use()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
	ShaderState::use();

	glUniform1i(texture_uniform, 0);
}
