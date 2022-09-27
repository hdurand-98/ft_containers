#ifndef LEXICO_COMPARE_H
#define LEXICO_COMPARE_H

namespace ft
{

template< class InputIt1, class InputIt2 >
bool lexicographical_compare( InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2 )
{
	while (first1 != last1 && first2 != last2)
	{
		if (*first1 == *first2)
			;
		else
		{
			if (*first1 < *first2)
				return (true);
			else
				return (false);
		}
		first1++;
		first2++;
	}
	if (first1 == last1 && first2 != last2)
		return (true);
	else
		return (false);
}

template< class InputIt1, class InputIt2, class Compare >
bool lexicographical_compare( InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, Compare comp )
{
	while (first1 != last1 && first2 != last2)
	{
		if (*first1 == *first2)
			;
		else
			return (comp(*first1, *first2));
		first1++;
		first2++;
	}
	if (first1 == last1)
		return (true);
	else
		return (false);
}

}

#endif