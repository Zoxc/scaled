#pragma once
#include "../../freetype.hpp"
#include "../../hash-table.hpp"
#include "../../ref-object.hpp"
#include "font-size.hpp"

namespace River
{
	class FontSizeFunctions:
		public HashTableFunctions<size_t, FontSize *, Font *>
	{
	public:
		static bool compare_key_value(size_t key, FontSize *value)
		{
			return key == value->size;
		}

		static size_t get_key(FontSize *value)
		{
			return value->size;
		}

		static FontSize *get_value_next(FontSize *value)
		{
			return value->next;
		}

		static void set_value_next(FontSize *value, FontSize *next)
		{
			value->next = next;
		}

		static bool valid_key(size_t key)
		{
			return true;
		}

		static bool create_value()
		{
			return true;
		}

		static FontSize *create_value(Font *font, size_t key)
		{
			return new FontSize(*font, key);
		}
	};

	typedef HashTable<size_t, FontSize *, Font *, FontSizeFunctions> FontSizeTable;

	class Font:
		public RefObject
	{
	public:
		Font();
		~Font();

		bool allocate(const char *name);
		FontSize *get_size(size_t size);
		
		static void setup();

		static FT_Library library;

		size_t current_size;
		const char *name;
		FT_Face face;

		/*
		 * TODO: Make FontSize's destructor remove the reference in this hash table.
		 */
		FontSizeTable table;

	};
};
