#include "memory-pool.hpp"
#include <new>
#include <cstring>

#ifdef WIN32
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#else
	#include <sys/mman.h>
#endif

static const size_t memory_align = 8;

static inline size_t align(size_t value, size_t alignment)
{
	alignment -= 1;
	return (value + alignment) & ~alignment;
};

namespace River
{
	MemoryPool::~MemoryPool()
	{
		SimplerList<Page>::Iterator page = pages.begin();

		while(page != pages.end())
		{
			Page *current = *page;
			++page;
			free_page(current);
		};
	}
	
	MemoryPool::MemoryPool()
		: current(0),
		max(0)
	{
	}
		
	uint8_t *MemoryPool::allocate_page(size_t bytes)
	{
		uint8_t *result;
		
		#ifdef WIN32
			result = (uint8_t *)VirtualAlloc(0, bytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		#else	
			result = (uint8_t *)mmap(0, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		#endif
		
		assert(result != 0);

		Page *page = new ((void *)result) Page;

		#ifndef WIN32
			page->length = bytes;
		#endif

		pages.append(page);

		return result + sizeof(Page);
	}
	
	void MemoryPool::free_page(Page *page)
	{
		#ifdef WIN32
			VirtualFree((void *)page, 0, MEM_RELEASE);
		#else
			munmap((void *)page, page->length);
		#endif
	}
	
	void *MemoryPool::get_page(size_t bytes)
	{
		if(bytes > (max_alloc - sizeof(Page)))
			return allocate_page(bytes + sizeof(Page));

		uint8_t *result = allocate_page();

		max = result + (max_alloc - sizeof(Page));

		result = (uint8_t *)align((size_t)result, memory_align);

		current = result + bytes;
		
		return result;
	}
	
	void *MemoryPool::realloc(void *mem, size_t old_size, size_t new_size)
	{
		void *result = alloc(new_size);
			
		std::memcpy(result, mem, old_size);
			
		return result;
	}
	
	void *MemoryPool::alloc(size_t bytes)
	{
		uint8_t *result;

		#ifdef VALGRIND
			result = (uint8_t *)malloc(bytes);
			
			assert(result != 0);
		#else
			result = (uint8_t *)align((size_t)current, memory_align);

			uint8_t *next = result + bytes;
		
			if(next > max)
				return get_page(bytes);

			current = next;
		#endif

		return (void *)result;
	}
};

void *operator new(size_t bytes, River::MemoryPool &memory_pool) throw()
{
	return memory_pool.alloc(bytes);
}

void operator delete(void *, River::MemoryPool &memory_pool) throw()
{
}

void *operator new[](size_t bytes, River::MemoryPool &memory_pool) throw()
{
	return memory_pool.alloc(bytes);
}

void operator delete[](void *, River::MemoryPool &memory_pool) throw()
{
}
