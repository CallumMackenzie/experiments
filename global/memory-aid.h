
// Counts allocations into a variable called 'allocs'.
#ifndef MEMORY_AID_H_
#define MEMORY_AID_H_ 1
#define COUNT_ALLOCS                \
	size_t allocs = 0;              \
	void *operator new(size_t size) \
	{                               \
		++allocs;                   \
		return malloc(size);        \
	}

// Counts frees into a variable called 'deletes'.
#define COUNT_DELETES               \
	size_t deletes = 0;             \
	void operator delete(void *ptr) \
	{                               \
		++deletes;                  \
		free(ptr);                  \
	}

// Counts allocs and frees into variables called 'allocs' and 'deletes' respectively.
#define COUNT_MEMORY \
	COUNT_ALLOCS     \
	COUNT_DELETES

// Prints the number of allocations.
#define PRINT_ALLOCS \
	std::cout        \
		<< "Allocations: " << allocs << std::endl;

// Prints the number of frees.
#define PRINT_DELETES \
	std::cout         \
		<< "Deletes: " << deletes << std::endl;

// Prints the number of allocations and frees.
#define PRINT_MEMORY_SUMMARY                 \
	std::cout                                \
		<< "Allocs: " << allocs << std::endl \
		<< "Deletes: " << deletes << std::endl;
#endif