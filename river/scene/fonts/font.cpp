#include "font.hpp"
#include "font-size.hpp"
#include <assert.h>

namespace River
{
	bool FontSizeFunctions::compare_key_value(size_t key, FontSize *value)
	{
		return key == value->size;
	}

	size_t FontSizeFunctions::get_key(FontSize *value)
	{
		return value->size;
	}

	FontSize *FontSizeFunctions::get_value_next(FontSize *value)
	{
		return value->next;
	}

	void FontSizeFunctions::set_value_next(FontSize *value, FontSize *next)
	{
		value->next = next;
	}

	bool FontSizeFunctions::valid_key(size_t key)
	{
		return true;
	}

	bool FontSizeFunctions::create_value()
	{
		return true;
	}

	FontSize *FontSizeFunctions::create_value(StdLibAllocator::Ref alloc_ref, Font *font, size_t key)
	{
		return new FontSize(*font, key);
	}

	FT_Library Font::library;

	void Font::setup()
	{
		assert(FT_Init_FreeType(&library) == 0);
	}

	Font::Font() : current_size(0), name(0), table(1, this)
	{
	}

	Font::~Font()
	{
		FT_Done_Face(face);
	}
	
	bool Font::allocate(const char *name)
	{
		this->name = name;

		return FT_New_Face(library, name, 0, &face) == 0;
	}

	FontSize *Font::get_size(size_t size)
	{
		return table.get(size);
	}

	FT_Face Font::use_size(size_t size)
	{
		if(current_size != size)
		{
			assert(FT_Set_Char_Size(face, 0, size * 64, 3 * 96, 96) == 0);
			current_size = size;
		}

		return face;
	}
};
