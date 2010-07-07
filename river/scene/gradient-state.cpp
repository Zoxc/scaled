#include "gradient-state.hpp"

namespace River
{
	GradientState gradient_state;

	static const char* _vertex_source = "precision highp float;\
		uniform vec2 scene;\
		attribute vec2 point;\
		attribute vec3 acolor;\
		varying vec4 vcolor;\
		void main(void)\
		{\
			gl_Position.x = point.x / scene.x - 1.0;\
			gl_Position.y = -(point.y / scene.y - 1.0);\
			gl_Position.z = 0.0;\
			gl_Position.w = 1.0;\
			vcolor.rgb = acolor;\
			vcolor.a = 1.0;\
		}";
	
	static const char* _fragment_source = "precision highp float;\
		varying vec4 vcolor;\
		void main (void)\
		{\
			gl_FragColor = vcolor;\
		}";

	GradientState::GradientState() : ShaderState(_vertex_source, _fragment_source)
	{
	}

	void GradientState::bind_attributes(GLuint program)
	{
		glBindAttribLocation(program, 0, "point");
		glBindAttribLocation(program, 1, "acolor");
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
