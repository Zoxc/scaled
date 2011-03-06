#pragma once
#include <new>
#include <stddef.h>

namespace River
{
	class ContentSerializer;

	class ContentMeasurer
	{
	private:
		size_t size;
	public:
		ContentMeasurer();

		void count_lists(size_t count);

		template<class T> void count_objects(size_t count)
		{
			size += sizeof(T) * count;
		}
		
		size_t get_size();
	};

	class ContentSerializer
	{
	private:
		char *position;
	public:
		ContentSerializer(void *memory);

		void write_list(size_t list_entries);
		char *get_position();
		
		template<class T> T &write_object()
		{
			T *result = new ((void *)position) T;
			position += sizeof(T);
			return *result;
		}
	};
	
	class ContentWalker
	{
	private:
		char *position;
	public:
		ContentWalker(void *memory) : position((char *)memory)
		{
		}

		template<class T> T &read_object()
		{
			T *result = (T *)position;
			position += sizeof(T);
			return *result;
		}
		
		template<class T> T &peek_object()
		{
			return *(T *)position;
		}

		template<class T> class Iterator
		{
		private:
			ContentWalker &walker;
			size_t count;
			T *current;
		public:
			Iterator(ContentWalker &walker) : walker(walker)
			{
				count = walker.read_object<size_t>();
			}
			
			bool get_next()
			{
				if(count > 0)
				{
					count--;
					current = &walker.read_object<T>();
					return true;
				}
				else
					return false;
			}
			
			T &operator ()() const
			{
				return *current;
			}
		};
		
		template<class T> Iterator<T> read_list()
		{
			return Iterator<T>(*this);
		}
	};
};
