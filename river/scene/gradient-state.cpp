#include "gradient-state.hpp"

namespace River
{
	static const char* vertex_shader = "precision highp float;\
		uniform vec2 scene;\
		attribute vec2 point;\
		attribute vec3 color;\
		varying vec4 v_color;\
		void main(void)\
		{\
			gl_Position = vec4(point.x / scene.x - 1.0, 1.0 - point.y / scene.y, 0.0, 1.0);\
			v_color.rgb = color;\
			v_color.a = 1.0;\
		}";
	
	static const char* fragment_shader = "precision highp float;\
		varying vec4 v_color;\
		void main(void)\
		{\
			gl_FragColor = v_color;\
		}";

	GradientState::GradientState() : ShaderState(vertex_shader, fragment_shader)
	{
	}

	void GradientState::bind_attributes(GLuint program)
	{
		glBindAttribLocation(program, 0, "point");
		glBindAttribLocation(program, 1, "color");
	}

	void GradientState::get_uniforms(GLuint program)
	{
	}

	void GradientState::use()
	{
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glDisable(GL_BLEND);

		ShaderState::use();
	}
};
