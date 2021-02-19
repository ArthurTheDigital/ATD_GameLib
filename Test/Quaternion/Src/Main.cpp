/**
@file     
@brief    Quaternion test.
@details  License: GPL v3.
@author   ArthurTheDigital (arthurthedigital@gmail.com)
@since    $Id: $
*/

#include <ATD/Core/AutoTest.hpp>
#include <ATD/Core/Debug.hpp>
#include <ATD/Core/Matrix4.hpp>
#include <ATD/Core/Quaternion.hpp>
#include <ATD/Core/Vector3.hpp>
#include <ATD/Core/Vector4.hpp>

#include <stdio.h>


int main(int argc, char** argv)
{
	const double eps = 0.000001;

	{
		const ATD::Vector3D v(0., 1., 0.);

		const double angleFrc = 1.;
		const ATD::Vector3D axis(1., 0., 0.);

		const ATD::Vector3D rTrue = v;

		ATD::AutoTest tester("identical rotation test:");
		ATD::Quaternion rotation = ATD::Quaternion::rotation(angleFrc, axis);
		ATD::Vector3D r = rotation.applyRotation(v);

		tester.finish(ATD::Vector3D(r - rTrue).length() < eps);
	}

	{
		const ATD::Vector3D v(0., 1., 0.);

		const double angleFrc = 0.5;
		const ATD::Vector3D axis(1., 0., 0.);

		const ATD::Vector3D rTrue(0., -1., 0.);

		ATD::AutoTest tester("1/2 circle rotation test:");
		ATD::Quaternion rotation = ATD::Quaternion::rotation(angleFrc, axis);
		ATD::Vector3D r = rotation.applyRotation(v);

		tester.finish(ATD::Vector3D(r - rTrue).length() < eps);
	}

	{
		const ATD::Vector3D v(0., 1., 0.);

		const double angleFrc = 0.25;
		const ATD::Vector3D axis(1., 0., 0.);

		const ATD::Vector3D rTrue(0., 0., -1.);

		ATD::AutoTest tester("1/4 circle rotation test:");
		ATD::Quaternion rotation = ATD::Quaternion::rotation(angleFrc, axis);
		ATD::Vector3D r = rotation.applyRotation(v);

		tester.finish(ATD::Vector3D(r - rTrue).length() < eps);
	}

	{
		const ATD::Vector4F v(0., 1., 0., 1.);

		const double angleFrc = 0.25;
		const ATD::Vector3D axis(1., 0., 0.);

		const ATD::Vector4F rTrue(0., 0., -1., 1.);

		ATD::AutoTest tester("1/4 circle matrix rotation test:");
		ATD::Quaternion rotation = ATD::Quaternion::rotation(angleFrc, axis);
		ATD::Matrix4F rotationMatrix = rotation.getRotationMatrix();
		ATD::Vector4F r = rotationMatrix * v;

		tester.finish(ATD::Vector4F(r - rTrue).length() < eps);
	}

	{
		const ATD::Vector4F v(0., 1., 0., 1.);

		const double angleFrc = 0.25;
		const ATD::Vector3D axis(1., 0., 0.);

		const ATD::Vector4F rTrue(0., 0., 1., 1.);

		ATD::AutoTest tester("1/4 circle matrix inverse rotation test:");
		ATD::Quaternion rotation = ATD::Quaternion::rotation(angleFrc, axis);
		ATD::Matrix4F rotationMatrix = rotation.inverted().getRotationMatrix();
		ATD::Vector4F r = rotationMatrix * v;

		tester.finish(ATD::Vector4F(r - rTrue).length() < eps);
	}

	return 0;
}


