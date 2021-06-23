#ifndef MATH_H_
#define MATH_H_ 1

#include <initializer_list>
#include <stddef.h>
#include <iostream>

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

		const inline size_t size() const
		{
			return LEN;
		}

		FULL_NUMERIC(nvec)

		T operator[](const size_t index)
		{
			return vec[index];
		}

		const T operator[](const size_t index) const
		{
			return vec[index];
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
	lvec##NUM_ELEMS &operator=(nvec<T, NUM_ELEMS> vec) \
	{                                                  \
		for (size_t i = 0; i < this->size(); i++)      \
			this->vec[i] = vec[i];                     \
		return *this;                                  \
	}                                                  \
	lvec##NUM_ELEMS &operator=(lvec##NUM_ELEMS vec)    \
	{                                                  \
		for (size_t i = 0; i < this->size(); i++)      \
			this->vec[i] = vec[i];                     \
		return *this;                                  \
	}

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
		T &z = this->vec[2];

		lv2<T> zz() { return lv2<T>{z, z}; }
		lv3<T> zzz() { return lv3<T>{z, z, z}; }
		lv4<T> zzzz() { return lv4<T>{z, z, z, z}; }
		lv3<T> xyz() { return lv3<T>{this->x, this->y, z}; }
		lv3<T> zyx() { return lv3<T>{z, this->y, this->x}; }

		LVEC_OP_EQ(3)
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
		LVEC_OP_EQ(NUM_ELEMS)                                            \
	};

	NEW_LVEC(4, 3, w)

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

		size_t width() const
		{
			return W;
		}

		size_t height() const
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

		T *operator[](const size_t index)
		{
			return m[index];
		}

		const T *operator[](const size_t index) const
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

		friend mat operator*(const mat &lhs, const T &rhs)
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

		friend nvec<T, H> operator*(const mat<T, W, H> &lhs, const nvec<T, W> &rhs)
		{
			nvec<T, H> ret;
			for (size_t r = 0; r < H; r++)
				for (size_t c = 0; c < W; c++)
					ret.vec[r] += rhs[c] * lhs[r][c];
			return ret;
		}

		static mat<T, 4, 4> projection(T fovDeg, T aspectRatio, T near, T far)
		{
			T fovRad = (T)1 / tanf((fovDeg * (T)((T)377 / (T)120) / (T)180));
			T zero = (T)0;
			mat<T, 4, 4> ret{{fovRad * aspectRatio, zero, zero, zero},
							 {zero, fovRad, zero, zero},
							 {zero, zero, far / (far - near), (T)1},
							 {zero, zero, (-far * near) / (far - near), zero}};
			return ret;
		}

		static mat identity()
		{
			mat ret;
			for (size_t i = 0; i < (W > H ? H : W); i++)
				ret[i][i] = (T)1;
			return ret;
		}

		static mat<T, 4, 4> lookAt(nvec<T, 3> pos, nvec<T, 3> target, nvec<T, 3> up = nvec<T, 3>{0, 1, 0})
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

		OSTREAM_OP(mat)
	};

	template<typename T>
	using mat4 = mat<T, 4, 4>;

}

#endif