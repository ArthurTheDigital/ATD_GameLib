/**
 * @file      
 * @brief     Linear 2D transform.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#include <ATD/Core/Transform2D.hpp>

#include <ATD/Core/Printf.hpp>
#include <ATD/Core/Vector3.hpp>

#include <math.h>

#include <stdexcept>


/* ATD::Transform2D auxiliary: */

const double EPS = 0.000001;
const double CIRCLE = 2. * M_PI;


/* ATD::Transform2D: */

ATD::Transform2D::Transform2D(const ATD::Vector2D &scale, 
		double angleFrc, 
		const Vector2D &offset)
	: m_scale(scale)
	, m_angleFrc(angleFrc)
	, m_offset(offset)
	, m_matrix()
	, m_matrixVld(false)
{
	if (scale.x <= 0. + EPS || scale.y <= 0. + EPS) {
		throw std::runtime_error(Printf("Invalid scale (%lf, %lf)", 
					scale.x, scale.y));
	}
}

ATD::Transform2D::Transform2D(const ATD::Transform2D &other)
	: Transform2D(other.m_scale, other.m_angleFrc, other.m_offset)
{}

/* Setters/getters */

ATD::Vector2D ATD::Transform2D::Scale() const
{ return m_scale; }

void ATD::Transform2D::SetScale(const ATD::Vector2D &scale)
{
	if (scale.x <= 0. + EPS || scale.y <= 0. + EPS) {
		throw std::runtime_error(Printf("Invalid scale (%lf, %lf)", 
					scale.x, scale.y));
	}
	m_scale = scale;
	InvalidateCache();
}

double ATD::Transform2D::AngleFrc() const
{ return m_angleFrc; }

void ATD::Transform2D::SetAngleFrc(double angleFrc)
{ m_angleFrc = angleFrc; InvalidateCache(); }

ATD::Vector2D ATD::Transform2D::Offset() const
{ return m_offset; }

void ATD::Transform2D::SetOffset(const ATD::Vector2D &offset)
{ m_offset = offset; InvalidateCache(); }

/* === */

ATD::Vector2D ATD::Transform2D::Apply(const ATD::Vector2D &target) const
{
	ATD::Vector3F result3F = GetMatrixRotation() * 
		(GetMatrixScale() * ATD::Vector3F(target, 1.));
	return ATD::Vector2D(result3F.x, result3F.y) + m_offset;
}

ATD::Vector2D ATD::Transform2D::ApplyReverse(const ATD::Vector2D &target) const
{
	ATD::Vector3F result3F = GetMatrixScaleReverse() * 
		(GetMatrixRotationReverse() * ATD::Vector3F(target - m_offset, 1.));
	return ATD::Vector2D(result3F.x, result3F.y);
}

ATD::Matrix3F ATD::Transform2D::GetMatrix() const
{
	if (!m_matrixVld) {
		m_matrix = 
			GetMatrixOffset() * GetMatrixRotation() * GetMatrixScale();
		m_matrixVld = true;
	}
	return m_matrix;
}

ATD::Matrix3F ATD::Transform2D::GetMatrixReverse() const
{
	return (GetMatrixScaleReverse() * GetMatrixRotationReverse() * 
			GetMatrixOffsetReverse());
}

ATD::Matrix3F ATD::Transform2D::GetMatrixScale() const
{
	ATD::Matrix3F scale;
	scale[0][0] = static_cast<float>(m_scale.x);
	scale[1][1] = static_cast<float>(m_scale.y);
	return scale;
}

ATD::Matrix3F ATD::Transform2D::GetMatrixScaleReverse() const
{
	ATD::Matrix3F scale;
	scale[0][0] = static_cast<float>(1. / m_scale.x);
	scale[1][1] = static_cast<float>(1. / m_scale.y);
	return scale;
}

ATD::Matrix3F ATD::Transform2D::GetMatrixRotation() const
{
	ATD::Matrix3F rotation;
	rotation[0][0] = static_cast<float>(::cos(m_angleFrc * CIRCLE));
	rotation[0][1] = static_cast<float>(-::sin(m_angleFrc * CIRCLE));
	rotation[1][0] = static_cast<float>(::sin(m_angleFrc * CIRCLE));
	rotation[1][1] = static_cast<float>(::cos(m_angleFrc * CIRCLE));
	return rotation;
}

ATD::Matrix3F ATD::Transform2D::GetMatrixRotationReverse() const
{
	ATD::Matrix3F rotation;
	rotation[0][0] = static_cast<float>(::cos(m_angleFrc * CIRCLE * (-1.)));
	rotation[0][1] = static_cast<float>(-::sin(m_angleFrc * CIRCLE * (-1.)));
	rotation[1][0] = static_cast<float>(::sin(m_angleFrc * CIRCLE * (-1.)));
	rotation[1][1] = static_cast<float>(::cos(m_angleFrc * CIRCLE * (-1.)));
	return rotation;
}

ATD::Matrix3F ATD::Transform2D::GetMatrixOffset() const
{
	ATD::Matrix3F offset;
	offset[0][2] = static_cast<float>(m_offset.x);
	offset[1][2] = static_cast<float>(m_offset.y);
	return offset;
}

ATD::Matrix3F ATD::Transform2D::GetMatrixOffsetReverse() const
{
	ATD::Matrix3F offset;
	offset[0][2] = static_cast<float>(m_offset.x * (-1.));
	offset[1][2] = static_cast<float>(m_offset.y * (-1.));
	return offset;
}

void ATD::Transform2D::InvalidateCache() const
{ m_matrixVld = false; }


/* ATD::Projection2D: */

ATD::Projection2D::Projection2D(const ATD::Transform2D &transform)
	: Transform2D(transform)
	, m_matrixProjection()
	, m_matrixProjectionVld(false)
{}

ATD::Matrix3F ATD::Projection2D::GetMatrix() const
{
	if (!m_matrixProjectionVld) {
		m_matrixProjection = Transform2D::GetMatrixReverse();
		m_matrixProjectionVld = true;
	}
	return m_matrixProjection;
}

ATD::Matrix3F ATD::Projection2D::GetMatrixReverse() const
{ return Transform2D::GetMatrix(); }

void ATD::Projection2D::InvalidateCache() const
{ Transform2D::InvalidateCache(); m_matrixProjectionVld = false; }


