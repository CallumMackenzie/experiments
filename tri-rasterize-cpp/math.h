#ifndef MATH_H_
#define MATH_H_ 1

#include <initializer_list>
#include <stddef.h>
#include <iostream>
#include <cmath>

namespace cmm
{

	typedef long double vec_hash;

#define BIT_OP(OP, TYPE)                                      \
	friend TYPE operator OP(const TYPE &lhs, const TYPE &rhs) \
	{                                                         \
		TYPE ret;                                             \
		for (size_t i = 0; i < TYPE::length; i++)             \
			ret.vec[i] = lhs.vec[i] OP rhs.vec[i];            \
		return ret;                                           \
	};                                                        \
	friend TYPE operator OP(const TYPE &lhs, const T &rhs)    \
	{                                                         \
		TYPE ret;                                             \
		for (size_t i = 0; i < TYPE::length; i++)             \
			ret.vec[i] = lhs.vec[i] OP rhs;                   \
		return ret;                                           \
	};                                                        \
	friend TYPE operator OP(const T &lhs, const TYPE &rhs)    \
	{                                                         \
		return rhs * lhs;                                     \
	};                                                        \
	TYPE &operator OP##=(const TYPE &rhs)                     \
	{                                                         \
		for (size_t i = 0; i < TYPE::length; i++)             \
			this->vec[i] OP## = rhs.vec[i];                   \
		return *this;                                         \
	};                                                        \
	TYPE &operator OP##=(const T &rhs)                        \
	{                                                         \
		for (size_t i = 0; i < TYPE::length; i++)             \
			this->vec[i] OP## = rhs;                          \
		return *this;                                         \
	};

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

#define OSTREAM_OP(...)                                                       \
	friend std::ostream &operator<<(std::ostream &os, const __VA_ARGS__ &val) \
	{                                                                         \
		return os << val.to_string();                                         \
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

#define LVEC_DECL(NUM_ELEMS)        \
	template <typename T, size_t N> \
	class lvec##NUM_ELEMS;          \
	template <typename T>           \
	using lv##NUM_ELEMS = lvec##NUM_ELEMS<T, NUM_ELEMS>;

	LVEC_DECL(1)
	LVEC_DECL(2)
	LVEC_DECL(3)
	LVEC_DECL(4)

	template <typename T, size_t LEN>
	class nvec
	{
	public:
		static const size_t length = LEN;
		T vec[LEN] = {0};

		nvec() {}

		nvec(std::initializer_list<T> list)
		{
			size_t i = -1;
			for (auto item : list)
				vec[++i] = item;
		}

		template <typename E>
		nvec(nvec<E, LEN> v)
		{
			for (int i = 0; i < LEN; ++i)
				vec[i] = (T)v[i];
		}

		const inline size_t size() const
		{
			return LEN;
		}

		FULL_NUMERIC(nvec)

		T &operator[](const size_t index)
		{
			return vec[index];
		}

		const T &operator[](const size_t index) const
		{
			return vec[index];
		}

		nvec &make_abs()
		{
			for (size_t i = 0; i < LEN; ++i)
				vec[i] = std::abs(vec[i]);
			return *this;
		}

		nvec &fill(const T val)
		{
			for (size_t i = 0; i < LEN; i++)
				vec[i] = val;
			return *this;
		}

		T dot(const nvec &v2) const
		{
			T sum = (T)0;
			for (size_t i = 0; i < LEN; i++)
				sum += (vec[i] * v2[i]);
			return sum;
		}

		T len() const
		{
			return sqrtf(dot(*this));
		}

		nvec &normalize()
		{
			T l = len();
			if (l != (T)0)
				for (size_t i = 0; i < LEN; i++)
					vec[i] /= l;
			return *this;
		}

		nvec normalized() const
		{
			T l = len();
			nvec ret;
			if (l != (T)0)
				for (size_t i = 0; i < LEN; i++)
					ret.vec[i] = vec[i] / l;
			return ret;
		}

		nvec cross(nvec v2) const
		{
			nvec ret;
			if (LEN == 3)
			{
				ret.vec[0] = (vec[1] * v2.vec[2]) - (vec[2] * v2.vec[1]);
				ret.vec[1] = (vec[2] * v2.vec[0]) - (vec[0] * v2.vec[2]);
				ret.vec[2] = (vec[0] * v2.vec[1]) - (vec[1] * v2.vec[0]);
			}
			return ret;
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

#define LVEC_OP_EQ(NUM_ELEMS)                          \
	// lvec##NUM_ELEMS &operator=(nvec<T, NUM_ELEMS> vec) \
	// {                                                  \
	// 	for (size_t i = 0; i < NUM_ELEMS; i++)         \
	// 		this->vec[i] = vec[i];                     \
	// 	return *this;                                  \
	// }                                                  \
	// lvec##NUM_ELEMS &operator=(lvec##NUM_ELEMS vec)    \
	// {                                                  \
	// 	for (size_t i = 0; i < NUM_ELEMS; i++)         \
	// 		this->vec[i] = vec[i];                     \
	// 	return *this;                                  \
	// }

	template <typename T, size_t NELEMS = 1>
	class lvec1 : public nvec<T, NELEMS>
	{
	private:
		using parent = nvec<T, NELEMS>;

	public:
		lvec1() : parent() {}
		lvec1(std::initializer_list<T> list) : parent(list) {}
		T &x() { return this->vec[0]; }
		lv2<T> xx() { return lv2<T>{x(), x()}; }
		lv3<T> xxx() { return lv3<T>{x(), x(), x()}; }
		lv4<T> xxxx() { return lv4<T>{x(), x(), x(), x()}; }

		LVEC_OP_EQ(1)
	};

	template <typename T, size_t NELEMS = 2>
	class lvec2 : public lvec1<T, NELEMS>
	{
	private:
		using parent = lvec1<T, NELEMS>;

	public:
		lvec2() : parent() {}
		lvec2(std::initializer_list<T> list) : parent(list) {}
		lvec2(nvec<T, 2> base)
		{
			for (size_t i = 0; i < 2; i++)
				this->vec[i] = base.vec[i];
		}

		T &x() { return this->vec[0]; }
		T &y() { return this->vec[1]; }

		lv4<T> yyyy() { return lv4<T>{y(), y(), y(), y()}; }
		lv3<T> yyy() { return lv3<T>{y(), y(), y()}; }
		lv2<T> yy() { return lv2<T>{y(), y()}; }
		lv2<T> yx() { return lv2<T>{y(), x()}; }
		lv2<T> xy() { return lv2<T>{x(), y()}; }
		lv3<T> xxy() { return lv3<T>{x(), x(), y()}; }
		lv3<T> xyy() { return lv3<T>{x(), y(), y()}; }
		lv3<T> yxx() { return lv3<T>{y(), x(), x()}; }
		lv3<T> yyx() { return lv3<T>{y(), y(), x()}; }
		lv3<T> xyx() { return lv3<T>{x(), y(), x()}; }

		nvec<T, NELEMS> to_nvec()
		{
			return nvec<T, NELEMS>{x(), y()};
		}

		LVEC_OP_EQ(2)
	};

	template <typename T, size_t NELEMS = 3>
	class lvec3 : public lvec2<T, NELEMS>
	{
	private:
		using parent = lvec2<T, NELEMS>;

	public:
		lvec3() : parent() {}
		lvec3(std::initializer_list<T> list) : parent(list) {}
		lvec3(parent base, T next)
		{
			for (size_t i = 0; i < 2; i++)
				this->vec[i] = base.vec[i];
			this->vec[2] = next;
		}
		lvec3(nvec<T, 3> base)
		{
			for (size_t i = 0; i < NELEMS; i++)
				this->vec[i] = base.vec[i];
		}

		T &x() { return this->vec[0]; }
		T &y() { return this->vec[1]; }
		T &z() { return this->vec[2]; }

		lv2<T> zz() { return lv2<T>{z(), z()}; }
		lv3<T> zzz() { return lv3<T>{z(), z(), z()}; }
		lv4<T> zzzz() { return lv4<T>{z(), z(), z(), z()}; }
		lv3<T> xyz() { return lv3<T>{x(), y(), z()}; }
		lv3<T> zyx() { return lv3<T>{z(), y(), x()}; }
		lv4<T> xyz1() { return lv4<T>{x(), y(), z(), (T)1}; }

		nvec<T, NELEMS> to_nvec()
		{
			return nvec<T, NELEMS>{x(), y(), z()};
		}

		LVEC_OP_EQ(3)
	};

	template <typename T, size_t NELEMS = 4>
	class lvec4 : public lvec3<T, NELEMS>
	{
	private:
		using parent = lvec3<T, NELEMS>;

	public:
		lvec4() : parent() {}
		lvec4(std::initializer_list<T> list) : parent(list) {}
		lvec4(parent base, T next)
		{
			for (size_t i = 0; i < 3; i++)
				this->vec[i] = base.vec[i];
			this->vec[3] = next;
		}
		lvec4(nvec<T, 4> base)
		{
			for (size_t i = 0; i < NELEMS; i++)
				this->vec[i] = base.vec[i];
		}

		T &x() { return this->vec[0]; }
		T &y() { return this->vec[1]; }
		T &z() { return this->vec[2]; }
		T &w() { return this->vec[3]; }

		nvec<T, NELEMS> to_nvec()
		{
			return nvec<T, NELEMS>{x(), y(), z(), w()};
		}

		LVEC_OP_EQ(4)
	};

	struct matrix_exception
	{
		std::string what;

		matrix_exception(std::string what_)
		{
			what = what_;
		}
	};

#define MFNS mat_fns
	template <typename T, size_t W, size_t H>
	class MFNS;

	template <typename T, size_t W, size_t H>
	class mat
	{
	public:
		T m[H][W] = {0};

		mat() {}
		mat(std::initializer_list<std::initializer_list<T>> list) : mat()
		{
			size_t r = 0;
			size_t c = 0;
			for (auto row : list)
			{
				for (auto val : row)
				{
					m[r][c] = val;
					if (++c >= W)
						break;
				}
				c = 0;
				if (++r >= H)
					break;
			}
		}

		inline size_t width() const
		{
			return W;
		}

		inline size_t height() const
		{
			return H;
		}

		std::string to_string() const
		{
			std::string str("{");
			for (size_t i = 0; i < H; i++)
			{
				str.append((i == 0) ? "{" : " {");
				for (size_t j = 0; j < W; j++)
					str.append(std::to_string(m[i][j])).append(((j + 1) == W) ? "" : ", ");
				str.append(((i + 1) == H) ? "}" : "}\n");
			}
			return str.append("}");
		}

		mat get_cofactor(int p, int q, int n) const
		{
			mat temp;
			int i = 0, j = 0;
			for (int row = 0; row < n; ++row)
				for (int col = 0; col < n; ++col)
					if (row != p && col != q)
					{
						temp[i][j] = m[row][col];
						if ((++j) == n - 1)
						{
							j = 0;
							++i;
						}
					}
			return temp;
		}

		T determinant(int n = H) const
		{
			if (n == 1)
				return m[0][0];
			T D = 0;
			mat temp;
			int sign = 1;
			for (int f = 0; f < n; ++f)
			{
				temp = get_cofactor(0, f, n);
				D += ((T)sign) * m[0][f] * temp.determinant(n - 1);
				sign = -sign;
			}
			return D;
		}

		mat adjoint() const
		{
			if (H == 1)
				return identity();

			int sign = 1;
			mat temp;
			mat adj;

			for (int i = 0; i < H; i++)
				for (int j = 0; j < W; j++)
				{
					temp = get_cofactor(i, j, H);
					sign = ((i + j) % 2 == 0) ? 1 : -1;
					adj[j][i] = (T)sign * temp.determinant(H - 1);
				}
			return adj;
		}

		mat inverse() const
		{
			T det = determinant();
			if (det == 0)
				throw matrix_exception("Cannot find matrix of singular matrix.");

			return adjoint() / det;
		}

		inline T *operator[](const size_t index)
		{
			return m[index];
		}

		inline const T *operator[](const size_t index) const
		{
			return m[index];
		}

		friend mat operator*(const T &lhs, const mat &rhs)
		{
			mat ret = rhs;
			for (size_t i = 0; i < ret.height(); i++)
				for (size_t j = 0; j < ret.width(); j++)
					ret[i][j] *= lhs;
			return ret;
		}

		inline friend mat operator*(const mat &lhs, const T &rhs)
		{
			return rhs * lhs;
		}

		friend mat operator/(const mat &lhs, const T &rhs)
		{
			mat ret = lhs;
			for (size_t i = 0; i < ret.height(); i++)
				for (size_t j = 0; j < ret.width(); j++)
					ret[i][j] /= rhs;
			return ret;
		}

		friend mat<T, H, H> operator*(const mat<T, W, H> &lhs, const mat<T, H, W> &rhs)
		{
			mat<T, H, H> ret;
			for (size_t r = 0; r < H; r++)
				for (size_t c = 0; c < H; c++)
					for (size_t i = 0; i < W; i++)
						ret[r][c] += lhs[r][i] * rhs[i][c];
			return ret;
		}

		friend nvec<T, H> operator*(const mat<T, W, H> &lhs, const nvec<T, H> &rhs)
		{
			nvec<T, W> ret;
			for (size_t r = 0; r < H; r++)
				for (size_t c = 0; c < W; c++)
					ret.vec[c] += rhs[r] * lhs[r][c];
			return ret;
		}

		static mat identity()
		{
			mat ret;
			for (size_t i = 0; i < (W > H ? H : W); i++)
				ret[i][i] = (T)1;
			return ret;
		}

		OSTREAM_OP(mat)
	};

	template <typename T>
	class MFNS<T, 4, 4>
	{
	public:
		static mat<T, 4, 4> projection(T fovDeg, T aspectRatio, T near, T far)
		{
			auto deg_to_rad = [](T deg)
			{
				return deg * (3.14159265 / 180.000);
			};
			T fovRad = 1.00 / tanf(deg_to_rad(fovDeg * 0.500));
			T zero = (T)0;
			mat<T, 4, 4> ret{{fovRad * aspectRatio, zero, zero, zero},
							 {zero, fovRad, zero, zero},
							 {zero, zero, far / (far - near), (T)1},
							 {zero, zero, (-far * near) / (far - near), zero}};
			return ret;
		}

		static mat<T, 4, 4> look_at(nvec<T, 3> pos, nvec<T, 3> target, nvec<T, 3> up = nvec<T, 3>{0, 1, 0})
		{
			auto new_forward = (target - pos).normalize();
			auto new_up = (up - (new_forward * up.dot(new_forward))).normalize();
			auto new_right = new_up.cross(new_forward);
			auto m = mat<T, 4, 4>{
				{new_right[0], new_right[1], new_right[2]},
				{new_up[0], new_up[1], new_up[2]},
				{new_forward[0], new_forward[1], new_forward[2]},
				{pos[0], pos[1], pos[2], (T)1}};
			return m;
		}

		static mat<T, 4, 4> scale(T x, T y, T z)
		{
			auto m = mat<T, 4, 4>::identity();
			m[0][0] = x;
			m[1][1] = y;
			m[2][2] = z;
			return m;
		}

		static inline mat<T, 4, 4> scale(const nvec<T, 3> v)
		{
			return scale(v[0], v[1], v[2]);
		}

		static mat<T, 4, 4> translation(T x, T y, T z)
		{
			auto m = mat<T, 4, 4>::identity();
			m[3][0] = x;
			m[3][1] = y;
			m[3][2] = z;
			// m[0][3] = x;
			// m[1][3] = y;
			// m[2][3] = z;
			return m;
		}

		static inline mat<T, 4, 4> translation(const nvec<T, 3> v)
		{
			return translation(v[0], v[1], v[2]);
		}

		static mat<T, 4, 4> x_rotation(T x_rad)
		{
			auto m = mat<T, 4, 4>::identity();
			m[1][1] = cosf(x_rad);
			m[1][2] = sinf(x_rad);
			m[2][1] = -sinf(x_rad);
			m[2][2] = cosf(x_rad);
			return m;
		}

		static mat<T, 4, 4> y_rotation(T y_rad)
		{
			auto m = mat<T, 4, 4>::identity();
			m[0][0] = cosf(y_rad);
			m[0][2] = sinf(y_rad);
			m[2][0] = -sinf(y_rad);
			m[2][2] = cosf(y_rad);
			return m;
		}

		static mat<T, 4, 4> z_rotation(T z_rad)
		{
			auto m = mat<T, 4, 4>::identity();
			m[0][0] = cosf(z_rad);
			m[0][1] = sinf(z_rad);
			m[1][0] = -sinf(z_rad);
			m[1][1] = cosf(z_rad);
			return m;
		}

		static inline mat<T, 4, 4> rotation(T x, T y, T z)
		{
			return x_rotation(x) * y_rotation(y) * z_rotation(z);
		}

		static inline mat<T, 4, 4> rotation(nvec<T, 3> v)
		{
			return rotation(v[0], v[1], v[2]);
		}
	};

	template <typename T>
	class MFNS<T, 2, 2>
	{
	public:
		static mat<T, 2, 2> rotation(T radians)
		{
			return mat<T, 2, 2>{
				{cosf(radians), sinf(radians)},
				{-sinf(radians), cosf(radians)}};
		}

		static mat<T, 2, 2> scale(T x, T y)
		{
			return mat<T, 2, 2>{
				{x, 0},
				{0, y}};
		}
	};

	template <typename T>
	using mat4 = mat<T, 4, 4>;
	using m4f = MFNS<double, 4, 4>;
}

#endif