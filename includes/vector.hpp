#ifndef VECTOR_H
#define VECTOR_H

#include <memory>
#include "iterators.hpp"
#include "reverse_iterator.hpp"
#include "lexico_compare.hpp"
#include "enable_if.hpp"
#include "is_integral.hpp"
#include "iterator_traits.hpp"
#include <stdexcept>
#include <iostream>

namespace ft
{

template<class T, class Allocator = std::allocator<T> >
class vector
{
	public:
	
	typedef T value_type;
	typedef Allocator allocator_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef typename Allocator::pointer pointer;
	typedef const typename Allocator::const_pointer const_pointer;
	typedef ft::vector_iter<std::random_access_iterator_tag, value_type> iterator;
	typedef ft::vector_iter<std::random_access_iterator_tag, value_type, difference_type, const T*, const T&> const_iterator;
	typedef ft::reverse_iterator<iterator> reverse_iterator;
	typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

	vector() :  _cap(0), a(Allocator()), _begin(NULL), _end(NULL) {};
	explicit vector(const allocator_type &alloc) : _begin(NULL), _end(NULL), a(alloc), _cap(0) {}
	explicit vector( size_type count, const T& value = T(), const Allocator& alloc = Allocator()) : _cap(count), a(alloc)
	{
		if (count > max_size())
			return;
		_begin = a.allocate(count);
		_end = _begin;
		size_type i;
		for (i = 0; i < count; i++)
			push_back(value);
	}
	template<class InputIt>
	vector(InputIt first, InputIt last, const Allocator& alloc = Allocator(), typename ft::enable_if<!ft::is_integral<InputIt>::value>::type* = 0) : a(alloc)
	{
		size_type needed_space = 0;
		for (InputIt tmp = first; tmp != last; tmp++, needed_space++)
			;
		_begin = a.allocate(needed_space);
		_cap = needed_space;
		_end = _begin;
		for (; first != last; first++)
			push_back(*first);
	}
	vector(const vector& other)
	{
		size_type count = other.size();
		if (count > 0)
		{
			if (count > max_size())
			return;
			a = other.get_allocator();
			_begin = a.allocate(count);
			_end = _begin;
			_cap = count;
			size_type i = 0;
			for (; i < count; i++)
				push_back(other[i]);
		}
		else
		{
			_cap = 0;
			a = Allocator(); 
			_begin = NULL;
			_end = _begin;
		}
	}
	~vector()
	{
		clear();
		a.deallocate(_begin, capacity());
	}
	vector& operator=(const vector& other)
	{
		if (other == *this)
			return (*this);
		size_type count = other.size();
		if (count > 0)
		{
			if (count > max_size())
			{
				std::cout << "Max size reached\n";
				return (*this);
			}
			clear();
			a.deallocate(_begin, capacity());
			if (count < capacity())
				_begin = a.allocate(capacity());
			else
			{
				_begin = a.allocate(count);
				_cap = count;
			}
			size_type i = 0;
			for (; i < count; i++)
				a.construct(&_begin[i], other[i]);
			_end = &_begin[i];
		}
		else
		{
			clear();
			a.deallocate(_begin, capacity());
			_cap = 0;
			a = Allocator(); 
			_begin = NULL;
			_end = _begin;
		}
		return (*this);
	}

	void assign(size_type count, const_reference value)
	{
		if (count <= capacity())
		{
			size_type s = size();
			std::fill_n(begin(), std::min(count, s), value);
			if (count > s)
				for (size_type i = size(); i < count; i++)
					push_back(value);
			else
				while (size() > count)
					pop_back();
		}
		else
		{
			clear();
			a.deallocate(_begin, capacity());
			_begin = a.allocate(count);
			_cap = count;
			size_type i;
			for (i = 0; i < count; i++)
			a.construct(&_begin[i], value);
			_end = &_begin[i];
		}
	}

	template<class InputIt>
	void assign(InputIt first, InputIt last, typename ft::enable_if<!ft::is_integral<InputIt>::value>::type* = 0)
	{
		if (capacity() < static_cast<size_type>(ft::distance(first, last)))
			reserve(ft::distance(first, last));
		clear();
		while (first != last)
		{
			push_back(*first);
			first++;
		}
	}

	allocator_type get_allocator() const { return (a); }

	reference at(size_type pos)
	{
		if (pos >= size())
			throw std::out_of_range("Element is out of range");
		return (_begin[pos]);
	}

	reference at(size_type pos) const
	{
		if (pos >= size())
			throw std::out_of_range("Element is out of range");
		else
			return (_begin[pos]);
	}

	reference operator[](size_type pos) { return (_begin[pos]); }
	reference operator[](size_type pos) const { return (_begin[pos]); }

	reference front() { return (*_begin); }
	reference front() const { return (*_begin); }

	reference back()
	{
		pointer tmp = _end;
		return (*--tmp);
	}
	reference back() const 
	{
		pointer tmp = _end;
		return (*--tmp);
	}

	pointer data()
	{
		if (size() == 0)
			return (NULL);
		else
			return (_begin);
	}

	const_pointer data() const
	{
		if (size() == 0)
			return (NULL);
		else
			return (_begin);
	}

	iterator begin() { return (iterator(_begin)); }
	const_iterator begin() const { return (const_iterator(_begin)); }

	iterator end() { return (iterator(_end)); }
	const_iterator end() const { return (const_iterator(_end)); }

	reverse_iterator rbegin() { return (reverse_iterator(end())); }
	const_reverse_iterator rbegin() const { return (const_reverse_iterator(end())); }

	reverse_iterator rend() { return (reverse_iterator(begin())); }
	const_reverse_iterator rend() const { return (const_reverse_iterator(begin())); }

	bool empty() const { return (begin() == end()); }

	size_type size() const 
	{
		/*if (_begin == NULL && _end == NULL)
			return (0);
		else*/
			return (ft::distance<const_iterator>(begin(), end()));
	}

	size_type max_size() const { return (a.max_size()); }	

	void reserve(size_type newCap)
	{
		if (newCap > max_size())
			throw std::length_error("vector::reserve");
		else if (_cap == 0)
		{
			if (_begin != NULL)
				a.deallocate(_begin, 1);
			_begin = a.allocate(newCap);
			_cap = newCap;
			_end = _begin;
		}
		else if (newCap > capacity())
		{
			pointer _new = a.allocate(newCap);
			size_type i = 0;
			for (; i < size(); i++)
				a.construct(&_new[i], _begin[i]);
			clear();
			a.deallocate(_begin, capacity());
			_cap = newCap;
			_begin = _new;
			_end = &_new[i];
		}
	}

	size_type capacity() const
	{
		return (_cap);
	}

	void clear()
	{
		while (_begin < _end)
			pop_back();
	}

	iterator insert( iterator pos, const T& value )
	{
		size_type pos_index = ft::distance(begin(), pos);
		if (capacity() == 0)
		{
			*this = vector(1, value);
			return (begin());
		}

		insert(pos, 1, value);

		return (iterator(&_begin[pos_index]));
	}

	void insert( iterator pos, size_type count, const T& value )
	{
		size_type pos_index = ft::distance(begin(), pos);
		if (size() + count > capacity())
		{
			(count > size()) ? reserve(size() + count) : reserve(size() * 2);
			pos = iterator(&_begin[pos_index]);
		}

		vector tmp(*this);
		int constructs_needed = count;
		for (size_type i = 0; pos != end() && i < count ; pos++, i++)
			*pos = value;
		size_type i = pos_index;
		if (pos != end()) //tous les éléments sont insérés sans construction
		{
			for (; pos != end(); pos++, i++)
				*pos = tmp[i];
		}
		else //certains éléments insérés doivent être construits
		{
			for (size_type y = pos_index + count - size(); y > 0; pos++, y--, constructs_needed--)
				push_back(value);
		}
		for (; constructs_needed > 0; constructs_needed--, i++, pos++)
			push_back(tmp[i]);
	}

	template< class InputIt >
	void insert( iterator pos, InputIt first, InputIt last, typename ft::enable_if<!ft::is_integral<InputIt>::value>::type* = 0)
	{
		size_type count = ft::distance(first, last);
		size_type pos_index = ft::distance(begin(), pos);

		if (size() + count > capacity())
		{
			(count > size()) ? reserve(size() + count) : reserve((size() + 1) * 2);
			pos = iterator(&_begin[pos_index]);
		}

		vector tmp(*this);
		int constructs_needed = count;

		for (; first != last && pos != end(); first++, pos++)
			*pos = *first;
		size_type i = pos_index;
		if (first == last && pos != end()) //tous les éléments sont insérés sans construction
		{
			for (; pos != end(); pos++, i++)
				*pos = tmp[i];
		}
		else //certains éléments insérés doivent être construits
		{
			for (size_type i = pos_index + count - size(); i > 0; i--, constructs_needed--, first++)
				push_back(*first);
		}
		for (; constructs_needed > 0; constructs_needed--, i++)
			push_back(tmp[i]);
	}

	iterator erase(iterator pos)
	{
		if (pos == end())
		{
			//std::cout << "iterator out of range\n";
			return (pos);
		}
		vector tmp = *this;
		size_type pos_index = ft::distance(begin(), pos);
		for (int i = pos_index; &_begin[i + 1] != _end; i++)
			_begin[i] = tmp[i + 1];
		pop_back();
		return (iterator(&_begin[pos_index]));
	}

	iterator erase(iterator first, iterator last)
	{
		if (first >= last)
		{
			//std::cout << "invalid iterator range\n";
			return (last);
		}
		size_type start = ft::distance(begin(), first);
		size_type finish = ft::distance(begin(), last);
		int i = finish - start;
		while (i > 0)
		{
			first = erase(first);
			i--;
		}
		return (first);
	}

	void push_back(const_reference value)
	{
		if (size() + 1 > capacity())
		{
			if (size() != 0)
				reserve(size() * 2);
			else
				reserve(1);
		}
		a.construct(_end, value);
		_end++;
	}

	void pop_back()
	{
		_end--;
		a.destroy(_end);
	}

	void resize(size_type count, T value = T())
	{
		if (size() > count)
		{
			while (size() != count)
				pop_back();
		}
		else if (size() < count)
		{
			if (capacity() * 2 > count)
				reserve(capacity());
			else
				reserve(count);
			while (size() != count)
				push_back(value);
		}
	}

	void swap(vector &other)
	{
		size_t tmpcap = _cap;
		pointer tmpbegin = _begin;
		pointer tmpend = _end;

		this->_cap = other._cap;
		this->_begin = other._begin;
		this->_end = other._end;

		other._cap = tmpcap;
		other._begin = tmpbegin;
		other._end = tmpend;
	}

	friend bool operator==( const ft::vector<T,Allocator>& lhs, const ft::vector<T,Allocator>& rhs )
	{
		const_iterator it1 = lhs.begin();
		const_iterator it2 = rhs.begin();
		for (; it1 != lhs.end() && it2 != rhs.end() && *it1 == *it2; it1++, it2++)
			;
		if (it1 == lhs.end() && it2 == rhs.end())
			return (true);
		else
			return (false);
	}

	friend bool operator!=( const ft::vector<T,Allocator>& lhs, const ft::vector<T,Allocator>& rhs )
	{
		if (!(lhs == rhs))
			return (true);
		else
			return (false);
	}

	friend bool operator<( const ft::vector<T,Allocator>& lhs, const ft::vector<T,Allocator>& rhs )
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	friend bool operator<=( const ft::vector<T,Allocator>& lhs, const ft::vector<T,Allocator>& rhs )
	{
		if (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) == false)
			return (lhs == rhs);
		else
			return (true);
	}

	friend bool operator>( const ft::vector<T,Allocator>& lhs, const ft::vector<T,Allocator>& rhs )
	{
		if (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) == true)
			return (false);
		else
			return (lhs != rhs);
	}

	friend bool operator>=( const ft::vector<T,Allocator>& lhs, const ft::vector<T,Allocator>& rhs )
	{
		if (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) == true)
			return (false);
		else
			return (true);
	}



	private:
		size_t _cap;
		allocator_type a;
		pointer _begin;
		pointer _end;
};
}

template< class T, class Alloc >
void swap( ft::vector<T,Alloc>& lhs, ft::vector<T,Alloc>& rhs )
{
	lhs.swap(rhs);
}

#endif