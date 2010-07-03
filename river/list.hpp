#pragma once
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>

namespace River
{
	template<class T> class Entry
	{
	public:
		Entry() : next(0), prev(0) {}

		T *next;
		T* prev;
	};

	template<class T, size_t offset> class List
	{
	public:
		List() : first(0), last(0) {}
		
		T *first;
		T *last;

		static Entry<T> &get_entry(T *node)
		{
			return *reinterpret_cast<Entry<T> *>((size_t)node + offset);
		}

		void append(T *node)
		{
			if(!node) 
				assert(0);

			Entry<T> &entry = get_entry(node);

			entry.next = 0;

			if(last)
			{
				Entry<T> &last_entry = get_entry(last);

				entry.prev = last;
				last_entry.next = node;
				last = node;
			}
			else
			{
				first = node;
				last = node;
			}
		}

		class Iterator
		{
		private:
			T *current;

		public:
			Iterator(List<T, offset> &list) : current(list.first) {}

			void step()
			{
				Entry<T> &entry = List<T, offset>::get_entry(current);
				current = entry.next;
			}

			operator bool()
			{
				return current != 0;
			}

			T &operator ++()
			{
				step();
				return *current;
			}

			T &operator ++(int)
			{
				T *result = current;
				step();
				return *result;
			}

			T *operator*()
			{
				return current;
			}

			T &operator ()()
			{
				return *current;
			}
		};

		Iterator begin()
		{
			return Iterator(*this);
		}
	};

	#define RIVER_LIST(classname, field) List<classname, offsetof(classname, field)>
};
