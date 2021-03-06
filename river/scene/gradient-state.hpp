#pragma once
#include "shader-state.hpp"

namespace River
{
	class GradientState:
		public ShaderState
	{
	public:
		GradientState();

		void bind_attributes(GLuint program);
		void get_uniforms(GLuint program);
		void use();
	};
};
