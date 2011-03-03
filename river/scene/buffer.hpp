#pragma once
#include "../gles.hpp"

namespace River
{
	void *buffer_map(GLenum target);
	void buffer_unmap(GLenum target);
	void buffer_setup();

	template<GLenum target, class T> class Buffer
	{
	private:
		GLuint handle;
	public:
		~Buffer()
		{
			glDeleteBuffers(1, &handle);
		}

		T *setup(GLsizeiptr size)
		{
			glGenBuffers(1, &handle);
			bind();
			glBufferData(target, size * sizeof(T), 0, GL_STATIC_DRAW);
			return (T *)buffer_map(target);
		}

		void bind()
		{
			glBindBuffer(target, handle);
		}

		T *map()
		{
			bind();
			return (T *)buffer_map(target);
		}

		void unmap()
		{
			bind();
			buffer_unmap(target);
		}
	};
};
