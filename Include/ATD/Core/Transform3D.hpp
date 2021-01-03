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

class Transform3D
{
public:

	Transform3D(const Vector3D &scale = Vector3D(1., 1., 1.), 
			const Quaternion &rotation = Quaternion::Rotation(), 
			const Vector3D &offset = Vector3D());

	Transform3D(const Transform3D &other);


	Vector3D Scale() const;

	void SetScale(const Vector3D &scale);

	Quaternion Rotation() const;

	void SetRotation(const Quaternion &rotation);

	Vector3D Offset() const;

	void SetOffset(const Vector3D &offset);


	Vector3D Apply(const Vector3D &target) const;

	Vector3D ApplyReverse(const Vector3D &target) const;

	Matrix4F GetMatrix() const;

	Matrix4F GetMatrixReverse() const;

protected:
	Matrix4F GetMatrixScale() const;

	Matrix4F GetMatrixScaleReverse() const;

	Matrix4F GetMatrixRotation() const;

	Matrix4F GetMatrixRotationReverse() const;

	Matrix4F GetMatrixOffset() const;

	Matrix4F GetMatrixOffsetReverse() const;

protected:
	virtual void InvalidateCache() const;

private:
	Vector3D m_scale;
	Quaternion m_rotation;
	Vector3D m_offset;

	mutable Matrix4F m_matrix;
	mutable bool m_matrixVld;
};

class Projection3D : public Transform3D
{
public:
	typedef std::shared_ptr<Projection3D> Ptr;
	typedef std::shared_ptr<const Projection3D> CPtr;

	static const double DFT_Z_NEAR;
	static const double DFT_Z_FAR;
	static const double DFT_FIELD_OF_VIEW_ANGLE_FRC;


	Projection3D(const Transform3D &transform = Transform3D(), 
			double zNear = DFT_Z_NEAR, 
			double zFar = DFT_Z_FAR, 
			double fieldOfViewAngleFrc = DFT_FIELD_OF_VIEW_ANGLE_FRC);

	Projection3D(const Projection3D &other);

	/* TODO: Setters/getters for zNear, zFar, fieldOfViewAngleFrc */

	/**
	 * @brief ...
	 * @param aspectRatio - screen width / height
	 * @return ... */
	Matrix4F GetMatrix(double aspectRatio = 1.) const;

protected:
	Matrix4F GetMatrixPerspective(double aspectRatio) const;

protected:
	virtual void InvalidateCache() const;

private:
	Matrix4F GetMatrixReverse() const = delete;


	double m_zNear;
	double m_zFar;
	double m_fieldOfViewAngleFrc;

	mutable Matrix4F m_matrixReverse;
	mutable bool m_matrixReverseVld;
};

} /* namespace ATD */


