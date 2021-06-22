
#include <iostream>
#include <string>
#include <stdexcept>

#include "memory-aid.h"
#include "math.h"

COUNT_MEMORY

#define ASSERT(...)                                                                                                                                             \
	if (!(__VA_ARGS__))                                                                                                                                         \
		throw std::runtime_error(std::string(__FILE__) + std::string(":") + std::to_string(__LINE__) + std::string(" in ") + std::string(__PRETTY_FUNCTION__)); \
	else                                                                                                                                                        \
		std::cout << "Test passed." << std::endl;

using namespace cmm;

int main(int, char **)
{

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

		auto v = lvec4<double>{1, 2, 3, 4};
		v /= 2.0;
		v += 1;
		v -= 0.1;
		v += lvec4<double>{10, 0, 2, 3};
		v *= lvec4<double>{3, -3, 2, 0.3};
		v = (2 * v);
		v = (v / 2);
		v += (v - 12);
		std::cout << v << std::endl;
#define P << ", " <<
		std::cout << v.x P v.y P v.z P v.w << std::endl
				  << v.xx() P v.xxx() P v.xxxx() << std::endl
				  << v.xy() P v.yx() << std::endl
				  << std::endl;

		std::cout << lvec4<int>().fill(2) << std::endl;

		auto mt = mat<int, 3, 2>{{1, 2, 3}, {4, 5, 6}};
		auto mt2 = mat<int, 2, 3>{{7, 8}, {9, 10}, {11, 12}};
		auto mt3 = mt * mt2;
		std::cout << mt3 << std::endl;

		auto mt4 = mat<int, 3, 2>{{1, -1, 2}, {0, -3, 1}};
		auto mprv = lvec3<int>{2, 1, 0};
		std::cout << mt4 * mprv << std::endl;

		std::cout << mat<int, 4, 4>::identity() << std::endl;
	}
	PRINT_MEMORY_SUMMARY
}