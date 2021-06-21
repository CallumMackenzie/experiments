
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

	ASSERT(lvec2<int>{-1, -2} == -lvec2<int>{1, 2});
	ASSERT(lvec2<int>{2, 3} * lvec2<int>{-3, 4} == lvec2<int>{-6, 12});
	ASSERT(lvec2<int>{12, 3} + lvec2<int>{2, -1} == lvec2<int>{14, 2});
	ASSERT(lvec2<int>{9, 2} - lvec2<int>{8, 1} == lvec2<int>{1, 1});
	ASSERT(lvec2<int>{100, 80} / lvec2<int>{10, 8} == lvec2<int>{10, 10});
	ASSERT((lvec2<int>{0b111, 0b001} & lvec2<int>{0b101, 0b100}) == lvec2<int>{0b101, 0b000});
	ASSERT((lvec2<int>{0b110, 0b010} | lvec2<int>{0b101, 0b001}) == lvec2<int>{0b111, 0b011});
	ASSERT((!lvec2<int>{0b011, 0b111}) == lvec2<int>{0b000, 0b000});
	ASSERT((lvec2<int>{0b111, 0b101} ^ lvec2<int>{0b010, 0b111}) == lvec2<int>{0b101, 0b010});

	auto v = lvec5<int>{1, 2, 3, 4, 5};
	std::cout << v << std::endl;
#define P << ", " <<
	std::cout << v.x P v.y P v.z P v.w P v.a << std::endl
			  << v.xx() P v.xxx() P v.xxxx() << std::endl
			  << v.xy() P v.yx() << std::endl;
}