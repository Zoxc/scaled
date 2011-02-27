#include "../scene.hpp"
#include "glyph-state.hpp"
#include "glyph-cache.hpp"

namespace River
{
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
		uniform float alpha;\
		uniform sampler2D texture;\
		void main(void)\
		{\
			gl_FragColor = texture2D(texture, v_tex_coord);\
			gl_FragColor *= alpha;\
		}";

	GlyphState::GlyphState() : ShaderState(vertex_shader, fragment_shader)
	{
	}

	void GlyphState::bind_attributes(GLuint program)
	{
		glBindAttribLocation(program, 0, "point");
		glBindAttribLocation(program, 1, "tex_coord");
	}

	void GlyphState::get_uniforms(GLuint program)
	{
		alpha_uniform = glGetUniformLocation(program, "alpha");
		texture_uniform = glGetUniformLocation(program, "texture");
		
		ShaderState::use();
	}

	void GlyphState::use()
	{
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_CONSTANT_COLOR, GL_ONE_MINUS_SRC_COLOR); // TODO: Remove silly workaround thanks to ImgTec.
		
		ShaderState::use();

		glUniform1i(texture_uniform, 0);
	}
};
