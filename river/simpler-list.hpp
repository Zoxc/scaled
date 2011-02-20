#pragma once
#include <cassert>
#include "simple-list.hpp"

namespace River
{
	template<class T, class E = T, SimpleEntry<E> E::*field = &E::entry> class SimplerList
	{
	public:
		SimplerList() : first(0) {}
		
		T *first;
		
		bool empty()
		{
			return first == 0;
		}
		
		void append(T *node)
		{
			assert(node != 0);
			
			(node->*field).next = first;
			first = node;
		}
		
		class Iterator
		{
		private:
			T *current;

		public:
			Iterator(T *start) : current(start) {}

			void step()
			{
				current = static_cast<T *>((current->*field).next);
			}
			
			bool operator ==(const Iterator &other) const 
			{
				return current == other.current;
			}
				
			bool operator !=(const Iterator &other) const 
			{
				return current != other.current;
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
			
			T *operator*() const
			{
				return current;
			}

			T &operator ()() const
			{
				return *current;
			}
		};

		Iterator begin()
		{
			return Iterator(first);
		}
		
		Iterator end()
		{
			return Iterator(0);
		}
	};
};
