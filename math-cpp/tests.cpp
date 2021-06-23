
#include <iostream>
#include <string>
#include <stdexcept>
#include <time.h>

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

		std::cout << mat4<double>::translation(2, 3, 2) << std::endl;
		std::cout << mat4<double>::scale(2, 3, 2) << std::endl;
		std::cout << mat4<double>::rotation(2, 3, 2) << std::endl;

		auto v2 = lvec4<double>{1, 2, 3, 1};
		auto v3 = lvec3<double>{2, 2, 2};
		std::cout << "Transforming " << v2 << ": " << (mat4<double>::translation(v3) * v2) << std::endl;

		auto start = clock();
		for (size_t i = 0; i < 33333; i++)
			auto matt = mat4<double>::translation(i, 0.5 * i, 12 - i);
		for (size_t i = 0; i < 33333; i++)
			auto matt = mat4<double>::scale(2, i - 4, 3 * i);
		for (size_t i = 0; i < 33334; i++)
			auto matt = mat4<double>{{12, 2, -43, 0}, {122, 31, 32, 1}, {0, 0, 2, 1}, {12, 3, 4, 2}};
		auto end = ((double)(clock() - start) / (double)CLOCKS_PER_SEC) * 1000.0;
		std::cout << "Done 100,000 mat4 creations in " << end << "ms" << std::endl;

		start = clock();
		for (size_t i = 0; i < 100000; i++)
			auto v10 = lvec4<double>{2.0 + i, 12.44, i - 32.5, 1};
		end = ((double)(clock() - start) / (double)CLOCKS_PER_SEC) * 1000.0;
		std::cout << "Done 100,000 vec4 creations in " << end << "ms" << std::endl;

		auto mtrix = mat4<double>::translation(3, 4, 5) * mat4<double>::scale(2, 0.4, 0.7);
		auto vectr = lvec4<double>{123, 12, 34, 12};
		start = clock();
		for (size_t i = 0; i < 100000; i++)
			auto prod = mtrix * vectr;
		end = ((double)(clock() - start) / (double)CLOCKS_PER_SEC) * 1000.0;
		std::cout << "Done 100,000 mat4/vec4 multiplications in " << end << "ms" << std::endl;

		auto mtrix2 = mat4<double>::rotation(3, 4, 5) * mat4<double>::scale(0.33, 3, 2.7);
		start = clock();
		for (size_t i = 0; i < 100000; i++)
			auto prod = mtrix * mtrix2;
		end = ((double)(clock() - start) / (double)CLOCKS_PER_SEC) * 1000.0;
		std::cout << "Done 100,000 mat4 multiplications in " << end << "ms" << std::endl;

		auto mat2x3 = mat<double, 2, 3>{{3, 4}, {0.45, 2.343}, {-123, 434}};
		auto mat3x2 = mat<double, 3, 2>{{1, 2, 3}, {1234, 76.33, 9.44}, {9.123, 343.2, 331.434}};
		start = clock();
		for (size_t i = 0; i < 100000; i++)
			auto prod = mat2x3 * mat3x2;
		end = ((double)(clock() - start) / (double)CLOCKS_PER_SEC) * 1000.0;
		std::cout << "Done 100,000 mat3x2/mat2x3 multiplications in " << end << "ms" << std::endl;
	}
	PRINT_MEMORY_SUMMARY
}