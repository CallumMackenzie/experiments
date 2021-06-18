#include <iostream>
#include <vector>

#include "memory-aid.h"
#include "linked-list.h"
#include "doubly-linked-list.h"
#include "array.h"
#include "binary-tree.h"

COUNT_MEMORY

using namespace dst;

int main(int argc, char **argv)
{

	auto header = [] (std::string txt)
	{
		std::cout << std::endl << txt << std::endl;
	};

	{
		header("DST::LK_LIST");
		auto lkList = lk_list<char>(std::vector<char>{'a', 'b', 'c', 'd'});
		for (auto i : lkList)
			std::cout << i->data << std::endl;
		std::cout << "2: " << lkList[2]->data << std::endl;
		lkList.clear();

		header("DST::DLK_LIST");
		auto dlkList = dlk_list<char>(std::vector<char>{'w', 'x', 'y', 'z'});
		for (auto i : dlkList)
			std::cout << i->data << std::endl;
		std::cout << "2: " << dlkList[2]->data << std::endl;
		dlkList.clear();

		header("DST::ARRAY");
		auto arr = array<int>(std::vector<int>{1, 2, 3, 4});
		for (auto i : arr)
			std::cout << i << std::endl;
		std::cout << "2: " << arr[2] << std::endl;

		header("DST::BINARY_TREE");
		auto btree = binary_tree<char>();
	}

	PRINT_MEMORY_SUMMARY
	return 0;
}