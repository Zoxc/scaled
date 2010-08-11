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

	template<class T, class E, Entry<E> E::*field> class List
	{
	public:
		List() : first(0), last(0) {}
		
		T *first;
		T *last;

		void append(T *node)
		{
			if(!node) 
				assert(0);

			Entry<E> &entry = node->*field;

			entry.next = 0;

			if(last)
			{
				Entry<E> &last_entry = last->*field;

				entry.prev = static_cast<E *>(last);
				last_entry.next = static_cast<E *>(node);
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
			Iterator(List &list) : current(list.first) {}

			void step()
			{
				Entry<E> &entry = current->*field;
				current = static_cast<T *>(entry.next);
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
};
