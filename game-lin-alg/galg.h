#ifndef GALG_H
#define GALG_H 1

#include <initializer_list>
#include <string.h>
#include <cmath>

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
	}                                                           \
	CLASS &operator+=(const CLASS &rhs)                         \
	{                                                           \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			v[i] += rhs[i];                                     \
		return *this;                                           \
	}                                                           \
	CLASS &operator-=(const CLASS &rhs)                         \
	{                                                           \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			v[i] -= rhs[i];                                     \
		return *this;                                           \
	}                                                           \
	CLASS &operator/=(const CLASS &rhs)                         \
	{                                                           \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			v[i] /= rhs[i];                                     \
		return *this;                                           \
	}                                                           \
	CLASS &operator*=(const CLASS &rhs)                         \
	{                                                           \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			v[i] *= rhs[i];                                     \
		return *this;                                           \
	}                                                           \
	CLASS &operator+=(const fp_num &rhs)                        \
	{                                                           \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			v[i] += rhs;                                        \
		return *this;                                           \
	}                                                           \
	CLASS &operator-=(const fp_num &rhs)                        \
	{                                                           \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			v[i] -= rhs;                                        \
		return *this;                                           \
	}                                                           \
	CLASS &operator/=(const fp_num &rhs)                        \
	{                                                           \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			v[i] /= rhs;                                        \
		return *this;                                           \
	}                                                           \
	CLASS &operator*=(const fp_num &rhs)                        \
	{                                                           \
		for (size_s i = 0; i < NELEMS; ++i)                     \
			v[i] *= rhs;                                        \
		return *this;                                           \
	}

#define VEC_STD_OPS(CLASS, NELEMS)              \
	fp_num dot(const CLASS &v2) const           \
	{                                           \
		fp_num sum = 0;                         \
		for (size_s i = 0; i < NELEMS; ++i)     \
			sum += (v[i] * v2[i]);              \
		return sum;                             \
	}                                           \
	fp_num len() const                          \
	{                                           \
		return sqrtf((float)dot(*this));        \
	}                                           \
	CLASS &normalize()                          \
	{                                           \
		fp_num l = len();                       \
		if (l != 0)                             \
			for (size_s i = 0; i < NELEMS; ++i) \
				v[i] /= l;                      \
		return *this;                           \
	}                                           \
	CLASS normalized() const                    \
	{                                           \
		fp_num l = len();                       \
		CLASS ret;                              \
		if (l != 0)                             \
			for (size_s i = 0; i < NELEMS; ++i) \
				ret.v[i] = v[i] / l;            \
		return ret;                             \
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
	friend bool operator==(const CLASS &lhs, const CLASS &rhs)       \
	{                                                                \
		for (size_s r = 0; r < SZ; ++r)                              \
			for (size_s c = 0; c < SZ; ++c)                          \
				if (lhs[r][c] != rhs[r][c])                          \
					return false;                                    \
		return true;                                                 \
	}                                                                \
	friend bool operator!=(const CLASS &lhs, const CLASS &rhs)       \
	{                                                                \
		return !(lhs == rhs);                                        \
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
	typedef float fp_num;
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
			v[3] = 1;
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

		vec4 cross(vec4 &v2) const
		{
			return vec4(
				(y() * v2.z()) - (z() * v2.y()),
				(z() * v2.x()) - (x() * v2.z()),
				(x() * v2.y()) - (y() * v2.x()));
		}

		ALGEBRAIC_VEC(vec4, 4)
		VEC_STD_OPS(vec4, 3)
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

		vec3 cross(vec3 &v2) const
		{
			return vec3(
				(y() * v2.z()) - (z() * v2.y()),
				(z() * v2.x()) - (x() * v2.z()),
				(x() * v2.y()) - (y() * v2.x()));
		}

		ALGEBRAIC_VEC(vec3, 3)
		VEC_STD_OPS(vec3, 3)
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
		VEC_STD_OPS(vec2, 2)
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
		template <typename T>
		T **to_array()
		{
			T **ret = new T *[4];
			for (size_s i = 0; i < 4; ++i)
			{
				ret[i] = new T[4];
				for (size_s j = 0; j < 4; ++j)
					ret[i][j] = (T)m[i][j];
			}
			return ret;
		}
		template <typename T>
		static void delete_array(T **arr)
		{
			for (size_s i = 0; i < 4; ++i)
				delete[] arr[i];
			delete[] arr;
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
		static mat4 perspective(fp_num fovDeg, fp_num aspectRatio, fp_num near, fp_num far)
		{
			auto deg_to_rad = [](fp_num deg)
			{
				return deg * (3.14159265 / 180.000);
			};
			fp_num fovRad = (fp_num)1.0 / (fp_num)tanf((float)deg_to_rad(fovDeg * (fp_num)0.5));
			return mat4{{fovRad * aspectRatio, 0, 0, 0},
						{0, fovRad, 0, 0},
						{0, 0, far / (far - near), 1},
						{0, 0, (-far * near) / (far - near), 0}};
		}
		static mat4 orthographic(fp_num left, fp_num right, fp_num bottom, fp_num top, fp_num near = -1, fp_num far = 1)
		{
			return mat4{
				{(fp_num)2.0 / (right - left), 0, 0, 0},
				{0, (fp_num)2.0 / (top - bottom), 0, 0},
				{0, 0, -(fp_num)2.0 / (far - near), 0},
				{-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1}};
		}
		static mat4 look_at(const vec4 &pos, const vec4 &target, const vec4 &up)
		{
			auto new_forward = (target - pos).normalize();
			auto new_up = (up - (new_forward * up.dot(new_forward))).normalize();
			auto new_right = new_up.cross(new_forward);
			return mat4{
				{new_right.x(), new_right.y(), new_right.z(), 0},
				{new_up.x(), new_up.y(), new_up.z(), 0},
				{new_forward.x(), new_forward.y(), new_forward.z(), 0},
				{pos.x(), pos.y(), pos.z(), 1}};
		}
		FORCE_INLINE static mat4 scale(fp_num x, fp_num y, fp_num z)
		{
			return mat4{
				{x, 0, 0, 0},
				{0, y, 0, 0},
				{0, 0, z, 0},
				{0, 0, 0, 1}};
		}
		FORCE_INLINE static mat4 scale(vec4 s)
		{
			return scale(s.x(), s.y(), s.z());
		}
		FORCE_INLINE static mat4 translation(fp_num x, fp_num y, fp_num z)
		{
			return mat4{
				{1, 0, 0, 0},
				{0, 1, 0, 0},
				{0, 0, 1, 0},
				{x, y, z, 1}};
		}
		FORCE_INLINE static mat4 translation(vec4 t)
		{
			return translation(t.x(), t.y(), t.z());
		}
		FORCE_INLINE static mat4 x_rotation(fp_num x)
		{
			return mat4{
				{1, 0, 0, 0},
				{0, cosf(x), sinf(x), 0},
				{0, -sinf(x), cosf(x), 0},
				{0, 0, 0, 1}};
		}
		FORCE_INLINE static mat4 y_rotation(fp_num y)
		{
			return mat4{
				{cosf(y), 0, sinf(y), 0},
				{0, 1, 0, 0},
				{-sinf(y), 0, cosf(y), 0},
				{0, 0, 0, 1}};
		}
		FORCE_INLINE static mat4 z_rotation(fp_num z)
		{
			return mat4{
				{cosf(z), sinf(z), 0, 0},
				{-sinf(z), cosf(z), 0, 0},
				{0, 0, 1, 0},
				{0, 0, 0, 1}};
		}
		FORCE_INLINE static mat4 rotation(fp_num x, fp_num y, fp_num z)
		{
			return x_rotation(x) * y_rotation(y) * z_rotation(z);
		}
		FORCE_INLINE static mat4 rotation(vec4 r)
		{
			return rotation(r.x(), r.y(), r.z());
		}
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

		FORCE_INLINE static mat2 rotation(fp_num v)
		{
			return mat2{
				{cosf(v), sinf(v)},
				{-sinf(v), cosf(v)}};
		}

		FORCE_INLINE static mat2 scale(fp_num x, fp_num y)
		{
			return mat2{{x, 0}, {0, y}};
		}
	};
}

#endif