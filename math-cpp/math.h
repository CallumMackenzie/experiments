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

#define VEC_NAME nvec
#define L_VEC_NAME lvec

template <typename T, size_t LEN>
struct VEC_NAME
{
	static const size_t length = LEN;
	T vec[LEN];

	VEC_NAME() {}

	VEC_NAME(std::initializer_list<T> list)
	{
		size_t i = -1;
		for (auto item : list)
			vec[++i] = item;
	}

	FULL_NUMERIC(VEC_NAME)

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

	// inline vec_hash hash(VEC_NAME v);
};

#define SWIZZ(RT1, RT2, FN_NAME, ...) \
	RT1, RT2 FN_NAME()                \
	{                                 \
		return RT1, RT2{__VA_ARGS__}; \
	}

#define REF(TYPE, INDEX, FN_NAME) \
	TYPE &FN_NAME = this->vec[INDEX];

#define REF_1(TYPE, L1) REF(TYPE, 0, L1)

#define REF_2(TYPE, L1, L2) \
	REF_1(TYPE, L1)         \
	REF(TYPE, 1, L2)

#define REF_3(TYPE, L1, L2, L3) \
	REF_2(TYPE, L1, L2)         \
	REF(TYPE, 2, L3)            \
	SWIZZ(L_VEC_NAME<T, 2>, L1##L2, L1, L2)

#define REF_4(TYPE, L1, L2, L3, L4) \
	REF_3(TYPE, L1, L2, L3)         \
	REF(TYPE, 3, L4)                \
	SWIZZ(L_VEC_NAME<T, 3>, L1##L2##L3, L1, L2, L3)

#define REF_5(TYPE, L1, L2, L3, L4, L5) \
	REF_4(TYPE, L1, L2, L3, L4)         \
	REF(TYPE, 4, L5)                    \
	SWIZZ(L_VEC_NAME<T, 4>, L1##L2##L3##L4, L1, L2, L3, L4)

#define REF_6(TYPE, L1, L2, L3, L4, L5, L6) \
	REF_5(TYPE, L1, L2, L3, L4, L5)         \
	REF(TYPE, 5, L6)                        \
	SWIZZ(L_VEC_NAME<T, 5>, L1##L2##L3##L4##L5, L1, L2, L3, L4, L5)

#define SELECTOR(x, L1, L2, L3, L4, L5, L6, FUNC, ...) FUNC

#define SELECT(SELECT, ARG, ...) SELECTOR(, ##__VA_ARGS__,              \
										  SELECT##_6(ARG, __VA_ARGS__), \
										  SELECT##_5(ARG, __VA_ARGS__), \
										  SELECT##_4(ARG, __VA_ARGS__), \
										  SELECT##_3(ARG, __VA_ARGS__), \
										  SELECT##_2(ARG, __VA_ARGS__), \
										  SELECT##_1(ARG, __VA_ARGS__), \
										  SELECT##_0(ARG, __VA_ARGS__))

template <typename T, size_t LEN>
struct L_VEC_NAME;

#define LETTER_VEC(NELEMS, letters...)                                           \
	template <typename T>                                                        \
	struct L_VEC_NAME<T, NELEMS> : public VEC_NAME<T, NELEMS>                    \
	{                                                                            \
		L_VEC_NAME() : VEC_NAME<T, NELEMS>() {}                                  \
		L_VEC_NAME(std::initializer_list<T> list) : VEC_NAME<T, NELEMS>(list) {} \
		SELECT(REF, T, ##letters)                                                \
		static const char *structure() { return #letters; }                      \
	};

// LETTER_VEC(1, x);
// LETTER_VEC(2, x, y);
// LETTER_VEC(3, x, y, z);
// LETTER_VEC(4, x, y, z, a);
// LETTER_VEC(5, x, y, z, a, b);
// LETTER_VEC(6, x, y, z, a, b, c);

template <typename T>
struct lvec<T, 1> : public nvec<T, 1>
{
	lvec() : nvec<T, 1>() {}
	lvec(std::initializer_list<T> list) : nvec<T, 1>(list) {}
	T &x = this->vec[0];
	static const char *structure() { return "x"; }
};

template <typename T>
struct lvec<T, 2> : public nvec<T, 2>
{
	lvec() : nvec<T, 2>() {}
	lvec(std::initializer_list<T> list) : nvec<T, 2>(list) {}
	T &x = this->vec[0];
	T &y = this->vec[1];
	static const char *structure() { return "x, y"; }
};

template <typename T>
struct lvec<T, 3> : public nvec<T, 3>
{
	lvec() : nvec<T, 3>() {}
	lvec(std::initializer_list<T> list) : nvec<T, 3>(list) {}
	T &x = this->vec[0];
	T &y = this->vec[1];
	T &z = this->vec[2];
	lvec<T, 2> xy() { return lvec<T, 2>{x, y}; }
	static const char *structure() { return "x, y, z"; }
};

template <typename T>
struct lvec<T, 4> : public nvec<T, 4>
{
	lvec() : nvec<T, 4>() {}
	lvec(std::initializer_list<T> list) : nvec<T, 4>(list) {}
	T &x = this->vec[0];
	T &y = this->vec[1];
	T &z = this->vec[2];
	T &a = this->vec[3];
	lvec<T, 2> xy() { return lvec<T, 2>{x, y}; }
	lvec<T, 3> xyz() { return lvec<T, 3>{x, y, z}; }
	static const char *structure() { return "x, y, z, a"; }
};

template <typename T>
struct lvec<T, 5> : public nvec<T, 5>
{
	lvec() : nvec<T, 5>() {}
	lvec(std::initializer_list<T> list) : nvec<T, 5>(list) {}
	T &x = this->vec[0];
	T &y = this->vec[1];
	T &z = this->vec[2];
	T &a = this->vec[3];
	T &b = this->vec[4];
	lvec<T, 2> xy() { return lvec<T, 2>{x, y}; }
	lvec<T, 3> xyz() { return lvec<T, 3>{x, y, z}; }
	lvec<T, 4> xyza() { return lvec<T, 4>{x, y, z, a}; }
	static const char *structure() { return "x, y, z, a, b"; }
};

template <typename T>
struct lvec<T, 6> : public nvec<T, 6>
{
	lvec() : nvec<T, 6>() {}
	lvec(std::initializer_list<T> list) : nvec<T, 6>(list) {}
	T &x = this->vec[0];
	T &y = this->vec[1];
	T &z = this->vec[2];
	T &a = this->vec[3];
	T &b = this->vec[4];
	T &c = this->vec[5];
	lvec<T, 2> xy() { return lvec<T, 2>{x, y}; }
	lvec<T, 3> xyz() { return lvec<T, 3>{x, y, z}; }
	lvec<T, 4> xyza() { return lvec<T, 4>{x, y, z, a}; }
	lvec<T, 5> xyzab() { return lvec<T, 5>{x, y, z, a, b}; }
	static const char *structure() { return "x, y, z, a, b, c"; }
};

typedef L_VEC_NAME<int, 2> vec2i;

#endif