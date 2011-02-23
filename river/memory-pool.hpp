#pragma once
#include <stdint.h>
#include "allocator.hpp"
#include "simpler-list.hpp"

namespace River
{
	class MemoryPool:
			public Allocator
	{
		struct Page
		{
			SimpleEntry<Page> entry;
			#ifndef WIN32
				size_t length;
			#endif
		};

		static const unsigned int max_alloc = 0x1000;

		private:
			uint8_t *current;
			uint8_t *max;

			SimplerList<Page> pages;

			uint8_t *allocate_page(size_t bytes = max_alloc);
			void free_page(Page *page);
			void *get_page(size_t bytes);
		public:
			typedef MemoryPool &Ref;
			typedef Allocator::Wrap<MemoryPool &> Storage;

			MemoryPool();
			~MemoryPool();
			
			void *alloc(size_t bytes);
			void *realloc(void *mem, size_t old_size, size_t new_size);
			void free(void *mem)
			{
			}
	};
};

void *operator new(size_t bytes, River::MemoryPool &memory_pool) throw();
void operator delete(void *, River::MemoryPool &memory_pool) throw();
void *operator new[](size_t bytes, River::MemoryPool &memory_pool) throw();
void operator delete[](void *, River::MemoryPool &memory_pool) throw();
