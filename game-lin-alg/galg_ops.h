#ifndef GALG_OPS_H
#define GALG_OPS_H 1

#include <string>
#include "galg.h"

namespace galg
{
	std::string to_string(const vec4 &vec)
	{
		std::string ret("(");
		for (unsigned int i = 0; i < 4; ++i)
			ret.append(std::to_string(vec[i])).append(i == 3 ? ")" : ",");
		return ret;
	}
	std::string to_string(const vec2 &vec)
	{
		return std::string("(").append(std::to_string(vec.x())).append(", ").append(std::to_string(vec.y())).append(")");
	}
	std::string to_string(bool b)
	{
		return std::string(b ? "true" : "false");
	}
}

#endif