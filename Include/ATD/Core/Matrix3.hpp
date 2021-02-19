/**
 * @file      
 * @brief     Matrix3 template and classes.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Vector3.hpp>

#include <array>


namespace ATD {

template<typename T>
class Matrix3
{
public:
	inline Matrix3()
		: m{{
				T(1), T(0), T(0), 
				T(0), T(1), T(0), 
				T(0), T(0), T(1)
				}}
	{}

	inline Matrix3(const Matrix3<T> &other)
		: Matrix3(other.m)
	{}

	inline Matrix3<T> &operator=(const Matrix3<T> &other)
	{
		for (size_t i = 0; i < 9; i++) {
			m[i] = other.m[i];
		}
		return *this;
	}

	inline T *operator[](int i)
	{ return &(m[i * 3]); }

	inline const T *operator[](int i) const
	{ return &(m[i * 3]); }

	inline Matrix3<T> operator*(const Matrix3<T> &mLeft) const
	{
		std::array<Vector3<T>, 3> rRows = rows();
		std::array<Vector3<T>, 3> lCols = mLeft.cols();

		return Matrix3<T>(
				std::array<T, 9>{{
						rRows[0].dot(lCols[0]), rRows[0].dot(lCols[1]), 
						rRows[0].dot(lCols[2]), 

						rRows[1].dot(lCols[0]), rRows[1].dot(lCols[1]), 
						rRows[1].dot(lCols[2]), 

						rRows[2].dot(lCols[0]), rRows[2].dot(lCols[1]), 
						rRows[2].dot(lCols[2])
					}}
				);
	}

	inline Vector3<T> operator*(const Vector3<T> &v) const
	{
		std::array<Vector3<T>, 3> rRows = rows();

		return Vector3<T>(
				rRows[0].dot(v), 
				rRows[1].dot(v), 
				rRows[2].dot(v)
				);
	}

private:
	inline Matrix3(const std::array<T, 9> &n_m)
		: m{{
			n_m[0], n_m[1], n_m[2], 
			n_m[3], n_m[4], n_m[5], 
			n_m[6], n_m[7], n_m[8]
		}}
	{}

	inline std::array<Vector3<T>, 3> rows() const
	{
		return std::array<Vector3<T>, 3>{{
			Vector3<T>((*this)[0][0], (*this)[0][1], (*this)[0][2]), 
			Vector3<T>((*this)[1][0], (*this)[1][1], (*this)[1][2]), 
			Vector3<T>((*this)[2][0], (*this)[2][1], (*this)[2][2])
		}};
	}

	inline std::array<Vector3<T>, 3> cols() const
	{
		return std::array<Vector3<T>, 3>{{
			Vector3<T>((*this)[0][0], (*this)[1][0], (*this)[2][0]), 
			Vector3<T>((*this)[0][1], (*this)[1][1], (*this)[2][1]), 
			Vector3<T>((*this)[0][2], (*this)[1][2], (*this)[2][2])
		}};
	}

	std::array<T, 9> m;
};


/* Typedefs: */

typedef Matrix3<float> Matrix3F;

} /* namespace ATD */


