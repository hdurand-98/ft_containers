#ifndef MAP_H
#define MAP_H

#include <memory>
#include "iterators.hpp"
#include "reverse_iterator.hpp"
#include "lexico_compare.hpp"
#include "enable_if.hpp"
#include "is_integral.hpp"
#include "iterator_traits.hpp"
#include <iostream>
#include "less.hpp"
#include "pairs.hpp"
#include "tree.hpp"
#include <stdexcept>
#include <functional>
#include <limits>

namespace ft
{

template< class Key, class T, class Compare = ft::less<Key>, class Allocator = std::allocator<ft::pair<const Key, T> > > 
class map
{
	public:

	typedef Key key_type;
	typedef T mapped_type;
	typedef ft::pair<const Key, T> value_type;
	typedef size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef Compare key_compare;
	typedef Allocator allocator_type;
	typedef typename Allocator::pointer pointer;
	typedef const typename Allocator::const_pointer const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef typename ft::Tree<value_type>::iterator iterator;
	typedef typename ft::Tree<value_type>::const_iterator const_iterator;
	typedef typename ft::Tree<value_type>::reverse_iterator reverse_iterator;
	typedef typename ft::Tree<value_type>::const_reverse_iterator const_reverse_iterator;

	class value_compare : std::binary_function<value_type, value_type, bool>
	{
		public:
		friend class map;
		
		protected:
		Compare comp;
		value_compare( Compare c ) : comp(c) {}

		public:
		bool operator()( const value_type& lhs, const value_type& rhs ) const { return (comp(lhs.first, rhs.first)); }

	};

	map() : t(ft::Tree<value_type, key_compare>()), a(Allocator())  {}

	template<class InputIt>
	map(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) : t(ft::Tree<value_type, key_compare>()), a(alloc), c(comp)
	{
		insert(first, last);
	}

	map(const map &other) : t(ft::Tree<value_type, key_compare>())
	{
		a = other.get_allocator();
		insert(other.begin(), other.end());
	}

	~map()
	{
		clear();
	}

	map& operator=(const map& other)
	{
		if (this == &other)
			return (*this);
		clear();
		a = other.get_allocator();
		insert(other.begin(), other.end());
		return (*this);
	}

	allocator_type get_allocator() const { return (a); }

	T& at (const Key& key)
	{
		ft::node<value_type> ret = t.findNode(key);
		if (ret == NULL)
			throw std::out_of_range("Element is out of range");
		return (ret.value.second);
	}

	T& operator[](const Key& key)
	{
		iterator ret = insert(ft::make_pair(key, T())).first;
		return (ret->second);
	}

	iterator begin()
	{
		return (t.begin());
	}

	const_iterator begin() const
	{
		return (t.begin());
	}

		iterator end()
	{
		return (t.end());
	}

	const_iterator end() const
	{
		return (t.end());
	}

	reverse_iterator rbegin()
	{
		return (t.rbegin());
	}

	const_reverse_iterator rbegin() const 
	{
		return (t.rbegin());
	}

	reverse_iterator rend()
	{
		return (t.rend());
	}

	const_reverse_iterator rend() const 
	{
		return (t.rend());
	}

	bool empty() const
	{
		if (t.get_size() == 0)
			return (true);
		else
			return (false);
	}

	size_type size() const
	{
		return (t.get_size());
	}

	size_type max_size() const { return (t.max_size()); }

	void clear()
	{
		erase(begin(), end());
	}

	ft::pair<iterator, bool> insert( const value_type& value )
	{
		bool isAdded = t.addValue(value);
		return (ft::make_pair(iterator(t.findNode(value.first)), isAdded));
	}

	iterator insert(iterator hint, const value_type& value)
	{
		(void)hint;
		return (insert(value).first);
	}

	template<class InputIt>
	void insert(InputIt first, InputIt last)
	{
		while (first != last)
		{
			insert(*first);
			first++;
		}
	}

	void erase(iterator pos)
	{
		t.delValue(pos->first);
	}

	void erase(iterator first, iterator last)
	{
		if (first == NULL || last == NULL)
			return;
		if (t.findNode(first->first) == NULL || (t.findNode(last->first) == NULL && last != end()))
			return ;

		while (first != last)
		{
			iterator tmp = first;
			first++;
			erase(tmp);
		}
	}

	size_type erase(const Key& key)
	{
		return (t.delValue(key));
	}

	void swap(map &other)
	{
		allocator_type tmp_alloc = a;

		this->a = other.a;
		other.a = tmp_alloc;

		t.swap(other.t);
	}

	size_type count(const Key& key) const
	{
		if (t.findNode(key) == NULL)
			return (0);
		else
			return (1);
	}

	iterator find(const Key& key)
	{
		ft::node<value_type> *result = t.findNode(key);
		if (result == NULL)
			return (end());
		else
			return (iterator(result));
	}

	const_iterator find(const Key& key) const
	{
		ft::node<value_type> *result = t.findNode(key);
		if (result == NULL)
			return (end());
		else
			return (const_iterator(result));
	}

	ft::pair<iterator, iterator> equal_range(const Key& key)
	{
		return (ft::make_pair(lower_bound(key), upper_bound(key)));
	}

	ft::pair<const_iterator, const_iterator> equal_range(const Key &key) const
	{
		return (ft::make_pair(lower_bound(key), upper_bound(key)));
	}

	iterator lower_bound(const Key& key)
	{
		ft::node<value_type> *n = t.findNode(key);
		if (n == NULL)
			return (upper_bound(key));
		else
			return (iterator(n));
	}

	const_iterator lower_bound(const Key& key) const
	{
		ft::node<value_type> *n = t.findNode(key);
		if (n == NULL)
			return (upper_bound(key));
		else
			return (const_iterator(n));
	}

	iterator upper_bound(const Key& key)
	{
		iterator ptr = t.begin();
		while (ptr->first <= key && ptr != t.end())
			ptr++;
		return (ptr);
	}

	const_iterator upper_bound(const Key& key) const
	{
		const_iterator ptr = t.begin();
		while (ptr->first <= key && ptr != t.end())
			ptr++;
		return (ptr);
	}

	key_compare key_comp() const {return (c);}

	value_compare value_comp() const {return (value_compare(key_compare()));}

	friend bool operator==( const ft::map<Key, T, Compare, Allocator>& lhs, const ft::map<Key, T, Compare, Allocator>& rhs )
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

	friend bool operator!=( const ft::map<Key, T, Compare, Allocator>& lhs, const ft::map<Key, T, Compare, Allocator>& rhs )
	{
		if (!(lhs == rhs))
			return (true);
		else
			return (false);
	}

	friend bool operator<( const ft::map<Key, T, Compare, Allocator>& lhs, const ft::map<Key, T, Compare, Allocator>& rhs )
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	friend bool operator<=( const ft::map<Key, T, Compare, Allocator>& lhs, const ft::map<Key, T, Compare, Allocator>& rhs )
	{
		if (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) == false)
			return (lhs == rhs);
		else
			return (true);
	}

	friend bool operator>( const ft::map<Key, T, Compare, Allocator>& lhs, const ft::map<Key, T, Compare, Allocator>& rhs )
	{
		if (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) == true)
			return (false);
		else
			return (lhs != rhs);
	}

	friend bool operator>=( const ft::map<Key, T, Compare, Allocator>& lhs, const ft::map<Key, T, Compare, Allocator>& rhs )
	{
		if (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) == true)
			return (false);
		else
			return (true);
	}

	private:
	ft::Tree<value_type, key_compare> t;
	allocator_type a;
	key_compare c;
};

template< class Key, class T, class Compare, class Alloc >
void swap(ft::map<Key,T,Compare,Alloc>& lhs, ft::map<Key,T,Compare,Alloc>& rhs)
{
	lhs.swap(rhs);
}

}

#endif