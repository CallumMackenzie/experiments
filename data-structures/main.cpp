#include <iostream>
#include <vector>

#define DLK_LIST_TO_STR
#define LK_LIST_TO_STR

#include "memory-aid.h"
#include "linked-list.h"
#include "doubly-linked-list.h"
#include "array.h"

COUNT_MEMORY

using namespace dst;

struct tester
{
	int v1 = 212;
	char v2 = '1';
	bool v3 = true;
	std::string v4 = "omg";
};

int main(int argc, char **argv)
{

	{
		auto lkList = lk_list<char>(std::vector<char>{'a', 'b', 'c', 'd'});
		std::cout << lkList.to_string() << std::endl;
		lkList.clear();

		auto dlkList = dlk_list<char>(std::vector<char>{'1', '2', '3', '4', '5'});
		std::cout << dlkList.to_string() << std::endl;
		dlkList.clear();

		auto arr = array<tester>(std::vector<tester>{tester{}, tester{10, 'f', false, "brah"}});
		for (auto e : arr)
			std::cout << "{" << e.v1 << ", " << e.v2 << ", " << e.v3 << ", " << e.v4
					  << "}" << std::endl;
		// std::cout << arr.to_string() << std::endl;
	}

	PRINT_MEMORY_SUMMARY
	return 0;
}