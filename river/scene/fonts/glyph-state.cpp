#include "../scene.hpp"
#include "glyph-state.hpp"
#include "glyph-cache.hpp"

namespace River
{
	static const char* vertex_shader = "precision highp float;\
		uniform vec2 scene;\
		uniform float offset;\
		attribute vec2 point;\
		attribute vec2 tex_coord;\
		varying vec2 v_tex_coord;\
		void main(void)\
		{\
			gl_Position = vec4(point.x / scene.x - 1.0, 1.0 - point.y / scene.y, 0.0, 1.0);\
			v_tex_coord = vec2(tex_coord.x - offset, tex_coord.y);\
		}";
	
	static const char* fragment_shader = "precision highp float;\
		varying vec2 v_tex_coord;\
		uniform float alpha;\
		uniform float point_offset;\
		uniform lowp sampler2D texture;\
		void main(void)\
		{\
			gl_FragColor.r = texture2D(texture, vec2(v_tex_coord.x - point_offset, v_tex_coord.y)).a;\
			gl_FragColor.g = texture2D(texture, v_tex_coord).a;\
			gl_FragColor.b = texture2D(texture, vec2(v_tex_coord.x + point_offset, v_tex_coord.y)).a;\
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
		offset_uniform = glGetUniformLocation(program, "offset");
		texture_uniform = glGetUniformLocation(program, "texture");
		point_offset_uniform = glGetUniformLocation(program, "point_offset");
		
		ShaderState::use();

		glUniform1f(point_offset_uniform, GlyphCache::point_size);
	}

	void GlyphState::use()
	{
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_SRC_COLOR);
		
		ShaderState::use();

		glUniform1i(texture_uniform, 0);
	}
};
