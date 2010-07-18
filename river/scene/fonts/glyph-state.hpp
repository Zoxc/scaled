#pragma once
#include "../shader-state.hpp"

namespace River
{
	class GlyphState:
		public ShaderState
	{
	public:
		GlyphState();
		
		GLuint color_uniform;
		GLuint texture_uniform;

		void bind_attributes(GLuint program);
		void get_uniforms(GLuint program);
		void use();
	};
};
