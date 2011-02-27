#pragma once
#include "../../freetype.hpp"
#include "../../hash-table.hpp"
#include "../../allocator.hpp"
#include "../../ref-object.hpp"

namespace River
{
	class Font;
	class FontSize;

	class FontSizeFunctions:
		public HashTableFunctions<size_t, FontSize *, Font *, StdLibAllocator>
	{
	public:
		static bool compare_key_value(size_t key, FontSize *value);
		static size_t get_key(FontSize *value);
		static FontSize *get_value_next(FontSize *value);
		static void set_value_next(FontSize *value, FontSize *next);
		static bool valid_key(size_t key);
		static bool create_value();
		static FontSize *create_value(StdLibAllocator::Ref alloc_ref, Font *font, size_t key);
	};

	typedef HashTable<size_t, FontSize *, Font *, FontSizeFunctions> FontSizeTable;

	class Font:
		public RefObject
	{
	private:
		size_t current_size;
		FT_Face face;
	public:
		Font();
		~Font();

		bool allocate(const char *name);
		FontSize *get_size(size_t size);
		
		static void setup();

		static FT_Library library;

		const char *name;

		FT_Face use_size(size_t size);

		/*
		 * TODO: Make FontSize's destructor remove the reference in this hash table.
		 */
		FontSizeTable table;

	};
};
