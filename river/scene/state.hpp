#pragma once
#include "../list.hpp"
#include "layer.hpp"

namespace River
{
	class State
	{
	public:
		/*
		 * Hardcoded state types
		 */
		enum Type 
		{
			GradientState,
			StateCount
		};

		Entry<State> scene_entry;
		size_t index;
		
		virtual void use() = 0;
		virtual void alloc() = 0;
		virtual void free() = 0;
		virtual void size(int width, int height);
	};
};
