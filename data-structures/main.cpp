#include <iostream>
#include <vector>

#define DLK_LIST_TO_STR
#define LK_LIST_TO_STR
#define DST_ARRAY_TO_STR

#include "memory-aid.h"
#include "linked-list.h"
#include "doubly-linked-list.h"
#include "array.h"

COUNT_MEMORY

using namespace dst;

int main(int argc, char **argv)
{

	{
		auto lkList = lk_list<char>(std::vector<char>{'a', 'b', 'c', 'd'});
		for (auto i : lkList)
			std::cout << i->data << std::endl;
		std::cout << "2: " << lkList[2]->data << std::endl;
		std::cout << lkList.to_string() << std::endl;
		lkList.clear();

		auto dlkList = dlk_list<char>(std::vector<char>{'w', 'x', 'y', 'z'});
		for (auto i : dlkList)
			std::cout << i->data << std::endl;
		std::cout << "2: " << dlkList[2]->data << std::endl;
		std::cout << dlkList.to_string() << std::endl;
		dlkList.clear();

		auto arr = array<int>(std::vector<int>{1, 2, 3, 4});
		for (auto i : arr)
			std::cout << i << std::endl;
		std::cout << "2: " << arr[2] << std::endl;
		std::cout << arr.to_string() << std::endl;
	}

	PRINT_MEMORY_SUMMARY
	return 0;
}