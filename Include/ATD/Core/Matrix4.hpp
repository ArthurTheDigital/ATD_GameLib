

#pragma once

#include <ATD/Core/Vector4.hpp>

#include <array>


namespace ATD {

template<typename T>
class Matrix4
{
public:
	inline Matrix4()
		: m{{
				T(1), T(0), T(0), T(0), 
				T(0), T(1), T(0), T(0), 
				T(0), T(0), T(1), T(0), 
				T(0), T(0), T(0), T(1)
				}}
	{}

	inline Matrix4(const Matrix4<T> &other)
		: Matrix4(other.m)
	{}

	inline Matrix4<T> &operator=(const Matrix4<T> &other)
	{
		for (size_t i = 0; i < 16; i++) {
			m[i] = other.m[i];
		}
		return *this;
	}

	inline T *operator[](int i)
	{ return &(m[i * 4]); }

	inline const T *operator[](int i) const
	{ return &(m[i * 4]); }

	inline Matrix4<T> operator*(const Matrix4<T> &mLeft) const
	{
		std::array<Vector4<T>, 4> rRows = GetRows();
		std::array<Vector4<T>, 4> lCols = mLeft.GetCols();

		return Matrix4<T>(
				std::array<T, 16>{{
						rRows[0].Dot(lCols[0]), rRows[0].Dot(lCols[1]), 
						rRows[0].Dot(lCols[2]), rRows[0].Dot(lCols[3]), 

						rRows[1].Dot(lCols[0]), rRows[1].Dot(lCols[1]), 
						rRows[1].Dot(lCols[2]), rRows[1].Dot(lCols[3]), 

						rRows[2].Dot(lCols[0]), rRows[2].Dot(lCols[1]), 
						rRows[2].Dot(lCols[2]), rRows[2].Dot(lCols[3]), 

						rRows[3].Dot(lCols[0]), rRows[3].Dot(lCols[1]), 
						rRows[3].Dot(lCols[2]), rRows[3].Dot(lCols[3])
					}}
				);
	}

	inline Vector4<T> operator*(const Vector4<T> &v) const
	{
		std::array<Vector4<T>, 4> rRows = GetRows();

		return Vector4<T>(
				rRows[0].Dot(v), 
				rRows[1].Dot(v), 
				rRows[2].Dot(v), 
				rRows[3].Dot(v)
				);
	}

private:
	inline Matrix4(const std::array<T, 16> &n_m)
		: m{{
			n_m[ 0], n_m[ 1], n_m[ 2], n_m[ 3], 
			n_m[ 4], n_m[ 5], n_m[ 6], n_m[ 7], 
			n_m[ 8], n_m[ 9], n_m[10], n_m[11], 
			n_m[12], n_m[13], n_m[14], n_m[15]
		}}
	{}

	inline std::array<Vector4<T>, 4> GetRows() const
	{
		return std::array<Vector4<T>, 4>{{
			Vector4<T>((*this)[0][0], (*this)[0][1], (*this)[0][2], 
					(*this)[0][3]), 
			Vector4<T>((*this)[1][0], (*this)[1][1], (*this)[1][2], 
					(*this)[1][3]), 
			Vector4<T>((*this)[2][0], (*this)[2][1], (*this)[2][2], 
					(*this)[2][3]), 
			Vector4<T>((*this)[3][0], (*this)[3][1], (*this)[3][2], 
					(*this)[3][3])
		}};
	}

	inline std::array<Vector4<T>, 4> GetCols() const
	{
		return std::array<Vector4<T>, 4>{{
			Vector4<T>((*this)[0][0], (*this)[1][0], (*this)[2][0], 
					(*this)[3][0]), 
			Vector4<T>((*this)[0][1], (*this)[1][1], (*this)[2][1], 
					(*this)[3][1]), 
			Vector4<T>((*this)[0][2], (*this)[1][2], (*this)[2][2], 
					(*this)[3][2]), 
			Vector4<T>((*this)[0][3], (*this)[1][3], (*this)[2][3], 
					(*this)[3][3])
		}};
	}


	std::array<T, 16> m;
};

/* Typedefs: */

typedef Matrix4<float> Matrix4F;

} /* namespace ATD */


