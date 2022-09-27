#include "includes/ft.hpp"
#include "includes/map.hpp"
#include "includes/vector.hpp"
#include "includes/stack.hpp"

#include "tests/ft_map.cpp"
#include "tests/std_map.cpp"

#include "tests/ft_vector.cpp"
#include "tests/std_vector.cpp"

#include "tests/ft_stack.cpp"
#include "tests/std_stack.cpp"

#include <sys/time.h>
#include <stdlib.h>

long    ft_get_time(void)
{
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_usec);
}

int main()
{
    typedef void(*FunctionPointer)();

    FunctionPointer ft_vector_tests[] = { ft_test_vector_swap, ft_test_vector_relational_operator, ft_test_vector_allocator,
    ft_test_vector_modifiers_delete, ft_test_vector_modifiers_insert, ft_test_vector_modifiers_add,
    ft_test_vector_operator_front_back, ft_test_vector_operator_at, ft_test_vector_operator_access, ft_test_vector_capacity, ft_test_vector_size,
    ft_test_vector_reverse_iterators, ft_test_vector_iterators, ft_test_vector_operator,
    ft_test_vector_construct };

    FunctionPointer ft_map_tests[] = {ft_test_map_relational_operator, ft_test_map_allocator,
    ft_test_map_equal, ft_test_map_lower_upper, ft_test_map_operation_count,
    ft_test_map_operation_find, ft_test_map_observers_value_comp, ft_test_map_observers_key_comp,
    ft_test_map_modifiers, ft_test_map_modifiers_insert_erase, ft_test_map_operator_access,
    ft_test_max_size, ft_test_map_capacity, ft_test_map_iterator, ft_test_map_operator, ft_test_map_construct};

    FunctionPointer ft_stack_tests[] = { ft_test_stack_push_pop, ft_test_stack_top, ft_test_stack_size,
    ft_test_stack_empty, ft_test_stack_constructor };

    FunctionPointer std_vector_tests[] = { std_test_vector_swap, std_test_vector_relational_operator, std_test_vector_allocator,
    std_test_vector_modifiers_delete, std_test_vector_modifiers_insert, std_test_vector_modifiers_add,
    std_test_vector_operator_front_back, std_test_vector_operator_at, std_test_vector_operator_access, std_test_vector_capacity, std_test_vector_size,
    std_test_vector_reverse_iterators, std_test_vector_iterators, std_test_vector_operator,
    std_test_vector_construct };

    FunctionPointer std_map_tests[] = {std_test_map_relational_operator, std_test_map_allocator,
    std_test_map_equal, std_test_map_lower_upper, std_test_map_operation_count,
    std_test_map_operation_find, std_test_map_observers_value_comp, std_test_map_observers_key_comp,
    std_test_map_modifiers, std_test_map_modifiers_insert_erase, std_test_map_operator_access,
    std_test_max_size, std_test_map_capacity, std_test_map_iterator, std_test_map_operator, std_test_map_construct};

    FunctionPointer std_stack_tests[] = { std_test_stack_push_pop, std_test_stack_top, std_test_stack_size,
    std_test_stack_empty, std_test_stack_constructor };

    std::cout << "---------------STACK TESTS----------------\n";
    int k = 4;
    while (k >= 0)
    {
        long before = ft_get_time();
        ft_stack_tests[k]();
        long after = ft_get_time();

        std::cout << std::endl;
        long ft_duration = after - before;
        std::cout << "ft version took : " << ft_duration << " microseconds to complete\n";

        before = ft_get_time();
        std_stack_tests[k]();
        after = ft_get_time();

        std::cout << std::endl;
        long std_duration = after - before;
        std::cout << "std version took : " << std_duration << " microseconds to complete\n";

        if (ft_duration <= std_duration * 20)
        {
            std::cout << "\033[1;32m";
            std::cout << "FT VERSION IS FAST ENOUGH\n";
        }
        else
        {
            std::cout << "\033[1;31m";
            std::cout << "FT VERSION IS TOO SLOW\n";
        }
        std::cout << "\033[39;49m";

        k--;
    }

    std::cout << "---------------VECTOR TESTS----------------\n";
    int i = 14;
    while (i >= 0)
    {
        long before = ft_get_time();
        ft_vector_tests[i]();
        long after = ft_get_time();

        std::cout << std::endl;
        long ft_duration = after - before;
        std::cout << "ft version took : " << ft_duration << " microseconds to complete\n";

        before = ft_get_time();
        std_vector_tests[i]();
        after = ft_get_time();

        std::cout << std::endl;
        long std_duration = after - before;
        std::cout << "std version took : " << std_duration << " microseconds to complete\n";

        if (ft_duration <= std_duration * 20)
        {
            std::cout << "\033[1;32m";
            std::cout << "FT VERSION IS FAST ENOUGH\n";
        }
        else
        {
            std::cout << "\033[1;31m";
            std::cout << "FT VERSION IS TOO SLOW\n";
        }
        std::cout << "\033[39;49m";
        i--;
    }

    std::cout << "---------------MAP TESTS----------------\n";
    int y = 15;
    while (y >= 0)
    {
        long before = ft_get_time();
        ft_map_tests[y]();
        long after = ft_get_time();

        std::cout << std::endl;
        long ft_duration = after - before;
        std::cout << "ft version took : " << ft_duration << " microseconds to complete\n";

        before = ft_get_time();
        std_map_tests[y]();
        after = ft_get_time();

        std::cout << std::endl;
        long std_duration = after - before;
        std::cout << "std version took : " << std_duration << " microseconds to complete\n";


        if (ft_duration <= std_duration * 20)
        {
            std::cout << "\033[1;32m";
            std::cout << "FT VERSION IS FAST ENOUGH\n";
        }
        else
        {
            std::cout << "\033[1;31m";
            std::cout << "FT VERSION IS TOO SLOW\n";
        }
        std::cout << "\033[39;49m";

        y--;
    }
}