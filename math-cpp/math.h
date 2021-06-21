#ifndef MATH_H_
#define MATH_H_ 1

#include <initializer_list>
#include <stddef.h>
#include <iostream>

typedef long double vec_hash;

#define BIT_OP(OP, TYPE)                                      \
	friend TYPE operator OP(const TYPE &lhs, const TYPE &rhs) \
	{                                                         \
		TYPE ret;                                             \
		for (size_t i = 0; i < TYPE::length; i++)             \
			ret.vec[i] = lhs.vec[i] OP rhs.vec[i];            \
		return ret;                                           \
	}

#define UNARY_OP(OP, TYPE)                        \
	TYPE operator OP()                            \
	{                                             \
		TYPE ret;                                 \
		for (size_t i = 0; i < TYPE::length; i++) \
			ret.vec[i] = OP vec[i];               \
		return ret;                               \
	}

#define CMP_OP(SIDE_OP, ADD_OP, INIT, TYPE)                          \
	friend bool operator SIDE_OP(const TYPE &lhs, const TYPE &rhs)   \
	{                                                                \
		bool cmp_res = INIT;                                         \
		for (size_t i = 0; i < TYPE::length; i++)                    \
			cmp_res = cmp_res ADD_OP(lhs.vec[i] SIDE_OP rhs.vec[i]); \
		return cmp_res;                                              \
	}

#define OSTREAM_OP(TYPE)                                               \
	friend std::ostream &operator<<(std::ostream &os, const TYPE &val) \
	{                                                                  \
		return os << val.to_string();                                  \
	}

#define ISTREAM_OP(TYPE)                                  \
	std::istream &operator>>(std::istream &is, TYPE &val) \
	{                                                     \
		return val.from_string(is.str());                 \
	}

#define ALGEBRAIC(TYPE) \
	BIT_OP(+, TYPE)     \
	BIT_OP(-, TYPE)     \
	BIT_OP(*, TYPE)     \
	BIT_OP(/, TYPE)     \
	BIT_OP(&, TYPE)     \
	BIT_OP(|, TYPE)     \
	BIT_OP(^, TYPE)     \
	UNARY_OP(-, TYPE)   \
	UNARY_OP(!, TYPE)

#define PARTIAL_EQ(TYPE)      \
	CMP_OP(==, &, true, TYPE) \
	CMP_OP(!=, |, false, TYPE)

#define FULL_NUMERIC(TYPE) \
	ALGEBRAIC(TYPE);       \
	PARTIAL_EQ(TYPE);      \
	OSTREAM_OP(TYPE);

template <typename T, size_t LEN>
class nvec
{
public:
	static const size_t length = LEN;
	T vec[LEN];

	nvec() {}

	nvec(std::initializer_list<T> list)
	{
		size_t i = -1;
		for (auto item : list)
			vec[++i] = item;
	}

	FULL_NUMERIC(nvec)

	T operator[](size_t index)
	{
		return vec[index];
	}

	std::string to_string() const
	{
		std::string ret = "{";
		for (size_t i = 0; i < length; ++i)
		{
			ret.append(std::to_string(vec[i]));
			if (i == length - 1)
				ret.append("}");
			else
				ret.append(", ");
		}
		return ret;
	}

	// inline vec_hash hash(nvec v);
};

#define LVEC_DECL(NUM_ELEMS)        \
	template <typename T, size_t N> \
	class lvec##NUM_ELEMS;          \
	template <typename T>           \
	using lv##NUM_ELEMS = lvec##NUM_ELEMS<T, NUM_ELEMS>;

LVEC_DECL(1)
LVEC_DECL(2)
LVEC_DECL(3)
LVEC_DECL(4)

template <typename T, size_t NELEMS = 1>
class lvec1 : public nvec<T, NELEMS>
{
private:
	using parent = nvec<T, NELEMS>;

public:
	lvec1() : parent() {}
	lvec1(std::initializer_list<T> list) : parent(list) {}
	T &x = this->vec[0];
	lv2<T> xx() { return lv2<T>{x, x}; }
	lv3<T> xxx() { return lv3<T>{x, x, x}; }
	lv4<T> xxxx() { return lv4<T>{x, x, x, x}; }
};

template <typename T, size_t NELEMS = 2>
class lvec2 : public lvec1<T, NELEMS>
{
private:
	using parent = lvec1<T, NELEMS>;

public:
	lvec2() : parent() {}
	lvec2(std::initializer_list<T> list) : parent(list) {}
	T &y = this->vec[1];

	lv4<T> yyyy() { return lv4<T>{y, y, y, y}; }
	lv3<T> yyy() { return lv3<T>{y, y, y}; }
	lv2<T> yy() { return lv2<T>{y, y}; }
	lv2<T> yx() { return lv2<T>{y, this->x}; }
	lv2<T> xy() { return lv2<T>{this->x, y}; }
	lv3<T> xxy() { return lv3<T>{this->x, this->x, y}; }
	lv3<T> xyy() { return lv3<T>{this->x, y, y}; }
	lv3<T> yxx() { return lv3<T>{y, this->x, this->x}; }
	lv3<T> yyx() { return lv3<T>{y, y, this->x}; }
	lv3<T> xyx() { return lv3<T>{this->x, y, this->x}; }
};

template <typename T, size_t NELEMS = 3>
class lvec3 : public lvec2<T, NELEMS>
{
private:
	using parent = lvec2<T, NELEMS>;

public:
	lvec3() : parent() {}
	lvec3(std::initializer_list<T> list) : parent(list) {}
	T &z = this->vec[2];

	lv2<T> zz() { return lv2<T>{z, z}; }
	lv3<T> zzz() { return lv3<T>{z, z, z}; }
	lv4<T> zzzz() { return lv4<T>{z, z, z, z}; }
	lv3<T> xyz() { return lv3<T>{this->x, this->y, z}; }
	lv3<T> zyx() { return lv3<T>{z, this->y, this->x}; }
};

template <typename T, size_t NELEMS = 4>
class lvec4 : public lvec3<T, NELEMS>
{
private:
	using parent = lvec3<T, NELEMS>;

public:
	lvec4() : parent() {}
	lvec4(std::initializer_list<T> list) : parent(list) {}
	T &w = this->vec[3];
};

#define NEW_LVEC(NUM_ELEMS, PREV_NELEMS, LETTER)                         \
	template <typename T, size_t NELEMS = NUM_ELEMS>                     \
	class lvec##NUM_ELEMS : public lvec##PREV_NELEMS<T, NELEMS>          \
	{                                                                    \
	private:                                                             \
		using parent = lvec##PREV_NELEMS<T, NELEMS>;                     \
                                                                         \
	public:                                                              \
		lvec##NUM_ELEMS() : parent() {}                                  \
		lvec##NUM_ELEMS(std::initializer_list<T> list) : parent(list) {} \
		T &LETTER = this->vec[PREV_NELEMS];                              \
	};

NEW_LVEC(5, 4, a)

#endif