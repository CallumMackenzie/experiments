#include <iostream>
#include "memory-aid.h"
#include "galg.h"
#include "galg_ops.h"

COUNT_MEMORY

int main(int, char **)
{
    {
        galg::str_fmt fmt = galg::str_fmt::array;
        galg::vec4 v4{1, 2, 3, 4};
        galg::vec3 v3{3, 2, 1};
        galg::vec2 v2{10, 20};
        galg::mat2 m2{
            {1, 2},
            {3, 4}};
        galg::mat3 m3{
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}};
        galg::mat4 m4{
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 16}};
    }
    PRINT_MEMORY_SUMMARY
}
