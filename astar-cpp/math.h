#pragma once

typedef unsigned long long VecHash;

template <typename T>
struct Vec2
{
	union
	{
		T width;
		T x;
		T horizontal;
	};
	union
	{
		T height;
		T y;
		T vertical;
	};
	std::string toString()
	{
		return std::string("(").append(std::to_string(x)).append(", ").append(std::to_string(y)).append(")");
	}
	inline friend bool operator==(const Vec2 &lhs, const Vec2 &rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}
	inline friend Vec2 operator+(const Vec2 &lhs, const Vec2 &rhs)
	{
		return Vec2{lhs.x + rhs.x, lhs.y + rhs.y};
	}
	inline friend Vec2 operator-(const Vec2 &lhs, const Vec2 &rhs)
	{
		return Vec2{lhs.x - rhs.x, lhs.y - rhs.y};
	}
	inline friend Vec2 operator*(const Vec2 &lhs, const Vec2 &rhs)
	{
		return Vec2{lhs.x * rhs.x, lhs.y * rhs.y};
	}
	inline friend Vec2 operator/(const Vec2 &lhs, const Vec2 &rhs)
	{
		return Vec2{lhs.x / rhs.x, lhs.y / rhs.y};
	}

	static inline VecHash hash(Vec2 v)
	{
		return ((0ULL | v.x) << 16) | v.y;
	}
};