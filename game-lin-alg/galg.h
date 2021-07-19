#ifndef GALG_H
#define GALG_H 1

#include <initializer_list>
#include <string.h>
#include <cmath>

#define FORCE_INLINE inline

#ifdef _WIN32
typedef unsigned int uint;
#endif

#define ALGEBRAIC_VEC(CLASS, NELEMS)                           \
	friend CLASS operator*(const CLASS &lhs, const CLASS &rhs) \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs[i] * rhs[i];                          \
		return ret;                                            \
	}                                                          \
	friend CLASS operator/(const CLASS &lhs, const CLASS &rhs) \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs[i] / rhs[i];                          \
		return ret;                                            \
	}                                                          \
	friend CLASS operator+(const CLASS &lhs, const CLASS &rhs) \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs[i] + rhs[i];                          \
		return ret;                                            \
	}                                                          \
	friend CLASS operator-(const CLASS &lhs, const CLASS &rhs) \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs[i] - rhs[i];                          \
		return ret;                                            \
	}                                                          \
	friend CLASS operator*(const CLASS &lhs, const T &rhs)     \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs[i] * rhs;                             \
		return ret;                                            \
	}                                                          \
	friend CLASS operator/(const CLASS &lhs, const T &rhs)     \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs[i] / rhs;                             \
		return ret;                                            \
	}                                                          \
	friend CLASS operator+(const CLASS &lhs, const T &rhs)     \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs[i] + rhs;                             \
		return ret;                                            \
	}                                                          \
	friend CLASS operator-(const CLASS &lhs, const T &rhs)     \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs[i] - rhs;                             \
		return ret;                                            \
	}                                                          \
	friend CLASS operator*(const T &lhs, const CLASS &rhs)     \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs * rhs[i];                             \
		return ret;                                            \
	}                                                          \
	friend CLASS operator/(const T &lhs, const CLASS &rhs)     \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs / rhs[i];                             \
		return ret;                                            \
	}                                                          \
	friend CLASS operator+(const T &lhs, const CLASS &rhs)     \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs + rhs[i];                             \
		return ret;                                            \
	}                                                          \
	friend CLASS operator-(const T &lhs, const CLASS &rhs)     \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs - rhs[i];                             \
		return ret;                                            \
	}                                                          \
	friend bool operator==(const CLASS &lhs, const CLASS &rhs) \
	{                                                          \
		bool res = true;                                       \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			res &= (lhs.v[i] == rhs.v[i]);                     \
		return res;                                            \
	}                                                          \
	friend bool operator!=(const CLASS &lhs, const CLASS &rhs) \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			if (lhs[i] != rhs[i])                              \
				return true;                                   \
		return false;                                          \
	}                                                          \
	CLASS &operator+=(const CLASS &rhs)                        \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			v[i] += rhs[i];                                    \
		return *this;                                          \
	}                                                          \
	CLASS &operator-=(const CLASS &rhs)                        \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			v[i] -= rhs[i];                                    \
		return *this;                                          \
	}                                                          \
	CLASS &operator/=(const CLASS &rhs)                        \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			v[i] /= rhs[i];                                    \
		return *this;                                          \
	}                                                          \
	CLASS &operator*=(const CLASS &rhs)                        \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			v[i] *= rhs[i];                                    \
		return *this;                                          \
	}                                                          \
	CLASS &operator+=(const T &rhs)                            \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			v[i] += rhs;                                       \
		return *this;                                          \
	}                                                          \
	CLASS &operator-=(const T &rhs)                            \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			v[i] -= rhs;                                       \
		return *this;                                          \
	}                                                          \
	CLASS &operator/=(const T &rhs)                            \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			v[i] /= rhs;                                       \
		return *this;                                          \
	}                                                          \
	CLASS &operator*=(const T &rhs)                            \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			v[i] *= rhs;                                       \
		return *this;                                          \
	}

#define VEC_STD_OPS(CLASS, NELEMS)              \
	T dot(const CLASS &v2) const                \
	{                                           \
		T sum = 0;                              \
		for (size_s i = 0; i < NELEMS; ++i)     \
			sum += (v[i] * v2[i]);              \
		return sum;                             \
	}                                           \
	T len() const                               \
	{                                           \
		return (T)sqrtf((float)dot(*this));     \
	}                                           \
	CLASS &normalize()                          \
	{                                           \
		T l = len();                            \
		if (l != 0)                             \
			for (size_s i = 0; i < NELEMS; ++i) \
				v[i] /= l;                      \
		return *this;                           \
	}                                           \
	CLASS normalized() const                    \
	{                                           \
		T l = len();                            \
		CLASS ret;                              \
		if (l != 0)                             \
			for (size_s i = 0; i < NELEMS; ++i) \
				ret.v[i] = v[i] / l;            \
		return ret;                             \
	}

#define MAT_NUM_OP(CLASS, SZ, OP)                            \
	friend CLASS operator OP(const CLASS &lhs, const T &rhs) \
	{                                                        \
		CLASS ret = lhs;                                     \
		for (size_s i = 0; i < SZ; ++i)                      \
			for (size_s j = 0; j < SZ; ++j)                  \
				ret[i][j] OP## = rhs;                        \
		return ret;                                          \
	}

#define ALGEBRAIC_MAT_SQUARE(CLASS, SZ)                                  \
	MAT_NUM_OP(CLASS, SZ, +)                                             \
	MAT_NUM_OP(CLASS, SZ, -)                                             \
	MAT_NUM_OP(CLASS, SZ, /)                                             \
	MAT_NUM_OP(CLASS, SZ, *)                                             \
	friend CLASS operator*(const CLASS &lhs, const CLASS &rhs)           \
	{                                                                    \
		CLASS ret;                                                       \
		for (size_s r = 0; r < SZ; ++r)                                  \
			for (size_s c = 0; c < SZ; ++c)                              \
				for (size_s i = 0; i < SZ; ++i)                          \
					ret[r][c] += lhs[r][i] * rhs[i][c];                  \
		return ret;                                                      \
	}                                                                    \
	friend bool operator==(const CLASS &lhs, const CLASS &rhs)           \
	{                                                                    \
		for (size_s r = 0; r < SZ; ++r)                                  \
			for (size_s c = 0; c < SZ; ++c)                              \
				if (lhs[r][c] != rhs[r][c])                              \
					return false;                                        \
		return true;                                                     \
	}                                                                    \
	friend bool operator!=(const CLASS &lhs, const CLASS &rhs)           \
	{                                                                    \
		return !(lhs == rhs);                                            \
	}                                                                    \
	friend vec##SZ<T> operator*(const CLASS &lhs, const vec##SZ<T> &rhs) \
	{                                                                    \
		vec##SZ<T> ret;                                                  \
		for (size_s r = 0; r < SZ; ++r)                                  \
			for (size_s c = 0; c < SZ; ++c)                              \
				ret.v[c] += rhs[r] * lhs[r][c];                          \
		return ret;                                                      \
	}                                                                    \
	CLASS get_cofactor(int p, int q, int n) const                        \
	{                                                                    \
		CLASS temp;                                                      \
		int i = 0, j = 0;                                                \
		for (int row = 0; row < n; ++row)                                \
			for (int col = 0; col < n; ++col)                            \
				if (row != p && col != q)                                \
				{                                                        \
					temp[i][j] = m[row][col];                            \
					if ((++j) == n - 1)                                  \
					{                                                    \
						j = 0;                                           \
						++i;                                             \
					}                                                    \
				}                                                        \
		return temp;                                                     \
	}                                                                    \
	T determinant(int n = SZ) const                                      \
	{                                                                    \
		if (n == 1)                                                      \
			return m[0][0];                                              \
		T D = 0;                                                         \
		CLASS temp;                                                      \
		int sign = 1;                                                    \
		for (int f = 0; f < n; ++f)                                      \
		{                                                                \
			temp = get_cofactor(0, f, n);                                \
			D += ((T)sign) * m[0][f] * temp.determinant(n - 1);          \
			sign = -sign;                                                \
		}                                                                \
		return D;                                                        \
	}                                                                    \
	CLASS adjoint() const                                                \
	{                                                                    \
		int sign = 1;                                                    \
		CLASS temp;                                                      \
		CLASS adj;                                                       \
		for (size_s i = 0; i < 4; ++i)                                   \
			for (size_s j = 0; j < 4; ++j)                               \
			{                                                            \
				temp = get_cofactor(i, j, 4);                            \
				sign = ((i + j) % 2 == 0) ? 1 : -1;                      \
				adj.m[j][i] = (T)sign * temp.determinant(3);             \
			}                                                            \
		return adj;                                                      \
	}                                                                    \
	CLASS inverse() const                                                \
	{                                                                    \
		return adjoint() / determinant();                                \
	}

namespace galg
{
	typedef float fp_num;
	typedef unsigned short size_s;

	template <typename T>
	struct vec4;
	template <typename T>
	struct vec3;
	template <typename T>
	struct vec2;
	template <typename T>
	struct mat4;
	template <typename T>
	struct mat3;
	template <typename T>
	struct mat2;

	typedef vec4<fp_num> vec4fp;
	typedef vec4<float> vec4f;
	typedef vec4<double> vec4d;
	typedef vec4<int> vec4i;
	typedef vec4<uint> vec4ui;
	typedef vec4<size_t> vec4st;

	typedef vec3<fp_num> vec3fp;
	typedef vec3<float> vec3f;
	typedef vec3<double> vec3d;
	typedef vec3<int> vec3i;
	typedef vec3<uint> vec3ui;
	typedef vec3<size_t> vec3st;

	typedef vec2<fp_num> vec2fp;
	typedef vec2<float> vec2f;
	typedef vec2<double> vec2d;
	typedef vec2<int> vec2i;
	typedef vec2<uint> vec2ui;
	typedef vec2<size_t> vec2st;

	typedef mat4<fp_num> mat4fp;
	typedef mat4<float> mat4f;
	typedef mat4<double> mat4d;
	typedef mat4<int> mat4i;
	typedef mat4<uint> mat4ui;
	typedef mat4<size_t> mat4st;

	typedef mat3<fp_num> mat3fp;
	typedef mat3<float> mat3f;
	typedef mat3<double> mat3d;
	typedef mat3<int> mat3i;
	typedef mat3<uint> mat3ui;
	typedef mat3<size_t> mat3st;

	typedef mat2<fp_num> mat2fp;
	typedef mat2<float> mat2f;
	typedef mat2<double> mat2d;
	typedef mat2<int> mat2i;
	typedef mat2<uint> mat2ui;
	typedef mat2<size_t> mat2st;

	template <typename T = fp_num>
	struct vec4
	{
		T v[4];

		vec4(std::initializer_list<T> lst)
		{
			v[3] = 1;
			short ctr = -1;
			for (auto i : lst)
				v[++ctr] = i;
		}

		vec4(T x = 0, T y = 0, T z = 0, T w = (T)1)
		{
			v[0] = x;
			v[1] = y;
			v[2] = z;
			v[3] = w;
		}

		FORCE_INLINE T &x() { return v[0]; }
		FORCE_INLINE T &y() { return v[1]; }
		FORCE_INLINE T &z() { return v[2]; }
		FORCE_INLINE T &w() { return v[3]; }
		FORCE_INLINE const T &x() const { return v[0]; }
		FORCE_INLINE const T &y() const { return v[1]; }
		FORCE_INLINE const T &z() const { return v[2]; }
		FORCE_INLINE const T &w() const { return v[3]; }

		FORCE_INLINE T &operator[](size_t index)
		{
			return v[index];
		}
		FORCE_INLINE const T &operator[](size_t index) const
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

	template <typename T = fp_num>
	struct vec3
	{
		T v[3];

		vec3(std::initializer_list<T> lst)
		{
			short ctr = -1;
			for (auto i : lst)
				v[++ctr] = i;
		}
		vec3(T x = 0, T y = 0, T z = 0)
		{
			v[0] = x;
			v[1] = y;
			v[2] = z;
		}

		FORCE_INLINE T &x() { return v[0]; }
		FORCE_INLINE T &y() { return v[1]; }
		FORCE_INLINE T &z() { return v[2]; }
		FORCE_INLINE const T &x() const { return v[0]; }
		FORCE_INLINE const T &y() const { return v[1]; }
		FORCE_INLINE const T &z() const { return v[2]; }

		FORCE_INLINE T &operator[](size_t index)
		{
			return v[index];
		}
		FORCE_INLINE const T &operator[](size_t index) const
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

	template <typename T = fp_num>
	struct vec2
	{
		T v[2];

		vec2(std::initializer_list<T> lst)
		{
			short ctr = -1;
			for (auto i : lst)
				v[++ctr] = i;
		}
		vec2(T x = 0, T y = 0)
		{
			v[0] = x;
			v[1] = y;
		}

		FORCE_INLINE T &x() { return v[0]; }
		FORCE_INLINE T &y() { return v[1]; }
		FORCE_INLINE const T &x() const { return v[0]; }
		FORCE_INLINE const T &y() const { return v[1]; }

		FORCE_INLINE T &operator[](size_t index)
		{
			return v[index];
		}
		FORCE_INLINE const T &operator[](size_t index) const
		{
			return v[index];
		}

		ALGEBRAIC_VEC(vec2, 2)
		VEC_STD_OPS(vec2, 2)
	};

	template <typename T = fp_num>
	struct mat4
	{
		T m[4][4] = {0};

		mat4() {}
		mat4(std::initializer_list<std::initializer_list<T>> list)
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

		T *operator[](const size_t index)
		{
			return m[index];
		}

		const T *operator[](const size_t index) const
		{
			return m[index];
		}

		static mat4 identity()
		{
			return mat4{
				{(T)1, 0, 0, 0},
				{0, (T)1, 0, 0},
				{0, 0, (T)1, 0},
				{0, 0, 0, (T)1}};
		}

		ALGEBRAIC_MAT_SQUARE(mat4, 4)

		static mat4 perspective(T fovDeg, T aspectRatio, T near, T far)
		{
			auto deg_to_rad = [](T deg)
			{
				return deg * (3.14159265 / 180.000);
			};
			T fovRad = (T)1.0 / (T)tanf((float)deg_to_rad(fovDeg * (T)0.5));
			return mat4{{fovRad * aspectRatio, 0, 0, 0},
						{0, fovRad, 0, 0},
						{0, 0, far / (far - near), 1},
						{0, 0, (-far * near) / (far - near), 0}};
		}
		static mat4 orthographic(T left, T right, T bottom, T top, T near = (T)-1, T far = (T)1)
		{
			return mat4{
				{(T)2.0 / (right - left), 0, 0, 0},
				{0, (T)2.0 / (top - bottom), 0, 0},
				{0, 0, -(T)2.0 / (far - near), 0},
				{-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), (T)1}};
		}
		static mat4 look_at(const vec4<T> &pos, const vec4<T> &target, const vec4<T> &up)
		{
			auto new_forward = (target - pos).normalize();
			auto new_up = (up - (new_forward * up.dot(new_forward))).normalize();
			auto new_right = new_up.cross(new_forward);
			return mat4{
				{new_right.x(), new_right.y(), new_right.z(), 0},
				{new_up.x(), new_up.y(), new_up.z(), 0},
				{new_forward.x(), new_forward.y(), new_forward.z(), 0},
				{pos.x(), pos.y(), pos.z(), (T)1}};
		}
		FORCE_INLINE static mat4 scale(T x, T y, T z)
		{
			return mat4{
				{x, 0, 0, 0},
				{0, y, 0, 0},
				{0, 0, z, 0},
				{0, 0, 0, (T)1}};
		}
		FORCE_INLINE static mat4 scale(vec4<T> s)
		{
			return scale(s.x(), s.y(), s.z());
		}
		FORCE_INLINE static mat4 translation(T x, T y, T z)
		{
			return mat4{
				{(T)1, 0, 0, 0},
				{0, (T)1, 0, 0},
				{0, 0, (T)1, 0},
				{x, y, z, (T)1}};
		}
		FORCE_INLINE static mat4 translation(vec4<T> t)
		{
			return translation(t.x(), t.y(), t.z());
		}
		FORCE_INLINE static mat4 x_rotation(T x)
		{
			return mat4{
				{(T)1, 0, 0, 0},
				{0, (T)cosf(x), (T)sinf(x), 0},
				{0, (T)-sinf(x), (T)cosf(x), 0},
				{0, 0, 0, (T)1}};
		}
		FORCE_INLINE static mat4 y_rotation(T y)
		{
			return mat4{
				{(T)cosf(y), 0, (T)sinf(y), 0},
				{0, 1, 0, 0},
				{(T)-sinf(y), 0, (T)cosf(y), 0},
				{0, 0, 0, 1}};
		}
		FORCE_INLINE static mat4 z_rotation(T z)
		{
			return mat4{
				{(T)cosf(z), (T)sinf(z), 0, 0},
				{(T)-sinf(z), (T)cosf(z), 0, 0},
				{0, 0, (T)1, 0},
				{0, 0, 0, (T)1}};
		}
		FORCE_INLINE static mat4 rotation(T x, T y, T z)
		{
			return x_rotation(x) * y_rotation(y) * z_rotation(z);
		}
		FORCE_INLINE static mat4 rotation(vec4<T> r)
		{
			return rotation(r.x(), r.y(), r.z());
		}
	};

	template <typename T = fp_num>
	struct mat3
	{
		T m[3][3] = {0};

		mat3() {}
		mat3(std::initializer_list<std::initializer_list<T>> list)
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

		T *operator[](const size_t index)
		{
			return m[index];
		}

		const T *operator[](const size_t index) const
		{
			return m[index];
		}

		static mat3 identity()
		{
			return mat3{
				{(T)1, 0, 0},
				{0, (T)1, 0},
				{0, 0, (T)1}};
		}

		ALGEBRAIC_MAT_SQUARE(mat3, 3)
	};

	template <typename T = fp_num>
	struct mat2
	{
		T m[2][2] = {0};

		mat2() {}
		mat2(std::initializer_list<std::initializer_list<T>> list)
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

		T *operator[](const size_t index)
		{
			return m[index];
		}

		const T *operator[](const size_t index) const
		{
			return m[index];
		}

		static mat2 identity()
		{
			return mat2{
				{(T)1, 0}, {0, (T)1}};
		}

		ALGEBRAIC_MAT_SQUARE(mat2, 2)

		FORCE_INLINE static mat2 rotation(T v)
		{
			return mat2{
				{(T)cosf(v), (T)sinf(v)},
				{(T)-sinf(v), (T)cosf(v)}};
		}

		FORCE_INLINE static mat2 scale(T x, T y)
		{
			return mat2{{x, 0}, {0, y}};
		}
	};
}

#endif