#ifndef REVERSE_ITERATOR_H
#define REVERSE_ITERATOR_H

#include <iostream>
#include "iterator_traits.hpp"

namespace ft
{

template <class Iterator>
class reverse_iterator
{
	public:
	typedef Iterator iterator_type;
	typedef typename ft::iterator_traits<Iterator>::iterator_category iterator_category;
	typedef typename ft::iterator_traits<Iterator>::value_type value_type;
	typedef typename ft::iterator_traits<Iterator>::difference_type difference_type;
	typedef typename ft::iterator_traits<Iterator>::pointer pointer;
	typedef typename ft::iterator_traits<Iterator>::reference reference;

	reverse_iterator() : current() {}
	explicit reverse_iterator(iterator_type it) : current(it) {}
	template<class Iter>
	reverse_iterator(const reverse_iterator<Iter>& it) : current(it.base()) {}
	virtual ~reverse_iterator() {}
	template<class Iter>
	reverse_iterator &operator=(const reverse_iterator<Iter> &it) { current = it.base(); return (*this); }

	iterator_type base() const { return (current); }
	
	reference operator*() const 
	{
		iterator_type tmp = current;
		return (*(--tmp));
	}

	pointer operator->() const { return (&(operator*())); }

	reverse_iterator &operator++() 
	{
		--current;
		return (*this);
	}

	reverse_iterator operator++(int)
	{
		reverse_iterator tmp(*this);
		--current;
		return (tmp);
	}

	reverse_iterator& operator--()
	{
		++current;
		return (*this);
	}

	reverse_iterator operator--(int)
	{
		reverse_iterator tmp(*this);
		current++;
		return (tmp);
	}

	reverse_iterator operator+(difference_type n) { return (reverse_iterator(base() - n)); }
	reverse_iterator operator+=(difference_type n) { current -= n; return (*this); }
	reverse_iterator operator-(difference_type n) { return (reverse_iterator(base() + n)); }
	reverse_iterator operator-=(difference_type n) { current += n; return (*this); }
	reference operator[](difference_type n) const { return (base()[-n - 1]); }

	friend reverse_iterator<Iterator> operator+( typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& it )
	{
		return (reverse_iterator<Iterator>(it.base() - n));
	}

	friend reverse_iterator<Iterator> operator-( typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& it )
	{
		reverse_iterator<Iterator> tmp(it);
		tmp -= n;
		return (tmp);
	}

protected:
	iterator_type current;
};

template <class It1, class It2>
bool operator==(const reverse_iterator<It1> &i1, const reverse_iterator<It2> &i2) { return (i1.base() == i2.base()); }
template <class It1, class It2>
bool operator!=(const reverse_iterator<It1> &i1, const reverse_iterator<It2> &i2) { return (i1.base() != i2.base()); }
template <class It1, class It2>
bool operator>=(const reverse_iterator<It1> &i1, const reverse_iterator<It2> &i2) { return (i1.base() <= i2.base()); }
template <class It1, class It2>
bool operator>(const reverse_iterator<It1> &i1, const reverse_iterator<It2> &i2) { return (i1.base() < i2.base()); }
template <class It1, class It2>
bool operator<=(const reverse_iterator<It1> &i1, const reverse_iterator<It2> &i2) { return (i1.base() >= i2.base()); }
template <class It1, class It2>
bool operator<(const reverse_iterator<It1> &i1, const reverse_iterator<It2> &i2) { return (i1.base() > i2.base()); }

template<class It>
typename reverse_iterator<It>::difference_type operator-( const reverse_iterator<It>& lhs, const reverse_iterator<It>& rhs )
{
	return (rhs.base() - lhs.base());
}

template<class It1, class It2>
typename reverse_iterator<It1>::difference_type operator-( const reverse_iterator<It1>& lhs, const reverse_iterator<It2>& rhs )
{
	return (rhs.base() - lhs.base());
}

}

#endif 