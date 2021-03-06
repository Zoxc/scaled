#pragma once
#include "../gles.hpp"
#include "../list.hpp"
#include "state.hpp"

namespace River
{
	class ShaderState:
		public State
	{
	private:
		GLuint program;
		const char *vertex_source;
		const char *fragment_source;
	public:
		ShaderState(const char *vertex_source, const char *fragment_source) : vertex_source(vertex_source), fragment_source(fragment_source) {}
		
		virtual void bind_attributes(GLuint program) = 0;
		virtual void get_uniforms(GLuint program);

		GLuint scene_uniform;

		void use();
		void alloc();
		void free();
		void size(int width, int height);
	};
};
