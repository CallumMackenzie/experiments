#ifndef GALG_H
#define GALG_H 1

#include <initializer_list>
#include <string.h>

#define FORCE_INLINE inline

#define ALGEBRAIC_VEC(CLASS, NELEMS)                            \
	friend CLASS operator*(const CLASS &lhs, const CLASS &rhs)  \
	{                                                           \
		CLASS ret;                                              \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			ret[i] = lhs[i] * rhs[i];                           \
		return ret;                                             \
	}                                                           \
	friend CLASS operator/(const CLASS &lhs, const CLASS &rhs)  \
	{                                                           \
		CLASS ret;                                              \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			ret[i] = lhs[i] / rhs[i];                           \
		return ret;                                             \
	}                                                           \
	friend CLASS operator+(const CLASS &lhs, const CLASS &rhs)  \
	{                                                           \
		CLASS ret;                                              \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			ret[i] = lhs[i] + rhs[i];                           \
		return ret;                                             \
	}                                                           \
	friend CLASS operator-(const CLASS &lhs, const CLASS &rhs)  \
	{                                                           \
		CLASS ret;                                              \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			ret[i] = lhs[i] - rhs[i];                           \
		return ret;                                             \
	}                                                           \
	friend CLASS operator*(const CLASS &lhs, const fp_num &rhs) \
	{                                                           \
		CLASS ret;                                              \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			ret[i] = lhs[i] * rhs;                              \
		return ret;                                             \
	}                                                           \
	friend CLASS operator/(const CLASS &lhs, const fp_num &rhs) \
	{                                                           \
		CLASS ret;                                              \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			ret[i] = lhs[i] / rhs;                              \
		return ret;                                             \
	}                                                           \
	friend CLASS operator+(const CLASS &lhs, const fp_num &rhs) \
	{                                                           \
		CLASS ret;                                              \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			ret[i] = lhs[i] + rhs;                              \
		return ret;                                             \
	}                                                           \
	friend CLASS operator-(const CLASS &lhs, const fp_num &rhs) \
	{                                                           \
		CLASS ret;                                              \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			ret[i] = lhs[i] - rhs;                              \
		return ret;                                             \
	}                                                           \
	friend CLASS operator*(const fp_num &lhs, const CLASS &rhs) \
	{                                                           \
		CLASS ret;                                              \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			ret[i] = lhs * rhs[i];                              \
		return ret;                                             \
	}                                                           \
	friend CLASS operator/(const fp_num &lhs, const CLASS &rhs) \
	{                                                           \
		CLASS ret;                                              \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			ret[i] = lhs / rhs[i];                              \
		return ret;                                             \
	}                                                           \
	friend CLASS operator+(const fp_num &lhs, const CLASS &rhs) \
	{                                                           \
		CLASS ret;                                              \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			ret[i] = lhs + rhs[i];                              \
		return ret;                                             \
	}                                                           \
	friend CLASS operator-(const fp_num &lhs, const CLASS &rhs) \
	{                                                           \
		CLASS ret;                                              \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			ret[i] = lhs - rhs[i];                              \
		return ret;                                             \
	}                                                           \
	friend bool operator==(const CLASS &lhs, const CLASS &rhs)  \
	{                                                           \
		bool res = true;                                        \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			res &= (lhs.v[i] == rhs.v[i]);                      \
		return res;                                             \
	}                                                           \
	friend bool operator!=(const CLASS &lhs, const CLASS &rhs)  \
	{                                                           \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			if (lhs[i] != rhs[i])                               \
				return true;                                    \
		return false;                                           \
	}

#define MAT_FP_NUM_OP(CLASS, SZ, OP)                              \
	friend CLASS operator OP(const CLASS &lhs, const fp_num &rhs) \
	{                                                             \
		CLASS ret = lhs;                                          \
		for (size_s i = 0; i < SZ; ++i)                           \
			for (size_s j = 0; j < SZ; ++j)                       \
				ret[i][j] OP## = rhs;                             \
		return ret;                                               \
	}

#define ALGEBRAIC_MAT_SQUARE(CLASS, SZ)                              \
	MAT_FP_NUM_OP(CLASS, SZ, +)                                      \
	MAT_FP_NUM_OP(CLASS, SZ, -)                                      \
	MAT_FP_NUM_OP(CLASS, SZ, /)                                      \
	MAT_FP_NUM_OP(CLASS, SZ, *)                                      \
	friend CLASS operator*(const CLASS &lhs, const CLASS &rhs)       \
	{                                                                \
		CLASS ret;                                                   \
		for (size_s r = 0; r < SZ; ++r)                              \
			for (size_s c = 0; c < SZ; ++c)                          \
				for (size_s i = 0; i < SZ; ++i)                      \
					ret[r][c] += lhs[r][i] * rhs[i][c];              \
		return ret;                                                  \
	}                                                                \
	friend vec##SZ operator*(const CLASS &lhs, const vec##SZ &rhs)   \
	{                                                                \
		vec##SZ ret;                                                 \
		for (size_s r = 0; r < SZ; ++r)                              \
			for (size_s c = 0; c < SZ; ++c)                          \
				ret.v[c] += rhs[r] * lhs[r][c];                      \
		return ret;                                                  \
	}                                                                \
	CLASS get_cofactor(int p, int q, int n) const                    \
	{                                                                \
		CLASS temp;                                                  \
		int i = 0, j = 0;                                            \
		for (int row = 0; row < n; ++row)                            \
			for (int col = 0; col < n; ++col)                        \
				if (row != p && col != q)                            \
				{                                                    \
					temp[i][j] = m[row][col];                        \
					if ((++j) == n - 1)                              \
					{                                                \
						j = 0;                                       \
						++i;                                         \
					}                                                \
				}                                                    \
		return temp;                                                 \
	}                                                                \
	fp_num determinant(int n = SZ) const                             \
	{                                                                \
		if (n == 1)                                                  \
			return m[0][0];                                          \
		fp_num D = 0;                                                \
		CLASS temp;                                                  \
		int sign = 1;                                                \
		for (int f = 0; f < n; ++f)                                  \
		{                                                            \
			temp = get_cofactor(0, f, n);                            \
			D += ((fp_num)sign) * m[0][f] * temp.determinant(n - 1); \
			sign = -sign;                                            \
		}                                                            \
		return D;                                                    \
	}                                                                \
	CLASS adjoint() const                                            \
	{                                                                \
		int sign = 1;                                                \
		CLASS temp;                                                  \
		CLASS adj;                                                   \
		for (size_s i = 0; i < 4; ++i)                               \
			for (size_s j = 0; j < 4; ++j)                           \
			{                                                        \
				temp = get_cofactor(i, j, 4);                        \
				sign = ((i + j) % 2 == 0) ? 1 : -1;                  \
				adj.m[j][i] = (fp_num)sign * temp.determinant(3);    \
			}                                                        \
		return adj;                                                  \
	}                                                                \
	CLASS inverse() const                                            \
	{                                                                \
		return adjoint() / determinant();                            \
	}

namespace galg
{
	typedef double fp_num;
	typedef unsigned short size_s;

	struct vec4;
	struct vec3;
	struct vec2;
	struct mat4;
	struct mat3;
	struct mat2;

	struct vec4
	{
		fp_num v[4];

		vec4(std::initializer_list<fp_num> lst)
		{
			short ctr = -1;
			for (auto i : lst)
				v[++ctr] = i;
		}
		vec4(fp_num x = 0, fp_num y = 0, fp_num z = 0, fp_num w = 1)
		{
			v[0] = x;
			v[1] = y;
			v[2] = z;
			v[3] = w;
		}

		FORCE_INLINE fp_num &x() { return v[0]; }
		FORCE_INLINE fp_num &y() { return v[1]; }
		FORCE_INLINE fp_num &z() { return v[2]; }
		FORCE_INLINE fp_num &w() { return v[3]; }
		FORCE_INLINE const fp_num &x() const { return v[0]; }
		FORCE_INLINE const fp_num &y() const { return v[1]; }
		FORCE_INLINE const fp_num &z() const { return v[2]; }
		FORCE_INLINE const fp_num &w() const { return v[3]; }

		FORCE_INLINE fp_num &operator[](size_t index)
		{
			return v[index];
		}
		FORCE_INLINE const fp_num &operator[](size_t index) const
		{
			return v[index];
		}

		ALGEBRAIC_VEC(vec4, 4)
	};

	struct vec3
	{
		fp_num v[3];

		vec3(std::initializer_list<fp_num> lst)
		{
			short ctr = -1;
			for (auto i : lst)
				v[++ctr] = i;
		}
		vec3(fp_num x = 0, fp_num y = 0, fp_num z = 0)
		{
			v[0] = x;
			v[1] = y;
			v[2] = z;
		}

		FORCE_INLINE fp_num &x() { return v[0]; }
		FORCE_INLINE fp_num &y() { return v[1]; }
		FORCE_INLINE fp_num &z() { return v[2]; }
		FORCE_INLINE const fp_num &x() const { return v[0]; }
		FORCE_INLINE const fp_num &y() const { return v[1]; }
		FORCE_INLINE const fp_num &z() const { return v[2]; }

		FORCE_INLINE fp_num &operator[](size_t index)
		{
			return v[index];
		}
		FORCE_INLINE const fp_num &operator[](size_t index) const
		{
			return v[index];
		}

		ALGEBRAIC_VEC(vec3, 3)
	};

	struct vec2
	{
		fp_num v[2];

		vec2(std::initializer_list<fp_num> lst)
		{
			short ctr = -1;
			for (auto i : lst)
				v[++ctr] = i;
		}
		vec2(fp_num x = 0, fp_num y = 0)
		{
			v[0] = x;
			v[1] = y;
		}

		FORCE_INLINE fp_num &x() { return v[0]; }
		FORCE_INLINE fp_num &y() { return v[1]; }
		FORCE_INLINE const fp_num &x() const { return v[0]; }
		FORCE_INLINE const fp_num &y() const { return v[1]; }

		FORCE_INLINE fp_num &operator[](size_t index)
		{
			return v[index];
		}
		FORCE_INLINE const fp_num &operator[](size_t index) const
		{
			return v[index];
		}

		ALGEBRAIC_VEC(vec2, 2)
	};

	struct mat4
	{
		fp_num m[4][4] = {0};

		mat4() {}
		mat4(std::initializer_list<std::initializer_list<fp_num>> list)
		{
			size_t r = 0;
			size_t c = 0;
			for (auto row : list)
			{
				for (auto val : row)
				{
					m[r][c] = val;
					if (++c >= 4)
						break;
				}
				c = 0;
				if (++r >= 4)
					break;
			}
		}

		fp_num *operator[](const size_t index)
		{
			return m[index];
		}

		const fp_num *operator[](const size_t index) const
		{
			return m[index];
		}

		static mat4 identity()
		{
			return mat4{
				{1, 0, 0, 0},
				{0, 1, 0, 0},
				{0, 0, 1, 0},
				{0, 0, 0, 1}};
		}

		ALGEBRAIC_MAT_SQUARE(mat4, 4)
	};
	struct mat3
	{
		fp_num m[3][3] = {0};

		mat3() {}
		mat3(std::initializer_list<std::initializer_list<fp_num>> list)
		{
			size_t r = 0;
			size_t c = 0;
			for (auto row : list)
			{
				for (auto val : row)
				{
					m[r][c] = val;
					if (++c >= 3)
						break;
				}
				c = 0;
				if (++r >= 3)
					break;
			}
		}

		fp_num *operator[](const size_t index)
		{
			return m[index];
		}

		const fp_num *operator[](const size_t index) const
		{
			return m[index];
		}

		static mat3 identity()
		{
			return mat3{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}};
		}

		ALGEBRAIC_MAT_SQUARE(mat3, 3)
	};
	struct mat2
	{
		fp_num m[2][2] = {0};

		mat2() {}
		mat2(std::initializer_list<std::initializer_list<fp_num>> list)
		{
			size_t r = 0;
			size_t c = 0;
			for (auto row : list)
			{
				for (auto val : row)
				{
					m[r][c] = val;
					if (++c >= 2)
						break;
				}
				c = 0;
				if (++r >= 2)
					break;
			}
		}

		fp_num *operator[](const size_t index)
		{
			return m[index];
		}

		const fp_num *operator[](const size_t index) const
		{
			return m[index];
		}

		static mat2 identity()
		{
			return mat2{
				{1, 0}, {0, 1}};
		}

		ALGEBRAIC_MAT_SQUARE(mat2, 2)
	};
}

#endif