#include <iostream>
#include <malloc.h>
#include "../gles.hpp"
#include "buffer.hpp"
#include <GLES2/gl2ext.h>

#define RIVER_BUFFER_USE_EXTESION

namespace River
{
	PFNGLMAPBUFFEROESPROC glMapBufferOES;
	PFNGLUNMAPBUFFEROESPROC glUnmapBufferOES;

	void buffer_setup()
	{
		glMapBufferOES = (PFNGLMAPBUFFEROESPROC)swl_get_function("glMapBufferOES");
		glUnmapBufferOES = (PFNGLUNMAPBUFFEROESPROC)swl_get_function("glUnmapBufferOES");
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
