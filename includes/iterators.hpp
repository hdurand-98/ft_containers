#ifndef ITERATORS_H
#define ITERATORS_H

#include "iterator_traits.hpp"
#include "enable_if.hpp"
//#include "map.hpp"

namespace ft
{

template <class Category, class T, class Distance = std::ptrdiff_t, class Pointer = T*, class Reference = T&>
struct	iterator
{
	typedef	Category 	iterator_category;
	typedef	T			value_type;
	typedef	Distance	difference_type;
	typedef	Pointer		pointer;
	typedef	Reference	reference;
};

template <class Category, class T, class Distance = std::ptrdiff_t, class Pointer = T*, class Reference = T&>
class vector_iter
{
public:
    typedef Category iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;

	vector_iter() {}
    vector_iter(pointer ptr) : i(ptr) {}
    vector_iter(const vector_iter<Category, T, Distance, T*, T&> &ref) : i(ref.base()) {}
    vector_iter&	operator=(pointer ptr) { i = ptr; return *this; }
	vector_iter&	operator=(const vector_iter &ref) { i = ref.i; return *this; }

    reference operator*() const { return (*i); }

    pointer operator->() const 
    {
        return (&(operator*()));
    }

    vector_iter& operator++()
    {
        ++i;
        return (*this);
    }
    vector_iter operator++(int)
    {
        vector_iter tmp(*this);
        ++(*this);
        return (tmp);
    }
    vector_iter& operator--()
    {
        --i;
        return (*this);
    }
    vector_iter operator--(int)
    {
        vector_iter tmp(*this);
        --(*this);
        return (tmp);
    }
    vector_iter operator+(difference_type n)
    {
        vector_iter result(*this);
        result += n;
        return (result);
    }
    vector_iter& operator+=(difference_type n)
    {
        difference_type m = n;
        if (m >= 0)
            while (m--) ++i;
        else
            while (m++) --i;
        return (*this);
    }
    vector_iter operator-(difference_type n)
    {
        vector_iter result(*this);
        result -= n;
        return (result);
    }

    difference_type operator-(const vector_iter& other)
    {
        return (this->i - other.i);
    }

    vector_iter& operator-=(difference_type n)
    {
        i += -n;
        return (*this);
    }

    friend vector_iter	operator+(difference_type n, const vector_iter& it) { return vector_iter(it.i + n); }
    friend vector_iter	operator-(const vector_iter& it, difference_type n) { return vector_iter(it.i - n); }
	friend vector_iter	operator-(difference_type n, const vector_iter& it) { return vector_iter(it.i - n); }
    friend difference_type	operator-(const vector_iter& a, const vector_iter& b) { return (a.i - b.i); }

    reference operator[](difference_type n) { return (i[n]); }

    pointer base() const { return (i); }

    friend bool operator==(const vector_iter &i1, const vector_iter &i2) { return (i1.i == i2.i); }
    friend bool operator!=(const vector_iter &i1, const vector_iter &i2) { return (i1.i != i2.i); }
    friend bool operator>(const vector_iter &i1, const vector_iter &i2) { return (i1.i > i2.i); }
    friend bool operator>=(const vector_iter &i1, const vector_iter &i2) { return (i1.i >= i2.i); }
    friend bool operator<(const vector_iter &i1, const vector_iter &i2) { return (i1.i < i2.i); }
    friend bool operator<=(const vector_iter &i1, const vector_iter &i2) { return (i1.i <= i2.i); }

private:
    pointer i;
};

template<typename T, class Pointer, class Reference, bool isConst>
class map_iter : private ft::iterator<std::bidirectional_iterator_tag, T>
{
    public:

    typedef typename ft::enable_if_const<isConst, T>::value_type  value_type;
    typedef typename ft::iterator<std::bidirectional_iterator_tag, value_type>::pointer           pointer;
	typedef typename ft::iterator<std::bidirectional_iterator_tag, value_type>::reference			reference;
	typedef typename ft::iterator<std::bidirectional_iterator_tag, value_type>::difference_type	difference_type;
	typedef typename ft::iterator<std::bidirectional_iterator_tag, value_type>::iterator_category	iterator_category;

    map_iter() {}
    map_iter(T* ptr) : i(ptr) {}
    template<typename type, class ptr, class rf, bool c>
    map_iter(const map_iter<type, ptr, rf, c> &ref, typename ft::enable_if<!c>::type* = 0) : i(ref.base()) {}
    map_iter&	operator=(T* ptr) { i = ptr; return *this; }
	map_iter&	operator=(const map_iter &ref) { i = ref.i; return *this; }

    reference operator*() const { return (i->value); }

    pointer operator->() const { return (&operator*()); }

    map_iter& operator++()
    {
        if (i)
        {
            if (i->right)
            {
                i = i->right;
                while (i->left)
                    i = i->left;
            }
            else if (i->parent && i->parent->left && i == i->parent->left)
                i = i->parent;
            else if (i->parent)
            {
                while (i->parent && i->parent->right && i == i->parent->right)
                    i = i->parent;
                i = i->parent;
            }
        }

        return (*this);
    }
    map_iter operator++(int)
    {
        map_iter tmp(*this);
        ++(*this);
        return (tmp);
    }
    map_iter& operator--()
    {
        if (i->left)
        {
            i = i->left;
            while (i->right)
                i = i->right;
        }
        else if (i->parent && i == i->parent->right)
            i = i->parent;
        else if (i->parent)
        {
            while (i->parent && i == i->parent->left)
                i = i->parent;
            i = i->parent;
        }
        return (*this);
    }

    map_iter operator--(int)
    {
        map_iter tmp(*this);
        --(*this);
        return (tmp);
    }

    friend bool operator==(const map_iter &i1, const map_iter &i2) { return (i1.i == i2.i); }
    friend bool operator!=(const map_iter &i1, const map_iter &i2) { return (i1.i != i2.i); }

    T* base() const { return (i); }

    private:
        T* i;
};

template <class _RandIter>
std::ptrdiff_t distance(_RandIter first, _RandIter last)
{
    std::ptrdiff_t dst = 0;
    for (_RandIter tmp = first; tmp != last; tmp++, dst++)
        ;
    return (dst);
}
}

#endif