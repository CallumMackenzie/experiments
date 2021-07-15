#include <iostream>

#include "galg.h"
#include "galg_ops.h"

using namespace galg;
int main(int, char **)
{
    vec4 a{1, 2, 3, 4};
    vec4 b = a + 2;
    vec4 c = 2 * a;
    vec4 d{0, 1, 1, 0};
    mat4 e = mat4::identity();
    std::cout << to_string(e * (a * d)) << std::endl;
}
