#pragma once
#include <assert.h>

namespace River
{
	template<class T> class SimpleEntry
	{
	public:
		SimpleEntry() : next(0) {}

		T *next;
	};

	template<class T, size_t offset> class SimpleList
	{
	public:
		SimpleList() : first(0), last(0) {}
		
		T *first;
		T *last;

		static SimpleEntry<T> &get_entry(T *node)
		{
			return *reinterpret_cast<SimpleEntry<T> *>((size_t)node + offset);
		}

		void append(T *node)
		{
			if(!node) 
				assert(0);

			SimpleEntry<T> &entry = get_entry(node);

			entry.next = 0;

			if(last)
			{
				SimpleEntry<T> &last_entry = get_entry(last);

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
			Iterator(SimpleList<T, offset> &list) : current(list.first) {}

			void step()
			{
				SimpleEntry<T> &entry = SimpleList<T, offset>::get_entry(current);
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

	#define RIVER_SIMPLE_LIST(classname, field) \
		SimpleList<classname, (size_t)(&((classname *)0)->field)>
};
