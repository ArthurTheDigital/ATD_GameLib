/**
 * @file      
 * @brief     Linear 3D transform.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <ATD/Core/Matrix4.hpp>
#include <ATD/Core/Quaternion.hpp>
#include <ATD/Core/Vector3.hpp>

#include <memory>


namespace ATD {

/**
 * @brief Linear 3D transform
 * @class ... */
class Transform3D
{
public:

	/**
	 * @brief ...
	 * @param scale    - ...
	 * @param rotation - ...
	 * @param offset   - ... */
	Transform3D(const Vector3D &scale = Vector3D(1., 1., 1.), 
			const Quaternion &rotation = Quaternion::rotation(), 
			const Vector3D &offset = Vector3D());

	/**
	 * @brief ...
	 * @param other - ... */
	Transform3D(const Transform3D &other);

	/**
	 * @brief ...
	 * @return ... */
	inline const Vector3D &scale() const
	{ return m_scale; }

	/**
	 * @brief ...
	 * @param scale - ... */
	void setScale(const Vector3D &scale);

	/**
	 * @brief ...
	 * @return ... */
	inline const Quaternion &rotation() const
	{ return m_rotation; }

	/**
	 * @brief ...
	 * @param rotation - ... */
	void setRotation(const Quaternion &rotation);

	/**
	 * @brief ...
	 * @return ... */
	inline const Vector3D &offset() const
	{ return m_offset; }

	/**
	 * @brief ...
	 * @param offset - ... */
	void setOffset(const Vector3D &offset);

	/**
	 * @brief Update internally cached matrix if needed. */
	void flushMatrix() const;

	/**
	 * @brief Update internally cached reverse matrix if needed. */
	void flushReverseMatrix() const;

	/**
	 * @brief ...
	 * @param target - ...
	 * @return ... */
	Vector3D apply(const Vector3D &target) const;

	/**
	 * @brief ...
	 * @param target - ...
	 * @return ... */
	Vector3D applyReverse(const Vector3D &target) const;

	/**
	 * @brief ...
	 * @return ... */
	inline const Matrix4F &matrix() const
	{ flushMatrix(); return m_matrix; }

	/**
	 * @brief ...
	 * @return ... */
	inline const Matrix4F &reverseMatrix() const
	{ flushReverseMatrix(); return m_reverseMatrix; }

protected:
	/**
	 * @brief ...
	 * @return ... */
	Matrix4F getScaleMatrix() const;

	/**
	 * @brief ...
	 * @return ... */
	Matrix4F getReverseScaleMatrix() const;

	/**
	 * @brief ...
	 * @return ... */
	Matrix4F getRotationMatrix() const;

	/**
	 * @brief ...
	 * @return ... */
	Matrix4F getReverseRotationMatrix() const;

	/**
	 * @brief ...
	 * @return ... */
	Matrix4F getOffsetMatrix() const;

	/**
	 * @brief ...
	 * @return ... */
	Matrix4F getReverseOffsetMatrix() const;

protected:
	/**
	 * @brief Invalidate both cached matrices. */
	virtual void invalidateCache() const;

private:
	Vector3D m_scale;
	Quaternion m_rotation;
	Vector3D m_offset;

	mutable Matrix4F m_matrix;
	mutable bool m_matrixVld;

	mutable Matrix4F m_reverseMatrix;
	mutable bool m_reverseMatrixVld;
};

/**
 * @brief ... */
class Projection3D : public Transform3D
{
public:
	typedef std::shared_ptr<Projection3D> Ptr;
	typedef std::shared_ptr<const Projection3D> CPtr;

	static const double DFT_Z_NEAR;
	static const double DFT_Z_FAR;
	static const double DFT_FOV_ANGLE_FRC;


	/**
	 * @brief ...
	 * @param transform   - ...
	 * @param zNear       - distance to the near 'wall'
	 * @param zFar        - distance to the far 'wall'
	 * @param fovAngleFrc - field of view angle */
	Projection3D(const Transform3D &transform = Transform3D(), 
			double zNear = DFT_Z_NEAR, 
			double zFar = DFT_Z_FAR, 
			double fovAngleFrc = DFT_FOV_ANGLE_FRC);

	/**
	 * @brief ...
	 * @param other - ... */
	Projection3D(const Projection3D &other);

	/* For a while no setters/getters for zNear, zFar, fovAngleFrc. */

	/**
	 * @brief ...
	 * @param aspectRatio - screen width / height */
	void setAspectRatio(double aspectRatio) const;

	/**
	 * @brief ... */
	void flushMatrix() const;

	/**
	 * @brief ...
	 * @return ... */
	inline const Matrix4F &matrix() const
	{ flushMatrix(); return m_resultMatrix; }

	// TODO: Get rid of
	// inline const Matrix4F &matrix(double aspectRatio) const
	// { setAspectRatio(aspectRatio); flushMatrix(); return m_resultMatrix; }

protected:
	/**
	 * @brief ...
	 * @return ... */
	Matrix4F getPerspectiveMatrix() const;

protected:
	/**
	 * @brief ... */
	virtual void invalidateCache() const;

	/**
	 * @brief ... */
	void invalidateResultCache() const;

private:
	Matrix4F reverseMatrix() const = delete;
	void flushReverseMatrix() const = delete;


	double m_zNear;
	double m_zFar;
	double m_fovAngleFrc;

	mutable double m_aspectRatio;
	mutable Matrix4F m_resultMatrix;
	mutable bool m_resultMatrixVld;
};

} /* namespace ATD */


