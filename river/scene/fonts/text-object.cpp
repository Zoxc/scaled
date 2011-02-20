#include "text-object.hpp"
#include "../layer.hpp"
#include "../scene.hpp"

namespace River
{
	TextObject::TextObject() : font_size(0), layer(0)
	{
	}

	TextObject::~TextObject()
	{
		clear();
	}

	void TextObject::clear()
	{
		/*if(!glyph_list.empty())
		{
			GlyphList::Iterator i = glyph_list.begin();

			while(i != glyph_list.end())
			{
				GlyphObject *object = *i;

				if(layer)
					object->deattach(layer);

				++i;
				delete object;
			}

			glyph_list.clear();

			delete vertex_buffer;
			delete coords_buffer;

			font_size->deref();
		}*/
	}
	
    GLshort *TextObject::buffer_quad(GLshort *buffer, bool first, int x, int y, int width, int height)
    {
		int right = x + width;
		int bottom = y + height;
		
        if(!first)
        {
            *buffer++ = x;
			*buffer++ = y;
        }

        *buffer++ = x;
		*buffer++ = y;

        *buffer++ = right;
		*buffer++ = y;

        *buffer++ = x;
		*buffer++ = bottom;

        *buffer++ = right;
		*buffer++ = bottom;

        if(first)
        {
            *buffer++ = right;
			*buffer++ = bottom;
        }

        return buffer;
    }

    GLfloat *TextObject::buffer_coords(GLfloat *buffer, bool first, Glyph::Variation *variant)
    {
        if(!first)
        {
            *buffer++ = variant->coords[0].x;
			*buffer++ = variant->coords[0].y;
        }

        for(int i = 0; i < 4; i++)
		{
            *buffer++ = variant->coords[i].x;
			*buffer++ = variant->coords[i].y;
		}

        if(first)
        {
            *buffer++ = variant->coords[3].x;
			*buffer++ = variant->coords[3].y;
        }

        return buffer;
    }

	void TextObject::position(int x, int y, FontSize *font_size, color_t color, const char *text)
	{
		clear();

		size_t chars = strlen(text);

		if(chars == 0)
			return;

		this->font_size = font_size;
		this->color = color;

		font_size->ref();

		indices = chars * 10;

        vertex_buffer = new Buffer(GL_ARRAY_BUFFER, chars * 10 * sizeof(GLshort));
        coords_buffer = new Buffer(GL_ARRAY_BUFFER, chars * 10 * sizeof(GLfloat));
		
        GLshort *vertex_map = (GLshort *)vertex_buffer->map();
        GLfloat *coords_map = (GLfloat *)coords_buffer->map();

		int left = 0;
        bool first = true;

		while(*text)
		{
			Glyph *glyph = font_size->get_glyph(*text);

			int offset = left + glyph->offset_x;
			int subpixel_offset = offset % 3;

			// Make sure it's a positive number
			if(subpixel_offset < 0)
				subpixel_offset += 3;
			
			vertex_map = buffer_quad(vertex_map, first, x + offset / 3, y - glyph->offset_y, glyph->offsets[subpixel_offset].width, glyph->height);
			coords_map = buffer_coords(coords_map, first, &glyph->offsets[subpixel_offset]);

			left += glyph->advance;
			
			first = false;

			text++;
		}
		
		vertex_buffer->unmap();
		coords_buffer->unmap();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	void TextObject::render()
	{
		glUniform1f(Scene::glyph_state.alpha_uniform, color_alpha_component(color) / (GLfloat)255.0);
		glBlendColor(color_red_component(color) / (GLfloat)255.0, color_green_component(color) / (GLfloat)255.0, color_blue_component(color) / (GLfloat)255.0, 0.0);

		vertex_buffer->bind();
		glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 0, 0);

		coords_buffer->bind();
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, indices);
		
		Object::render();
		
        glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void TextObject::attach(Layer *layer)
	{
		assert(this->layer == 0);

		this->layer = layer;
		/*
		for(GlyphList::Iterator i = glyph_list.begin(); i != glyph_list.end(); ++i)
			i().attach(layer);*/
	}
};
