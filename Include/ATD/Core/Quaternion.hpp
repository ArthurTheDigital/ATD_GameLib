/**
 * @file      
 * @brief     Quaternion class.
 * @detail    ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */ 

#pragma once

#include <ATD/Core/Matrix4.hpp>
#include <ATD/Core/Vector3.hpp>


namespace ATD {

/**
 * @brief Quaternion class */
class Quaternion
{
public:
	/**
	 * @brief Shortcut for creating rotation quaternion.
	 * @param angleFrc - ...
	 * @param axis - ...
	 * @return ...
	 *
	 * Rotation quaternion shall be applied as following:
	 * Vector3D vectorRotated = Quaternion(r.Inverted() * 
	 *     Quaternion(vector) * 
	 *     r).v; */
	static Quaternion Rotation(double angleFrc, const Vector3D &axis);

	/**
	 * @brief ...
	 * @return ...
	 *
	 * Default rotation to keep everything simple. */
	static Quaternion Rotation();

	/**
	 * @brief ...
	 * @param n_s - scalar part of quaternion
	 * @param n_v - vector part of quaternion */
	Quaternion(double n_s = 0., const Vector3D &n_v = Vector3D());

	/**
	 * @brief ...
	 * @param n_v - vector part of quaternion */
	Quaternion(const Vector3D &n_v);

	/**
	 * @brief ...
	 * @param other - ... */
	Quaternion(const Quaternion &other);

	/* Quaternion math: */

	/**
	 * @brief Get the inverse quaternion.
	 * @return ... */
	Quaternion Inverted() const;

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	Quaternion operator*(const Quaternion &other) const;

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	Quaternion &operator*=(const Quaternion &other);

	/* Quaternion rotation functions: */

	/**
	 * @brief ...
	 * @param target - ...
	 * @throw if quaternion is not a unit
	 * @return ... */
	Vector3D ApplyRotation(const Vector3D &target) const;

	/**
	 * @brief ...
	 * @throw if quaternion is not a unit
	 * @return ... */
	Matrix4F GetMatrixRotation() const;

	/* Constant arithmetics: */

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	Quaternion operator+(const Quaternion &other) const;

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	Quaternion operator-(const Quaternion &other) const;

	/**
	 * @brief ...
	 * @param a - ...
	 * @return ... */
	Quaternion operator*(double a) const;

	/**
	 * @brief ...
	 * @param a - ...
	 * @return ... */
	Quaternion operator/(double a) const;

	/* Modifying arithmetics: */

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	Quaternion &operator+=(const Quaternion &other);

	/**
	 * @brief ...
	 * @param other - ...
	 * @return ... */
	Quaternion &operator-=(const Quaternion &other);

	/**
	 * @brief ...
	 * @param a - ...
	 * @return ... */
	Quaternion &operator*=(double a);

	/**
	 * @brief ...
	 * @param a - ...
	 * @return ... */
	Quaternion &operator/=(double a);


	double s;
	Vector3D v;
};

} /* namespace ATD */


