#ifndef REL_PATHS_H_
#define REL_PATHS_H_ 1

#include <string>
#ifdef _WIN32
std::string R_PATH(const char *str)
{
	return std::string("../").append(std::string(str));
}
#elif
std::string R_PATH(const char *str)
{
	return std::string(str);
}
#endif

#endif