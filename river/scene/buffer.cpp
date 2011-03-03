#include <iostream>
#include <malloc.h>
#include "buffer.hpp"
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>

#define RIVER_BUFFER_USE_EXTESION

namespace River
{
	PFNGLMAPBUFFEROESPROC glMapBufferOES;
	PFNGLUNMAPBUFFEROESPROC glUnmapBufferOES;

	void buffer_setup()
	{
		glMapBufferOES = (PFNGLMAPBUFFEROESPROC)eglGetProcAddress("glMapBufferOES");
		glUnmapBufferOES = (PFNGLUNMAPBUFFEROESPROC)eglGetProcAddress("glUnmapBufferOES");
	}

	void *buffer_map(GLenum target)
	{
		return glMapBufferOES(target, GL_WRITE_ONLY_OES);
	}

	void buffer_unmap(GLenum target)
	{
		glUnmapBufferOES(target);
	}
};
