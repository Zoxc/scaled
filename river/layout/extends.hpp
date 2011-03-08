#pragma once
#include "../list.hpp"

namespace River
{
	struct Extends
	{
	public:
		Extends(int left, int top, int right, int bottom);
		Extends(const Extends *other);
		
		void union_with(const Extends *other);

		int left;
		int top;
		int right;
		int bottom;

		struct Accessors
		{
			int Extends::*primary_prefix;
			int Extends::*primary_postfix;
			int Extends::*secondary_prefix;
			int Extends::*secondary_postfix;
		};
		
		static const Accessors accessors[2];
	};
};
