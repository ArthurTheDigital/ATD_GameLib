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
	inline const Vector2D &scale() const
	{ return m_scale; }

	/**
	 * @brief ...
	 * @param scaleSrc - ... */
	void setScale(const Vector2D &scale);

	/**
	 * @brief ...
	 * @return angle of the transform as fraction of circle */
	inline double angleFrc() const
	{ return m_angleFrc; }

	/**
	 * @brief ...
	 * @param angleFrc - ... */
	void setAngleFrc(double angleFrc);

	/**
	 * @brief ...
	 * @return ... */
	inline const Vector2D &offset() const
	{ return m_offset; }

	/**
	 * @brief ...
	 * @param offset */
	void setOffset(const Vector2D &offset);

	/**
	 * @brief Update internally cached matrix if needed. */
	void flushMatrix() const;

	/**
	 * @brief Update internally cached reverse matrix if needed. */
	void flushReverseMatrix() const;

	/* === */

	/**
	 * @brief ...
	 * @param target - ...
	 * @return ... */
	Vector2D apply(const Vector2D &target) const;

	/**
	 * @brief ...
	 * @param target - ...
	 * @return ... */
	Vector2D applyReverse(const Vector2D &target) const;

	/**
	 * @brief Get 3x3 matrix, suitable to transform coords inside GLSL shader.
	 * @return ...
	 *
	 * Matrix dimention should be vector dimention + 1 to apply offset via 
	 * multiplication. Vector to be multiplied should be (x, y, 1.f). */
	inline const Matrix3F &matrix() const
	{ flushMatrix(); return m_matrix; }

	/**
	 * @brief ...
	 * @return ... */
	inline const Matrix3F &reverseMatrix() const
	{ flushReverseMatrix(); return m_reverseMatrix; }

protected:
	/**
	 * @brief ...
	 * @return ... */
	Matrix3F getScaleMatrix() const;

	/**
	 * @brief ...
	 * @return ... */
	Matrix3F getReverseScaleMatrix() const;

	/**
	 * @brief ...
	 * @return ... */
	Matrix3F getRotationMatrix() const;

	/**
	 * @brief ...
	 * @return ... */
	Matrix3F getReverseRotationMatrix() const;

	/**
	 * @brief ...
	 * @return ... */
	Matrix3F getOffsetMatrix() const;

	/**
	 * @brief ...
	 * @return ... */
	Matrix3F getReverseOffsetMatrix() const;

protected:
	/**
	 * @brief Invalidate both cached matrices. */
	virtual void invalidateCache() const;

private:
	Vector2D m_scale;
	double m_angleFrc;
	Vector2D m_offset;

	/* GL matrix is cached. */
	mutable Matrix3F m_matrix;
	mutable bool m_matrixVld;

	/* Reverse GL matrix is cached either. */
	mutable Matrix3F m_reverseMatrix;
	mutable bool m_reverseMatrixVld;
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
	 * @brief ... */
	inline void flushMatrix() const
	{ Transform2D::flushReverseMatrix(); }

	/**
	 * @brief ... */
	inline void flushReverseMatrix() const
	{ Transform2D::flushMatrix(); }

	/**
	 * @brief ...
	 * @return ... */
	inline const Matrix3F &matrix() const
	{ return Transform2D::reverseMatrix(); }

	/**
	 * @brief ...
	 * @return ... */
	inline const Matrix3F &reverseMatrix() const
	{ return Transform2D::matrix(); }
};

} /* namespace ATD */


