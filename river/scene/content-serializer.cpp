#include "content-serializer.hpp"

namespace River
{
	ContentMeasurer::ContentMeasurer() : size(0)
	{
	}

	void ContentMeasurer::count_lists(size_t count)
	{
		size += sizeof(size_t) * count;
	}

	size_t ContentMeasurer::get_size()
	{
		return size;
	}

	ContentSerializer::ContentSerializer(void *memory) : position((char *)memory)
	{
	}

	char *ContentSerializer::get_position()
	{
		return position;
	}

	void ContentSerializer::write_list(size_t list_entries)
	{
		size_t *result = (size_t *)position;
		*result = list_entries;
		position += sizeof(size_t);
	}
};
