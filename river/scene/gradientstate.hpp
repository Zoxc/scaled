#pragma once
#include "../list.hpp"
#include "shaderstate.hpp"

namespace River
{
	class GradientState:
		public ShaderState
	{
	public:
		void bind_attributes(GLuint program);
		void get_uniforms(GLuint program);
	};
};
