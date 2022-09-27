#ifndef STACK_H
#define STACK_H

#include "vector.hpp"
#include <vector>

namespace ft 
{

template <class T, class Container = ft::vector<T> >
class stack
{
	public:

		typedef T value_type;
		typedef Container container_type;
		typedef typename Container::size_type size_type;

		explicit stack(const Container &cont = Container())
		{
			c = cont;
		}

		stack( const stack& other )
		{
			*this = other;
		}

		stack& operator=( const stack& other )
		{
			c = other.c;
		}

		bool empty() const { return (c.empty()); }
		size_type size() const { return (c.size()); }
		value_type &top() { return (c.back()); }
		const value_type &top() const { return (c.back()); }
		void push(const value_type &val) { return (c.push_back(val)); }
		void pop() { return (c.pop_back()); }

		friend bool operator== (const stack<T,Container>& lhs, const stack<T,Container>& rhs)
		{
			if (lhs.c == rhs.c)
				return (true);
			else
				return (false);
		}
		friend bool operator!= (const stack<T,Container>& lhs, const stack<T,Container>& rhs)
		{
			if (lhs.c != rhs.c)
				return (true);
			else
				return (false);
		}
  		friend bool operator<  (const stack<T,Container>& lhs, const stack<T,Container>& rhs)
		{
			if (lhs.c < rhs.c)
				return (true);
			else
				return (false);
		}
  		friend bool operator<= (const stack<T,Container>& lhs, const stack<T,Container>& rhs)
		{
			if (lhs.c <= rhs.c)
				return (true);
			else
				return (false);
		}
  		friend bool operator>  (const stack<T,Container>& lhs, const stack<T,Container>& rhs)
		{
			if (lhs.c > rhs.c)
				return (true);
			else
				return (false);
		}
  		friend bool operator>= (const stack<T,Container>& lhs, const stack<T,Container>& rhs)
		{
			if (lhs.c >= rhs.c)
				return (true);
			else
				return (false);
		}

	protected:
	Container c;
};

}

#endif