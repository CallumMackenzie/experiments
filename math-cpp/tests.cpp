
#include <iostream>
#include <string>
#include <stdexcept>

#include "math.h"

#define ASSERT(...)                                                                                                                                             \
	if (!(__VA_ARGS__))                                                                                                                                         \
		throw std::runtime_error(std::string(__FILE__) + std::string(":") + std::to_string(__LINE__) + std::string(" in ") + std::string(__PRETTY_FUNCTION__)); \
	else                                                                                                                                                        \
		std::cout << "Test passed." << std::endl;

int main(int, char **)
{

	ASSERT(vec2i{-1, -2} == -vec2i{1, 2});
	ASSERT(vec2i{2, 3} * vec2i{-3, 4} == vec2i{-6, 12});
	ASSERT(vec2i{12, 3} + vec2i{2, -1} == vec2i{14, 2});
	ASSERT(vec2i{9, 2} - vec2i{8, 1} == vec2i{1, 1});
	ASSERT(vec2i{100, 80} / vec2i{10, 8} == vec2i{10, 10});
	ASSERT((vec2i{0b111, 0b001} & vec2i{0b101, 0b100}) == vec2i{0b101, 0b000});
	ASSERT((vec2i{0b110, 0b010} | vec2i{0b101, 0b001}) == vec2i{0b111, 0b011});
	ASSERT((!vec2i{0b011, 0b111}) == vec2i{0b000, 0b000});
	ASSERT((vec2i{0b111, 0b101} ^ vec2i{0b010, 0b111}) == vec2i{0b101, 0b010});

	auto v = lvec<int, 4>{1, 2, 3 ,4};
	std::cout << v << std::endl;
	std::cout << v.xyz() << std::endl;

}