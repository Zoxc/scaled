#pragma once
#include "../gles.hpp"
#include "../list.hpp"
#include "object.hpp"

namespace River
{
	class ObjectList:
		public RIVER_LIST(Object, entry)
	{
	public:
		virtual void render();
	};
	
	class ObjectHash:
		public ObjectList
	{
	public:
	};
};
