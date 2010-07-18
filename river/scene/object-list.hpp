#pragma once
#include "../gles.hpp"
#include "../list.hpp"
#include "object.hpp"

namespace River
{
	template<class T> class ObjectList:
		public RIVER_LIST(T, entry)
	{
	public:
		void render()
		{
			for(ObjectList::Iterator i = begin(); i; i++)
			{
				i().render();
			}
		}
	};
};
