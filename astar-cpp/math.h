#pragma once

struct Vec2
{
	union
	{
		int width;
		int x;
	};
	union
	{
		int height;
		int y;
	};
	std::string toString()
	{
		return std::string("(").append(std::to_string(x)).append(", ").append(std::to_string(y)).append(")");
	}
	friend bool operator==(const Vec2 &lhs, const Vec2 &rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}
	friend Vec2 operator+(const Vec2 &lhs, const Vec2 &rhs)
	{
		return Vec2{lhs.x + rhs.x, lhs.y + rhs.y};
	}
	friend Vec2 operator-(const Vec2 &lhs, const Vec2 &rhs)
	{
		return Vec2{lhs.x - rhs.x, lhs.y - rhs.y};
	}
	friend Vec2 operator*(const Vec2 &lhs, const Vec2 &rhs)
	{
		return Vec2{lhs.x * rhs.x, lhs.y * rhs.y};
	}
	friend Vec2 operator/(const Vec2 &lhs, const Vec2 &rhs)
	{
		return Vec2{lhs.x / rhs.x, lhs.y / rhs.y};
	}
};