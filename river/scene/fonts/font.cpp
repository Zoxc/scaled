#include "font.hpp"
#include <assert.h>

namespace River
{
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
