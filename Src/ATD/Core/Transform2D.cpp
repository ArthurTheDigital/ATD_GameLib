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

const double _CIRCLE = 2. * M_PI;

static void _checkScale(const ATD::Vector2D &scale)
{
	const double eps = 0.000001;
	if (scale.x <= 0. + eps || scale.y <= 0. + eps) {
		throw std::runtime_error(ATD::Aux::printf("Invalid scale (%lf, %lf)", 
					scale.x, scale.y));
	}
}


/* ATD::Transform2D: */

ATD::Transform2D::Transform2D(const ATD::Vector2D &scale, 
		double angleFrc, 
		const Vector2D &offset)
	: m_scale(scale)
	, m_angleFrc(angleFrc)
	, m_offset(offset)
	, m_matrix()
	, m_matrixVld(false)
	, m_reverseMatrix()
	, m_reverseMatrixVld(false)
{
	_checkScale(scale);
}

ATD::Transform2D::Transform2D(const ATD::Transform2D &other)
	: Transform2D(other.m_scale, other.m_angleFrc, other.m_offset)
{}

/* Setters/getters */

void ATD::Transform2D::setScale(const ATD::Vector2D &scale)
{
	_checkScale(scale);
	m_scale = scale;
	invalidateCache();
}

void ATD::Transform2D::setAngleFrc(double angleFrc)
{
	m_angleFrc = angleFrc;
	invalidateCache();
}

void ATD::Transform2D::setOffset(const ATD::Vector2D &offset)
{
	m_offset = offset;
	invalidateCache();
}

void ATD::Transform2D::flushMatrix() const
{
	if (!m_matrixVld) {
		m_matrix = getOffsetMatrix() * getRotationMatrix() * getScaleMatrix();
		m_matrixVld = true;
	}
}

void ATD::Transform2D::flushReverseMatrix() const
{
	if (!m_reverseMatrixVld) {
		m_reverseMatrix = getReverseScaleMatrix() * getReverseRotationMatrix() * 
			getReverseOffsetMatrix();
		m_reverseMatrixVld = true;
	}
}

/* === */

ATD::Vector2D ATD::Transform2D::apply(const ATD::Vector2D &target) const
{
	ATD::Vector3F result3F = getRotationMatrix() * 
		(getScaleMatrix() * ATD::Vector3F(target, 1.));
	return ATD::Vector2D(result3F.x, result3F.y) + m_offset;
}

ATD::Vector2D ATD::Transform2D::applyReverse(const ATD::Vector2D &target) const
{
	ATD::Vector3F result3F = getReverseScaleMatrix() * 
		(getReverseRotationMatrix() * ATD::Vector3F(target - m_offset, 1.));
	return ATD::Vector2D(result3F.x, result3F.y);
}

ATD::Matrix3F ATD::Transform2D::getScaleMatrix() const
{
	ATD::Matrix3F scale;
	scale[0][0] = static_cast<float>(m_scale.x);
	scale[1][1] = static_cast<float>(m_scale.y);
	return scale;
}

ATD::Matrix3F ATD::Transform2D::getReverseScaleMatrix() const
{
	ATD::Matrix3F scale;
	scale[0][0] = static_cast<float>(1. / m_scale.x);
	scale[1][1] = static_cast<float>(1. / m_scale.y);
	return scale;
}

ATD::Matrix3F ATD::Transform2D::getRotationMatrix() const
{
	ATD::Matrix3F rotation;
	rotation[0][0] = static_cast<float>(::cos(m_angleFrc * _CIRCLE));
	rotation[0][1] = static_cast<float>(-::sin(m_angleFrc * _CIRCLE));
	rotation[1][0] = static_cast<float>(::sin(m_angleFrc * _CIRCLE));
	rotation[1][1] = static_cast<float>(::cos(m_angleFrc * _CIRCLE));
	return rotation;
}

ATD::Matrix3F ATD::Transform2D::getReverseRotationMatrix() const
{
	ATD::Matrix3F rotation;
	rotation[0][0] = static_cast<float>(::cos(m_angleFrc * _CIRCLE * (-1.)));
	rotation[0][1] = static_cast<float>(-::sin(m_angleFrc * _CIRCLE * (-1.)));
	rotation[1][0] = static_cast<float>(::sin(m_angleFrc * _CIRCLE * (-1.)));
	rotation[1][1] = static_cast<float>(::cos(m_angleFrc * _CIRCLE * (-1.)));
	return rotation;
}

ATD::Matrix3F ATD::Transform2D::getOffsetMatrix() const
{
	ATD::Matrix3F offset;
	offset[0][2] = static_cast<float>(m_offset.x);
	offset[1][2] = static_cast<float>(m_offset.y);
	return offset;
}

ATD::Matrix3F ATD::Transform2D::getReverseOffsetMatrix() const
{
	ATD::Matrix3F offset;
	offset[0][2] = static_cast<float>(m_offset.x * (-1.));
	offset[1][2] = static_cast<float>(m_offset.y * (-1.));
	return offset;
}

void ATD::Transform2D::invalidateCache() const
{
	m_matrixVld = false;
	m_reverseMatrixVld = false;
}


/* ATD::Projection2D: */

ATD::Projection2D::Projection2D(const ATD::Transform2D &transform)
	: Transform2D(transform)
{}


