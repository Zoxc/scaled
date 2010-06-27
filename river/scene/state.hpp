#pragma once
#include "../list.hpp"
#include "layer.hpp"

namespace River
{
	class State
	{
	public:
		Entry<State> scene_entry;
		
		virtual void use() = 0;
		virtual void alloc() = 0;
		virtual void free() = 0;
	};
};
