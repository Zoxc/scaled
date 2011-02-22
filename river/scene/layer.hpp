#pragma once
#include <vector>
#include "../list.hpp"

namespace River
{
	class Layer
	{
	public:
		class Content
		{
			public:
				virtual void render() = 0;

				virtual ~Content()
				{
				}
		};

	private:
		std::vector<Content *> list;

	public:
		Layer();
		~Layer();
		
		Entry<Layer> window_entry;

		void append(Content *content);
		void render();
	};
};
