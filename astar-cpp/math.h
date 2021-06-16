#pragma once

template <typename T>
struct Vec2;

typedef unsigned long long vec_hash;
typedef Vec2<int> vec2i;
typedef Vec2<double> vec2d;
typedef Vec2<float> vec2f;
typedef Vec2<unsigned int> vec2ui;
typedef Vec2<long> vec2l;
typedef Vec2<short> vec2s;

template <typename T>
struct Vec2
{
	union
	{
		T width;
		T x;
		T horizontal;
		T w;
		T u;
		T s;
	};
	union
	{
		T height;
		T y;
		T vertical;
		T h;
		T v;
		T t;
	};
	std::string toString()
	{
		return std::string("(").append(std::to_string(x)).append(", ").append(std::to_string(y)).append(")");
	}
	Vec2 pow2()
	{
		return (*this) * (*this);
	}
	T sum()
	{
		return x + y;
	}
	inline friend bool operator==(const Vec2 &lhs, const Vec2 &rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}
	inline friend bool operator!=(const Vec2 &lhs, const Vec2 &rhs)
	{
		return lhs.x != rhs.x || lhs.y != rhs.y;
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

	static inline vec_hash hash(Vec2 v)
	{
		return ((0ULL | v.x) << 16) | v.y;
	}
};