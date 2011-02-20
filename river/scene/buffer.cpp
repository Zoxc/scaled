#include <iostream>
#include <malloc.h>
#include "buffer.hpp"

namespace River
{
	#ifdef RIVER_BUFFER_USE_EXTESION
	PFNGLMAPBUFFEROESPROC glMapBufferOES;
	PFNGLUNMAPBUFFEROESPROC glUnmapBufferOES;
	#endif

	void Buffer::setup()
	{
		#ifdef RIVER_BUFFER_USE_EXTESION
			glMapBufferOES = (PFNGLMAPBUFFEROESPROC)eglGetProcAddress("glMapBufferOES");
			glUnmapBufferOES = (PFNGLUNMAPBUFFEROESPROC)eglGetProcAddress("glUnmapBufferOES");
		#endif
	}

	Buffer::Buffer(GLenum target, GLsizeiptr size) : size(size), target(target)
	{
		glGenBuffers(1, &handle);
		bind();
		glBufferData(target, size, 0, GL_STATIC_DRAW);
	}

	void Buffer::bind()
	{
		glBindBuffer(target, handle);
	}

	void *Buffer::map()
	{
		#ifdef RIVER_BUFFER_USE_EXTESION
			bind();
			return glMapBufferOES(target, GL_WRITE_ONLY_OES);
		#else
			mapped = malloc(size);
			return mapped;
		#endif
	}

	void Buffer::unmap()
	{
		#ifdef RIVER_BUFFER_USE_EXTESION
			glUnmapBufferOES(target);
		#else
			bind();
			glBufferData(target, size, mapped, GL_STATIC_DRAW);
			free(mapped);
		#endif
	}

	Buffer::~Buffer()
	{
		if(handle != 0)
			glDeleteBuffers(1, &handle);
	}
};
