/**
 * @file      
 * @brief     Quaternion class.
 * @detail    ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */ 

#include <ATD/Core/Quaternion.hpp>

#include <math.h>

#include <stdexcept>


/* ATD::Quaternion static funcs auxiliary: */

static void CheckRotationQuaternion(const ATD::Quaternion &rotation)
{
	const double eps = 0.000001;
	double normSquare = rotation.s * rotation.s + rotation.v.LengthSquare();
	if (normSquare < 1. - eps || normSquare > 1. + eps) {
		throw std::runtime_error("not a rotation quaternion");
	}
}


/* ATD::Quaternion static funcs: */

ATD::Quaternion ATD::Quaternion::Rotation(double angleFrc, 
		const ATD::Vector3D &axis)
{
	const double circle = M_PI * 2.;
	return Quaternion(::cos(angleFrc * circle * 0.5), 
			axis.Normalized() * ::sin(angleFrc * circle * 0.5));
}

ATD::Quaternion ATD::Quaternion::Rotation()
{ return Rotation(0., Vector3D(1., 0., 0.)); }


/* ATD::Quaternion: */

ATD::Quaternion::Quaternion(double n_s, const ATD::Vector3D &n_v)
	: s(n_s)
	, v(n_v)
{}

ATD::Quaternion::Quaternion(const ATD::Vector3D &n_v)
	: Quaternion(0., n_v)
{}

ATD::Quaternion::Quaternion(const ATD::Quaternion &other)
	: Quaternion(other.s, other.v)
{}

ATD::Quaternion ATD::Quaternion::Inverted() const
{
	double normSquare = s * s + v.LengthSquare();
	if (normSquare == 0.) {
		/* FIXME: Is "Division by zero" better? */
		throw std::runtime_error("Zero quaternion inversion");
	}
	return Quaternion(s, v * (-1.)) / normSquare;
}

ATD::Quaternion ATD::Quaternion::operator*(const ATD::Quaternion &other) const
{
	Quaternion result(*this);
	result *= other;
	return result;
}

ATD::Quaternion &ATD::Quaternion::operator*=(const ATD::Quaternion &other)
{
	const double &os = other.s;
	const Vector3D &ov = other.v;

	/* Complex quaternion magic here:
	 *
	 * Q = s + vx * i + vy * j + vz *k, where
	 * i, j, k - imaginary units:
	 * i * i == j * j == k * k == -1 - like complex numbers,
	 * but with anticommutative cross-multiplication:
	 * i * j == k; j * i == -k;
	 * j * k == i; k * j == -i;
	 * k * i == j; i * k == -j; */
	double n_s = s * os - v.x * ov.x - v.y * ov.y - v.z * ov.z;
	double n_vx = s * ov.x + v.x * os + v.y * ov.z - v.z * ov.y;
	double n_vy = s * ov.y + v.y * os - v.x * ov.z + v.z * ov.x;
	double n_vz = s * ov.z + v.z * os + v.x * ov.y - v.y * ov.x;

	s = n_s;
	v = Vector3D(n_vx, n_vy, n_vz);
	return *this;
}

ATD::Vector3D ATD::Quaternion::ApplyRotation(const ATD::Vector3D &target) const
{
	CheckRotationQuaternion(*this);
	return Quaternion(Inverted() * Quaternion(target) * (*this)).v;
}

ATD::Matrix4F ATD::Quaternion::GetMatrixRotation() const
{
	CheckRotationQuaternion(*this);

	const Quaternion &r = *this;
	const Quaternion rInv = Inverted();

	Vector3D uX = Quaternion(rInv * Quaternion(Vector3D(1., 0., 0.)) * r).v;
	Vector3D uY = Quaternion(rInv * Quaternion(Vector3D(0., 1., 0.)) * r).v;
	Vector3D uZ = Quaternion(rInv * Quaternion(Vector3D(0., 0., 1.)) * r).v;

	Matrix4F rotation;
	rotation[0][0] = uX.x; rotation[0][1] = uY.x; rotation[0][2] = uZ.x;
	rotation[1][0] = uX.y; rotation[1][1] = uY.y; rotation[1][2] = uZ.y;
	rotation[2][0] = uX.z; rotation[2][1] = uY.z; rotation[2][2] = uZ.z;
	
	return rotation;
}

ATD::Quaternion ATD::Quaternion::operator+(const ATD::Quaternion &other) const
{ return Quaternion(s + other.s, v + other.v); }

ATD::Quaternion ATD::Quaternion::operator-(const ATD::Quaternion &other) const
{ return Quaternion(s - other.s, v - other.v); }

ATD::Quaternion ATD::Quaternion::operator*(double a) const
{ return Quaternion(s * a, v * a); }

ATD::Quaternion ATD::Quaternion::operator/(double a) const
{
	if (a == 0.) { throw std::runtime_error("Division by zero"); }
	return Quaternion(s / a, v / a);
}

ATD::Quaternion &ATD::Quaternion::operator+=(const ATD::Quaternion &other)
{ s += other.s; v += other.v; return *this; }

ATD::Quaternion &ATD::Quaternion::operator-=(const ATD::Quaternion &other)
{ s -= other.s; v -= other.v; return *this; }

ATD::Quaternion &ATD::Quaternion::operator*=(double a)
{ s *= a; v *= a; return *this; }

ATD::Quaternion &ATD::Quaternion::operator/=(double a)
{
	if (a == 0.) { throw std::runtime_error("Division by zero"); }
	s /= a;
	v /= a;
	return *this;
}


