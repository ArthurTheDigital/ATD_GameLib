/**
 * @file      
 * @brief     Linear 2D transform.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Matrix3.hpp>
#include <ATD/Core/Vector2.hpp>


namespace ATD {

/**
 * @brief Linear 2D transform
 * @class ... */
class Transform2D
{
public:
	/**
	 * @brief ...
	 * @param scale    - ...
	 * @param angleFrc - ...
	 * @param offset   - offset in pixels */
	Transform2D(const Vector2D &scale = Vector2D(1., 1.), 
			double angleFrc = 0., 
			const Vector2D &offset = Vector2D(0., 0.));

	/**
	 * @brief ...
	 * @param other - ... */
	Transform2D(const Transform2D &other);

	/* Setters/getters: */

	/**
	 * @brief ...
	 * @return ... */
	Vector2D Scale() const;

	/**
	 * @brief ...
	 * @param scaleSrc - ... */
	void SetScale(const Vector2D &scale);

	/**
	 * @brief ...
	 * @return angle of the transform as fraction of circle */
	double AngleFrc() const;

	/**
	 * @brief ...
	 * @param angleFrc - ... */
	void SetAngleFrc(double angleFrc);

	/**
	 * @brief ...
	 * @return ... */
	Vector2D Offset() const;

	/**
	 * @brief ...
	 * @param offset */
	void SetOffset(const Vector2D &offset);

	/* === */

	/**
	 * @brief ...
	 * @param target - ...
	 * @return ... */
	Vector2D Apply(const Vector2D &target) const;

	/**
	 * @brief ...
	 * @param target - ...
	 * @return ... */
	Vector2D ApplyReverse(const Vector2D &target) const;

	/**
	 * @brief Get 3x3 matrix, suitable to transform coords inside GLSL shader.
	 * @return ...
	 *
	 * Matrix dimention should be vector dimention + 1 to apply offset via 
	 * multiplication. Vector to be multiplied should be (x, y, 1.f). */
	Matrix3F GetMatrix() const;

	/**
	 * @brief ...
	 * @return ... */
	Matrix3F GetMatrixReverse() const;

protected:
	/**
	 * @brief ...
	 * @return ... */
	Matrix3F GetMatrixScale() const;

	/**
	 * @brief ...
	 * @return ... */
	Matrix3F GetMatrixScaleReverse() const;

	/**
	 * @brief ...
	 * @return ... */
	Matrix3F GetMatrixRotation() const;

	/**
	 * @brief ...
	 * @return ... */
	Matrix3F GetMatrixRotationReverse() const;

	/**
	 * @brief ...
	 * @return ... */
	Matrix3F GetMatrixOffset() const;

	/**
	 * @brief ...
	 * @return ... */
	Matrix3F GetMatrixOffsetReverse() const;

protected:
	virtual void InvalidateCache() const;

private:
	Vector2D m_scale;
	double m_angleFrc;
	Vector2D m_offset;

	/* GL matrix is cached. */
	mutable Matrix3F m_matrix;
	mutable bool m_matrixVld;
};

/**
 * @brief ... */
class Projection2D : public Transform2D
{
public:
	/**
	 * @brief ...
	 * @param transform - ... */
	Projection2D(const Transform2D &transform = Transform2D());

	/**
	 * @brief ...
	 * @return ... */
	Matrix3F GetMatrix() const;

	/**
	 * @brief ...
	 * @return ... */
	Matrix3F GetMatrixReverse() const;

protected:
	virtual void InvalidateCache() const;

private:
	/* GL projection (reverse) matrix is cached. */
	mutable Matrix3F m_matrixProjection;
	mutable bool m_matrixProjectionVld;
};

} /* namespace ATD */


