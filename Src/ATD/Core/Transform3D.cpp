/**
 * @file      
 * @brief     Linear 3D transform.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Core/Transform3D.hpp>

#include <ATD/Core/Printf.hpp>

#include <math.h>

#include <stdexcept>


/* ATD::Transform3D auxiliary: */

static void CheckScale(const ATD::Vector3D &scale)
{
	const double eps = 0.000001;
	if ((scale.x <= 0. + eps) || (scale.y <= 0. + eps) || 
			(scale.z <= 0. + eps)) {
		throw std::runtime_error(ATD::Printf("invalid scale (%lf, %lf, %lf)", 
					scale.x, scale.y, scale.z));
	}
}


/* ATD::Transform3D: */

ATD::Transform3D::Transform3D(const ATD::Vector3D &scale, 
		const ATD::Quaternion &rotation, 
		const ATD::Vector3D &offset)
	: m_scale(scale)
	, m_rotation(rotation)
	, m_offset(offset)
	, m_matrix()
	, m_matrixVld(false)
{
	CheckScale(m_scale);
}

ATD::Transform3D::Transform3D(const ATD::Transform3D &other)
	: Transform3D(other.m_scale, other.m_rotation, other.m_offset)
{}

ATD::Vector3D ATD::Transform3D::Scale() const
{ return m_scale; }

void ATD::Transform3D::SetScale(const ATD::Vector3D &scale)
{
	CheckScale(scale);
	m_scale = scale;
	InvalidateCache();
}

ATD::Quaternion ATD::Transform3D::Rotation() const
{ return m_rotation; }

void ATD::Transform3D::SetRotation(const ATD::Quaternion &rotation)
{ m_rotation = rotation; InvalidateCache(); }

ATD::Vector3D ATD::Transform3D::Offset() const
{ return m_offset; }

void ATD::Transform3D::SetOffset(const ATD::Vector3D &offset)
{ m_offset = offset; InvalidateCache(); }

ATD::Vector3D ATD::Transform3D::Apply(const ATD::Vector3D &target) const
{
	ATD::Vector3D result = target;

	/* Scale: */
	result.x *= m_scale.x;
	result.y *= m_scale.y;
	result.z *= m_scale.z;

	/* Rotation: */
	result = Quaternion(m_rotation.Inverted() * Quaternion(result) * 
			m_rotation).v;

	/* Offset: */
	result += m_offset;

	return result;
}

ATD::Matrix4F ATD::Transform3D::GetMatrix() const
{
	if (!m_matrixVld) {
		m_matrix = (GetMatrixOffset() * GetMatrixRotation() * 
				GetMatrixScale());

		m_matrixVld = true;
	}

	return m_matrix;
}

ATD::Matrix4F ATD::Transform3D::GetMatrixReverse() const
{
	return (GetMatrixScaleReverse() * GetMatrixRotationReverse() * 
			GetMatrixOffsetReverse());
}

ATD::Matrix4F ATD::Transform3D::GetMatrixScale() const
{
	ATD::Matrix4F scale;
	scale[0][0] = m_scale.x;
	scale[1][1] = m_scale.y;
	scale[2][2] = m_scale.z;

	return scale;
}

ATD::Matrix4F ATD::Transform3D::GetMatrixScaleReverse() const
{
	ATD::Matrix4F scale;
	scale[0][0] = 1. / m_scale.x;
	scale[1][1] = 1. / m_scale.y;
	scale[2][2] = 1. / m_scale.z;

	return scale;
}

ATD::Matrix4F ATD::Transform3D::GetMatrixRotation() const
{ return m_rotation.GetMatrixRotation(); }

ATD::Matrix4F ATD::Transform3D::GetMatrixRotationReverse() const
{ return m_rotation.Inverted().GetMatrixRotation(); }

ATD::Matrix4F ATD::Transform3D::GetMatrixOffset() const
{
	ATD::Matrix4F offset;
	offset[0][3] = m_offset.x;
	offset[1][3] = m_offset.y;
	offset[2][3] = m_offset.z;

	return offset;
}

ATD::Matrix4F ATD::Transform3D::GetMatrixOffsetReverse() const
{
	ATD::Matrix4F offset;
	offset[0][3] = m_offset.x * (-1.);
	offset[1][3] = m_offset.y * (-1.);
	offset[2][3] = m_offset.z * (-1.);

	return offset;
}

void ATD::Transform3D::InvalidateCache() const
{ m_matrixVld = false; }

/* ATD::Projection3D constants: */

const double ATD::Projection3D::DFT_Z_NEAR = 0.1;
const double ATD::Projection3D::DFT_Z_FAR = 100.0;
const double ATD::Projection3D::DFT_FIELD_OF_VIEW_ANGLE_FRC = 0.067;


/* ATD::Projection3D: */

ATD::Projection3D::Projection3D(const ATD::Transform3D &transform, 
		double zNear, 
		double zFar, 
		double fieldOfViewAngleFrc)
	: Transform3D(transform)
	, m_zNear(zNear)
	, m_zFar(zFar)
	, m_fieldOfViewAngleFrc(fieldOfViewAngleFrc)
	, m_matrixReverse()
	, m_matrixReverseVld(false)
{}

ATD::Projection3D::Projection3D(const ATD::Projection3D &other)
	: Projection3D(static_cast<Transform3D>(other), other.m_zNear, 
			other.m_zFar, other.m_fieldOfViewAngleFrc)
{}

ATD::Matrix4F ATD::Projection3D::GetMatrix(double aspectRatio) const
{
	if (!m_matrixReverseVld) {
		m_matrixReverse = Transform3D::GetMatrixReverse();
		m_matrixReverseVld = true;
	}

	return GetMatrixPerspective(aspectRatio) * m_matrixReverse;
}

ATD::Matrix4F ATD::Projection3D::GetMatrixPerspective(double aspectRatio) const
{
	const double circle = M_PI * 2.;
	const double halfFovTan = ::tan(m_fieldOfViewAngleFrc * circle / 2.);
	const double zDiff = m_zFar - m_zNear;
	const double zSum = m_zFar + m_zNear;

	Matrix4F perspective;

	perspective[0][0] = static_cast<float>(1. / (halfFovTan * aspectRatio));
	perspective[1][1] = static_cast<float>(1. / halfFovTan);
	perspective[2][2] = static_cast<float>((-1.) * zSum / zDiff);
	perspective[2][3] = static_cast<float>(2. * m_zNear * m_zFar / zDiff);
	perspective[3][2] = 1.f;
	perspective[3][3] = 0.f;

	return perspective;
}

void ATD::Projection3D::InvalidateCache() const
{ Transform3D::InvalidateCache(); m_matrixReverseVld = false; }


