#pragma once
#include "../list.hpp"

namespace River
{
	class Layer;

	class Object
	{
	public:
		Entry<Object> entry;
		
		void render();
	};
};
