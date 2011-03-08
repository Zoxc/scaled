#include "stack.hpp"

namespace River
{
	Stack::Stack() : orientation(Vertical)
	{
	}

	void Stack::set_orientation(Orientation orientation)
	{
		this->orientation = orientation;
	}

	template<Orientation using_orientation> void Stack::generic_layout(int available_primary, int available_secondary)
	{
		const Extends::Accessors &extend_fields = Extends::accessors[orientation];
		const Element::Rect::Accessors &rect_fields = Element::Rect::accessors[orientation];
		const Element::Accessors &element_fields = Element::accessors[orientation];

		int primary = available_primary;
		Element *element = children.first;
		ExtendList extends;
		Element *line_start = element;
		Element *last_visible = 0;
		
		int margin_prefix = padding->*extend_fields.primary_prefix;

		while(element)
		{
			Extends margins(padding);

			margins.*extend_fields.primary_prefix = margin_prefix;

			margins.union_with(element->margins);
			
			int element_secondary = std::max(available_secondary - (margins.*extend_fields.secondary_prefix + margins.*extend_fields.secondary_postfix), 0);
			
			/*
			 * Delay layout of primary extended elements until we know the actual size of it.
			 */
			if(element->*element_fields.primary_length == Flags::Extend)
			{
				/*
				 * Use the minimum size as a placeholder in calculations.
				 */
				element->rect.*rect_fields.primary_length = element->*element_fields.primary_min_length;
				element->rect.*rect_fields.secondary_length = element_secondary;
				
				/*
				 * Add primary extended elements to the list.
				 */
				extends.append(element);
			}
			else
			{
				int element_primary = std::max(primary - (margins.*extend_fields.primary_prefix + margins.*extend_fields.primary_postfix), 0);

				element->generic_layout<using_orientation>(element_primary, element_secondary);
			}

			int element_span = element->rect.*rect_fields.primary_length + margins.*extend_fields.primary_prefix;

			primary -= element_span;
			margin_prefix = margins.*extend_fields.primary_postfix;

			last_visible = element;
			
			element = element->children_entry.next;
		}
		
		/*
		 * Include the postfix margin of the last element in the primary length calculation.
		 */
		if(last_visible)
			primary -= std::max(padding->*extend_fields.primary_postfix, last_visible->margins->*extend_fields.primary_postfix);
		else
			primary -= padding->*extend_fields.primary_postfix;

		/* 
		 * Second pass, extend elements in the primary direction.
		 */
		if(extends.first)
		{
			/* 
			 * Calculate the total element weight.
			 */
			size_t weight = 0;

			for(ExtendList::Iterator i = extends.begin(); i != extends.end(); ++i)
				weight += i().weight;

			/*
			 * Layout elements.
			 */
			for(ExtendList::Iterator i = extends.begin(); i != extends.end(); ++i)
			{
				i().rect.*rect_fields.primary_length += primary * i().weight / weight;
				i().layout(i().rect.width, i().rect.height);
			}
			
			/* 
			 * We used up all our space now.
			 */
			primary = 0;
		}

		/*
		 * Third pass, position all the elements.
		 */

		element = line_start;
		margin_prefix = padding->*extend_fields.primary_prefix;
		int max_secondary = 0;
		int primary_offset = 0;

		while(element)
		{
			Extends margins(padding);

			margins.*extend_fields.primary_prefix = margin_prefix;

			margins.union_with(element->margins);

			primary_offset += margins.*extend_fields.primary_prefix;

			element->rect.*rect_fields.secondary_offset = margins.*extend_fields.secondary_prefix;
			element->rect.*rect_fields.primary_offset = primary_offset;

			primary_offset += element->rect.*rect_fields.primary_length;

			margin_prefix = margins.*extend_fields.primary_postfix;

			max_secondary = std::max(max_secondary, margins.*extend_fields.secondary_prefix + element->rect.*rect_fields.secondary_length + margins.*extend_fields.secondary_postfix);

			element = element->children_entry.next;
		}

		generic_simple_layout<using_orientation>(available_primary, available_secondary, available_primary - primary, max_secondary);
	}

	void Stack::layout(int available_width, int available_height)
	{
		if(orientation == Vertical)
			generic_layout<Vertical>(available_height, available_width);
		else
			generic_layout<Horizontal>(available_width, available_height);
	}
};
