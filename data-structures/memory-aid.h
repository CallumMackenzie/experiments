
#define COUNT_ALLOCS                \
	size_t allocs = 0;              \
	void *operator new(size_t size) \
	{                               \
		++allocs;                   \
		return malloc(size);        \
	}

#define COUNT_DELETES               \
	size_t deletes = 0;             \
	void operator delete(void *ptr) \
	{                               \
		++deletes;                  \
		free(ptr);                  \
	}

#define COUNT_MEMORY \
	COUNT_ALLOCS     \
	COUNT_DELETES

#define PRINT_ALLOCS \
	std::cout        \
		<< "Allocations: " << allocs << std::endl;

#define PRINT_DELETES \
	std::cout         \
		<< "Deletes: " << deletes << std::endl;

#define PRINT_MEMORY_SUMMARY                 \
	std::cout                                \
		<< "Allocs: " << allocs << std::endl \
		<< "Deletes: " << deletes << std::endl;