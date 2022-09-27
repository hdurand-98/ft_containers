#ifndef TREE_H
#define TREE_H

#include <memory>
#include <iostream>
#include <vector>
#include "less.hpp"
#include "pairs.hpp"
#include "iterators.hpp"
#include "reverse_iterator.hpp"

namespace ft
{

template<class T>
T max(T a, T b)
{
	return (a > b) ? a : b;
}

template<class T>
class node
{ 
	public :
	typedef T value_type;

	node *left;
	node *right;
	node *parent;
	T value;
	int height;

	node(T val) : left(NULL), right(NULL), parent(NULL), value(val), height(1) {}

	~node() {}

};

template<class T, class Compare = ft::less<typename T::first_type>, class Allocator = std::allocator<node<T> > >
class Tree
{
	public:

	typedef node<T> node;
	typedef std::ptrdiff_t difference_type;
	typedef typename Allocator::pointer pointer;
	typedef const typename Allocator::const_pointer const_pointer;
	typedef size_t size_type;
	typedef node& reference;
	typedef const node& const_reference;
	typedef ft::map_iter<node, T*, T&, false> iterator;
	typedef ft::map_iter<node, const T*, const T&, true> const_iterator;
	typedef ft::reverse_iterator<iterator> reverse_iterator;
	typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

	struct Trunk
	{
		Trunk *prev;
		std::string str;

		Trunk(Trunk *prev, std::string str)
		{
			this->prev = prev;
			this->str = str;
		}
	};

	Tree() : root(NULL), a(Allocator()), comp(Compare()), size(0)
	{
		_end = newNode(T());
		_end->height = 0;
	}

	~Tree()
	{
		a.destroy(_end);
		a.deallocate(_end, 1);
	}

	size_type get_size() const
	{
		return (size);
	}

	size_type max_size() const { return (a.max_size()); }

	int getBalance(node *n)
	{
		if (n == NULL)
			return (0);

		return (getHeight(n->left) - getHeight(n->right));
	}

	node *newNode(T val)
	{
		node *n = a.allocate(1);
		a.construct(n, node(val));

		return (n);
	}

	template<class Key>
	size_type delValue(const Key& value)
	{
		if (findNode(value) == NULL)
			return (0);
		root = deleteNode(root, value);
		size--;
		if (root != _end)
		{
			node *last = findLast();
			if (_end->parent)
				_end->parent->right = NULL;
			_end->parent = last;
			if (_end->parent)
				_end->parent->right = _end;
		}
		_end->height = 0;
		return (1);
	}

	bool addValue(T value)
	{
		node* result = findNode(value.first);
		if (result != NULL && result->height != 0)
			return (false);
		root = insert(root, value);
		node *last = findLast();
		if (value.first == last->value.first)
		{
			_end->parent = last;
			last->right = _end;
		}
		size++;
		return (true);
	}

	int getHeight(node *n)
	{
		if (n == NULL)
			return (0);
		return (n->height);
	}

	template<class key>
	node *findNode(key val) const
	{
		node* current = root;
		
		if (current == NULL)
			return (NULL);

		while (current->left != NULL || current->right != NULL)
		{
			if (val == current->value.first)
				return (current);
			else if (comp(val, current->value.first))
			{
				if (current->left)
					current = current->left;
				else
					break;
			}
			else
			{
				if (current->right)
					current = current->right;
				else
					break;
			}
		}
		if (current->value.first == val)
			return (current);
		else
			return (NULL);
	}

	node *insert(node *n, T value)
	{
		if (n == NULL || n->height == 0)
			return (newNode(value));

    	if (comp(value.first, n->value.first))
		{
			n->left = insert(n->left, value);
			n->left->parent = n;
		}
		else
		{
        	n->right = insert(n->right, value);
			n->right->parent = n;
		}
		
		n->height = 1 + max(getHeight(n->left), getHeight(n->right));

		int balance = getBalance(n);
		//std::cout << "balance = " << balance << std::endl;

		if (balance > 1 && comp(value.first, n->left->value.first))
        return rightRotate(n);
 
    	if (balance < -1 && comp(n->right->value.first, value.first))
        	return leftRotate(n);
 
    	if (balance > 1 && comp(n->left->value.first, value.first))
    	{
        	n->left = leftRotate(n->left);
        	return rightRotate(n);
    	}
 
    	if (balance < -1 && comp(value.first, n->right->value.first))
    	{
        	n->right = rightRotate(n->right);
        	return leftRotate(n);
    	}

		return (n);
	}

	void swapNodes(node* father, node* son)
	{
		node tmp = node(T());

		tmp.parent = father->parent;
		tmp.left = father->left;
		tmp.right = father->right;
		tmp.height = father->height;

		if (father->parent)
		{
			if (father->parent->left && father->parent->left->value.first == father->value.first)
				father->parent->left = son;
			else
				father->parent->right = son;
		}
		else 
			root = son;

		//le pere prend la place du fils
		father->parent = son;		
		if (son->left == NULL)
			father->left = NULL;
		else
		{
			father->left = son->left;
			father->left->parent = father;
		}
		if (son->right == NULL || son->right->height == 0)
			father->right = NULL;
		else
		{
			father->right = son->right;
			father->right->parent = father;
		}
		father->height = son->height;

		//le fils prend la place du pere
		son->parent = tmp.parent;
		if (tmp.left && son == tmp.left)
		{
			son->left = father;
			son->right = tmp.right;
			if (tmp.right)
				son->right->parent = son;
		}
		else
		{
			son->left = tmp.left;
			if (tmp.left)
				son->left->parent = son;
			son->right = father;
		}
		son->height = tmp.height;
	}

	template<class Key>
	node *deleteNode(node *n, const Key& value)
	{
		if (n == NULL)
			return n;

		if (comp(value, n->value.first) && n->height != 0)
			n->left = deleteNode(n->left, value);
		else
		{
			if (value == n->value.first && n->height != 0)
			{
				if ((n->left == NULL) || (n->right == NULL))
				{
					node *temp = n->left ? n->left : n->right;

					if (temp == NULL)
					{
						temp = n;
						n = NULL;
					}
					else
					{
						node *tmp;
						swapNodes(n, temp);
						tmp = n;
						n = temp;
						if (tmp->height == 0)
							n->height = 0;
						n->left = NULL;
						n->right = NULL;
						temp = tmp;
					}

					a.destroy(temp);
					a.deallocate(temp, 1);
				}
				else
				{
					if (n->right->left)
					{
						int i = 0;
						swapNodes(n, n->right);
						while (n->left)
						{
							swapNodes(n, n->left);
							i++;
						}
						while (i != 0)
						{
							swapNodes(n->parent->parent, n->parent);
							n = n->parent;
							i--;
						}
						n = n->parent;
						n->right = deleteNode(n->right, value);
					}
					else
					{
						if (n->right->height == 0)
						{
							swapNodes(n, n->left);
							n = n->parent;
							n->left = deleteNode(n->left, value);
							n->height = 1;
						}
						else
						{
							swapNodes(n, n->right);
							n = n->parent;
							n->right = deleteNode(n->right, value);
						}
					}
				}
			}
			else
				n->right = deleteNode(n->right, value);
		}

		if (n == NULL || n->height == 0)
			return n;

		n->height = 1 + max(getHeight(n->left), getHeight(n->right));

		int balance = getBalance(n);

		if (balance > 1 && getBalance(n->left) >= 0)
			return rightRotate(n);

		if (balance > 1 && getBalance(n->left) < 0)
		{
			n->left = leftRotate(n->left);
			return rightRotate(n);
		}

		if (balance < -1 && getBalance(n->right) <= 0)
			return leftRotate(n);

		if (balance < -1 && getBalance(n->right) > 0)
		{
			n->right = rightRotate(n->right);
			return leftRotate(n);
		}

		return n;
	}

	node *rightRotate(node *old_root)
	{
    	node *new_root = old_root->left;
    	node *right_leaf = new_root->right;
 
    	new_root->right = old_root;
    	old_root->left = right_leaf;

    	old_root->height = max(getHeight(old_root->left), getHeight(old_root->right)) + 1;
    	new_root->height = max(getHeight(new_root->left), getHeight(new_root->right)) + 1;

		new_root->parent = old_root->parent;
		old_root->parent = new_root;
		if (right_leaf != NULL)
			right_leaf->parent = old_root;

		return (new_root);
	}

	node *leftRotate(node *old_root)
	{
    	node *new_root = old_root->right;
    	node *left_leaf = new_root->left;

    	new_root->left = old_root;
    	old_root->right = left_leaf;

    	old_root->height = max(getHeight(old_root->left), getHeight(old_root->right)) + 1;
    	new_root->height = max(getHeight(new_root->left), getHeight(new_root->right)) + 1;

		new_root->parent = old_root->parent;
		old_root->parent = new_root;
		if (left_leaf != NULL)
			left_leaf->parent = old_root;
 
    	return (new_root);
	}

	node *findFirst() const
	{
		node *current = root;

		while (current && current->left)
			current = current->left;
		return (current);
	}

	node *findLast() const
	{
		node *current = root;

		while (current && current->right && current->right->height != 0)
			current = current->right;
		return (current);
	}

	iterator begin()
	{
		if (root == NULL)
			return (iterator(_end));
		else
			return (iterator(findFirst()));
	}

	const_iterator begin() const
	{
		if (root == NULL)
			return (const_iterator(_end));
		else
			return (const_iterator(findFirst()));
	}

	reverse_iterator rbegin()
	{
		return (reverse_iterator(end()));
	}

	const_reverse_iterator rbegin() const
	{
		return (const_reverse_iterator(end()));
	}

	iterator end()
	{
		return (iterator(_end));
	}

	const_iterator end() const
	{
		return (const_iterator(_end));
	}

	reverse_iterator rend()
	{
		return (reverse_iterator(begin()));
	}

	const_reverse_iterator rend() const
	{
		return (const_reverse_iterator(begin()));
	}

	void swap(Tree &other)
	{
		node *tmp_end = _end;
		node *tmp_root = root;
		std::allocator<node> tmp_a = a;
		Compare tmp_comp = comp;
		size_type tmp_s = size;

		_end = other._end;
		root = other.root;
		a = other.a;
		comp = other.comp;
		size = other.size;

		other._end = tmp_end;
		other.root = tmp_root;
		other.a = tmp_a;
		other.comp = tmp_comp;
		other.size = tmp_s;
	}

	void nodeInfo(node* n)
	{
		if (n->parent)
			std::cout << "parent : key = " << n->parent->value.first << " value = " << n->parent->value.second << " height = " << n->parent->height << std::endl;
		if (n)
			std::cout << "current : key = " << n->value.first << " value = " << n->value.second << " height = " << n->height << std::endl;
		if (n->left)
			std::cout << "left : key = " << n->left->value.first << " value = " << n->left->value.second << " height = " << n->left->height << std::endl;
		if (n->right)
			std::cout << "right : key = " << n->right->value.first << " value = " << n->right->value.second << " height = " << n->right->height << std::endl;
		std::cout << "-----------------------\n";
	}

	void printTree() const
	{
		printTree2(root, NULL, true);
		std::cout << "\n-----------------------\n";
	}

	void showTrunks(Trunk *p) const 
	{
		if (p == NULL)
		{
			return;
		}

		showTrunks(p->prev);
		std::cout << p->str;
	}

	void printTree2(node *root, Trunk *prev, bool isLeft) const 
	{
		if (root == NULL)
		{
			return;
		}

		std::string prev_str = "    ";
		Trunk *trunk = new Trunk(prev, prev_str);

		printTree2(root->right, trunk, true);

		if (!prev)
		{
			trunk->str = "———";
		}
		else if (isLeft)
		{
			trunk->str = ".———";
			prev_str = "   |";
		}
		else
		{
			trunk->str = "`———";
			prev->str = prev_str;
		}

		showTrunks(trunk);
		std::cout << " (" << root->value.first << " / " << root->height << ")\n"; //<< root->height << endl;

		if (prev)
		{
			prev->str = prev_str;
		}
		trunk->str = "   |";

		printTree2(root->left, trunk, false);
	}

private:
	node *root;
	std::allocator<node> a;
	Compare comp;
	size_type size;
	node *_end;
};

}
#endif