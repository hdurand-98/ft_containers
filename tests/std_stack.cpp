#include <iostream>
#include <vector>
#include <deque>
#include <stack>

void std_test_stack_constructor()
{
    std::cout << "---------STD STACK CONSTRUCTOR--------\n";
    std::deque<int> mydeque(3, 100);   // deque with 3 elements
    std::vector<int> myvector(2, 200); // vector with 2 elements
    std::stack<int> first;              // empty stack
    std::stack<int, std::deque<int> > second(mydeque);    // stack initialized to copy of deque

    std::stack<int, std::vector<int> > third; // empty stack using vector
    std::stack<int, std::vector<int> > fourth(myvector);

    std::cout << "size of first: " << first.size() << '\n';
    std::cout << "size of second: " << second.size() << '\n';
    std::cout << "size of third: " << third.size() << '\n';
    std::cout << "size of fourth: " << fourth.size() << '\n';
}

void std_test_stack_empty()
{
    std::cout << "---------STD STACK EMPTY--------\n";
    std::stack<int> mystack;
    int sum(0);

    for (int i = 1; i <= 10; i++)
        mystack.push(i);

    while (!mystack.empty())
    {
        sum += mystack.top();
        mystack.pop();
    }

    std::cout << "total: " << sum << '\n';
}

void std_test_stack_size()
{
    std::cout << "---------STD STACK SIZE--------\n";
    std::stack<int> myints;
    std::cout << "0. size: " << myints.size() << '\n';

    for (int i = 0; i < 5; i++)
        myints.push(i);
    std::cout << "1. size: " << myints.size() << '\n';

    myints.pop();
    std::cout << "2. size: " << myints.size() << '\n';
}

void std_test_stack_top()
{
    std::cout << "---------STD STACK TOP--------\n";
    std::stack<int> mystack;

    mystack.push(10);
    mystack.push(20);

    mystack.top() -= 5;

    std::cout << "mystack.top() is now " << mystack.top() << '\n';
}

void std_test_stack_push_pop()
{
    std::cout << "---------STD STACK PUSH TOP--------\n";
    std::stack<int> mystack;

    for (int i = 0; i < 5; ++i)
        mystack.push(i);

    std::cout << "Popping out elements...";
    while (!mystack.empty())
    {
        std::cout << ' ' << mystack.top();
        mystack.pop();
    }
    std::cout << '\n';
}