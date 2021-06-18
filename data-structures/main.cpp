#include <iostream>
#include <vector>

#include "memory-aid.h"
#include "linked-list.h"
#include "doubly-linked-list.h"

COUNT_MEMORY

int main(int argc, char **argv)
{

	LkList<char> lkList = LkList<char>(std::vector<char>{'a', 'b', 'c', 'd'});
	std::cout << lkList.to_string() << std::endl;
	lkList.clear();

	DLkList<char> dlkList = DLkList<char>(std::vector<char>{'1', '2', '3', '4', '5'});
	std::cout << dlkList.to_string() << std::endl;
	dlkList.clear();

	PRINT_MEMORY_SUMMARY
	return 0;
}