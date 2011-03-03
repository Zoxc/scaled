#pragma once
#include <vector>
#include "../list.hpp"

namespace River
{
	class ContentWalker;

	class Layer
	{
	public:
		class Content
		{
			public:
				virtual void render(ContentWalker &walker) = 0;
				virtual void deallocate(ContentWalker &walker) = 0;
		};

	private:
		void *serialized;
		Content *serialized_end;

	public:
		Layer(void *serialized, size_t size);
		~Layer();
		
		Entry<Layer> window_entry;

		void append(Content *content);
		void render();
	};
};
