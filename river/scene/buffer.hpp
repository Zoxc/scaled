#pragma once
#include "../gles.hpp"

namespace River
{
    class Buffer
    {
        public:
            Buffer(GLenum target, GLsizeiptr size);
            ~Buffer();

            void bind();
            void *map();
            void unmap();

            static void setup();

            size_t size;
            void *mapped;
            GLuint handle;
            GLenum target;
    };
};
