#pragma once
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>

namespace River
{
	class RefObject
	{
	private:
		size_t ref_count;

	public:
		RefObject() : ref_count(0) {}
		virtual ~RefObject() {}

		size_t ref()
		{
			return ++ref_count;
		}

		size_t deref()
		{
			assert(ref_count != 0);

			size_t result = --ref_count;

			if(result == 0)
				delete this;

			return result;
		}
	};
};
