#include "text-object.hpp"
#include "../layer.hpp"
#include "../scene.hpp"

namespace River
{
	TextObject::TextObject() : font_size(0)
	{
	}

	TextObject::~TextObject()
	{
		clear();
	}

	void TextObject::clear()
	{
		GlyphObject *object = glyph_list.first;

		while(object)
		{
			GlyphObject *temp = object;
			object->deattch(layer);
			object = object->text_entry.next;
			delete temp;
		}

		if(font_size)
			font_size->deref();
	}

	void TextObject::position(int x, int y, FontSize *font_size, color_t color, const char *text)
	{
		clear();

		this->font_size = font_size;

		font_size->ref();

		int left = 0;

		while(*text)
		{
			Glyph *glyph = font_size->get_glyph(*text);

			GlyphObject *object = new GlyphObject;

			int offset = left + glyph->offset_x;
			int subpixel_offset = offset % 3;

			// Make sure it's a positive number
			if(subpixel_offset < 0)
				subpixel_offset += 3;
			
			object->set_glyph(glyph, subpixel_offset, color);
			object->position(x + offset / 3, y - glyph->offset_y, glyph->offsets[subpixel_offset].width, glyph->height);

			glyph_list.append(object);

			left += glyph->advance;
			
			text++;
		}
	}

	void TextObject::attach(Layer *layer)
	{
		this->layer = layer;

		for(GlyphList::Iterator i = glyph_list.begin(); i != glyph_list.end(); ++i)
			i().attach(layer);
	}
};
