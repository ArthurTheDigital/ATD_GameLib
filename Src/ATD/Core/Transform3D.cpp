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

static void _checkScale(const ATD::Vector3D &scale)
{
	const double eps = 0.000001;
	if ((scale.x <= 0. + eps) || (scale.y <= 0. + eps) || 
			(scale.z <= 0. + eps)) {
		throw std::runtime_error(ATD::Aux::printf("invalid scale (%lf, %lf, %lf)", 
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
	, m_reverseMatrix()
	, m_reverseMatrixVld(false)
{
	_checkScale(m_scale);
}

ATD::Transform3D::Transform3D(const ATD::Transform3D &other)
	: Transform3D(other.m_scale, other.m_rotation, other.m_offset)
{}

void ATD::Transform3D::setScale(const ATD::Vector3D &scale)
{
	_checkScale(scale);
	m_scale = scale;
	invalidateCache();
}

void ATD::Transform3D::setRotation(const ATD::Quaternion &rotation)
{
	m_rotation = rotation;
	invalidateCache();
}

void ATD::Transform3D::setOffset(const ATD::Vector3D &offset)
{
	m_offset = offset;
	invalidateCache();
}

void ATD::Transform3D::flushMatrix() const
{
	if (!m_matrixVld) {
		m_matrix = (getOffsetMatrix() * getRotationMatrix() * getScaleMatrix());
		m_matrixVld = true;
	}
}

void ATD::Transform3D::flushReverseMatrix() const
{
	if (!m_reverseMatrixVld) {
		m_reverseMatrix = getReverseScaleMatrix() * getReverseRotationMatrix() * 
			getReverseOffsetMatrix();
		m_reverseMatrixVld = true;
	}
}

ATD::Vector3D ATD::Transform3D::apply(const ATD::Vector3D &target) const
{
	ATD::Vector3D result = target;

	/* Scale: */
	result.x *= m_scale.x;
	result.y *= m_scale.y;
	result.z *= m_scale.z;

	/* Rotation: */
	result = Quaternion(m_rotation.inverted() * Quaternion(result) * 
			m_rotation).v;

	/* Offset: */
	result += m_offset;

	return result;
}

ATD::Matrix4F ATD::Transform3D::getScaleMatrix() const
{
	ATD::Matrix4F scale;
	scale[0][0] = m_scale.x;
	scale[1][1] = m_scale.y;
	scale[2][2] = m_scale.z;

	return scale;
}

ATD::Matrix4F ATD::Transform3D::getReverseScaleMatrix() const
{
	ATD::Matrix4F scale;
	scale[0][0] = 1. / m_scale.x;
	scale[1][1] = 1. / m_scale.y;
	scale[2][2] = 1. / m_scale.z;

	return scale;
}

ATD::Matrix4F ATD::Transform3D::getRotationMatrix() const
{ return m_rotation.getRotationMatrix(); }

ATD::Matrix4F ATD::Transform3D::getReverseRotationMatrix() const
{ return m_rotation.inverted().getRotationMatrix(); }

ATD::Matrix4F ATD::Transform3D::getOffsetMatrix() const
{
	ATD::Matrix4F offset;
	offset[0][3] = m_offset.x;
	offset[1][3] = m_offset.y;
	offset[2][3] = m_offset.z;

	return offset;
}

ATD::Matrix4F ATD::Transform3D::getReverseOffsetMatrix() const
{
	ATD::Matrix4F offset;
	offset[0][3] = m_offset.x * (-1.);
	offset[1][3] = m_offset.y * (-1.);
	offset[2][3] = m_offset.z * (-1.);

	return offset;
}

void ATD::Transform3D::invalidateCache() const
{
	m_matrixVld = false;
	m_reverseMatrixVld = false;
}


/* ATD::Projection3D auxiliary: */

static void _checkAspectRatio(double aspectRatio)
{
	const double eps = 0.000001;
	if (aspectRatio <= 0. + eps) {
		throw std::runtime_error(ATD::Aux::printf("invalid aspect ratio %lf", 
					aspectRatio));
	}
}


/* ATD::Projection3D constants: */

const double ATD::Projection3D::DFT_Z_NEAR = 0.1;
const double ATD::Projection3D::DFT_Z_FAR = 100.0;
const double ATD::Projection3D::DFT_FOV_ANGLE_FRC = 0.067;


/* ATD::Projection3D: */

ATD::Projection3D::Projection3D(const ATD::Transform3D &transform, 
		double zNear, 
		double zFar, 
		double fovAngleFrc)
	: Transform3D(transform)
	, m_zNear(zNear)
	, m_zFar(zFar)
	, m_fovAngleFrc(fovAngleFrc)
	, m_aspectRatio(1.)
	, m_resultMatrix()
	, m_resultMatrixVld(false)
{}

ATD::Projection3D::Projection3D(const ATD::Projection3D &other)
	: Projection3D(static_cast<Transform3D>(other), other.m_zNear, 
			other.m_zFar, other.m_fovAngleFrc)
{}

void ATD::Projection3D::setAspectRatio(double aspectRatio) const
{
	_checkAspectRatio(aspectRatio);
	m_aspectRatio = aspectRatio;
	invalidateResultCache();
}

void ATD::Projection3D::flushMatrix() const
{
	if (!m_resultMatrixVld) {
		m_resultMatrix = getPerspectiveMatrix() * Transform3D::reverseMatrix();
		m_resultMatrixVld = true;
	}
}

ATD::Matrix4F ATD::Projection3D::getPerspectiveMatrix() const
{
	const double circle = M_PI * 2.;
	const double halfFovTan = ::tan(m_fovAngleFrc * circle / 2.);
	const double zDiff = m_zFar - m_zNear;
	const double zSum = m_zFar + m_zNear;

	Matrix4F perspective;

	perspective[0][0] = static_cast<float>(1. / (halfFovTan * m_aspectRatio));
	perspective[1][1] = static_cast<float>(1. / halfFovTan);
	perspective[2][2] = static_cast<float>((-1.) * zSum / zDiff);
	perspective[2][3] = static_cast<float>(2. * m_zNear * m_zFar / zDiff);
	perspective[3][2] = 1.f;
	perspective[3][3] = 0.f;

	return perspective;
}

void ATD::Projection3D::invalidateCache() const
{
	Transform3D::invalidateCache();
	invalidateResultCache();
}

void ATD::Projection3D::invalidateResultCache() const
{
	m_resultMatrixVld = false;
}


