#ifndef ENABLE_IF_H
#define ENABLE_IF_H

namespace ft
{
	template<bool B, class T = void>
	struct enable_if {};

	template<class T>
	struct enable_if<true, T> { typedef T type; };

	template <bool isConst, typename T>
	struct enable_if_const
	{
		typedef typename T::value_type value_type;
	};

	template <typename T>
	struct enable_if_const<true, T>
	{
		typedef const typename T::value_type value_type;
	};
}

#endif