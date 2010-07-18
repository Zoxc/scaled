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
};
