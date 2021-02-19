
#include <ATD/Core/MinMax.hpp>
#include <ATD/Core/Transform3D.hpp>
#include <ATD/Core/Quaternion.hpp>

#include <math.h>


class Camera
{
public:
	Camera(const ATD::Vector3D &position = ATD::Vector3D());

	void rotateAround(const ATD::Vector2D &deltaFrc, 
			const ATD::Vector3D &center);

	void changeDistance(const double delta, 
			const ATD::Vector3D &center);

	ATD::Projection3D getProjection() const;

	ATD::Vector3D getXAxis() const;

	ATD::Vector3D getYAxis() const;

private:
	ATD::Vector3D m_position;
	ATD::Vector2D m_angleFrc;

	mutable ATD::Projection3D m_projection;
	mutable bool m_projectionVld;
};


/* auxiliary: */

ATD::Quaternion angleXyToQuaternion(const ATD::Vector2D &angleXyFrc)
{
	ATD::Quaternion rotationX = 
		ATD::Quaternion::rotation(angleXyFrc.x, ATD::Vector3D(1., 0., 0.));

	ATD::Quaternion rotationY = 
		ATD::Quaternion::rotation(angleXyFrc.y, ATD::Vector3D(0., 1., 0.));

	return rotationX * rotationY;
}


/* === */

Camera::Camera(const ATD::Vector3D &position)
	: m_position(position)
	, m_angleFrc()
	, m_projection()
	, m_projectionVld(false)
{}

void Camera::rotateAround(const ATD::Vector2D &deltaFrc, 
		const ATD::Vector3D &center)
{
	double newAngleFrcX = ATD::clamp<double>(m_angleFrc.x + deltaFrc.x, 
			-0.24, 0.24);

	double newAngleFrcY = m_angleFrc.y + deltaFrc.y;
	newAngleFrcY -= ::floor(newAngleFrcY);

	ATD::Vector2D newAngleFrc = ATD::Vector2D(newAngleFrcX, newAngleFrcY);

	ATD::Quaternion oldRotation = angleXyToQuaternion(m_angleFrc);
	ATD::Quaternion newRotation = angleXyToQuaternion(newAngleFrc);

	ATD::Vector3D oldPositionRelative = m_position - center;
	ATD::Vector3D newPositionRelative = 
		newRotation.applyRotation(oldRotation.inverted().applyRotation(
					oldPositionRelative));

	m_angleFrc = newAngleFrc;
	m_position = center + newPositionRelative;
	m_projectionVld = false;
}

void Camera::changeDistance(const double delta, 
		const ATD::Vector3D &center)
{
	ATD::Vector3D oldPositionRelative = m_position - center;
	double oldDistance = oldPositionRelative.length();
	double newDistance = ATD::clamp<double>(oldDistance + delta, 0.1, 100.);
	ATD::Vector3D newPositionRelative = 
		oldPositionRelative * (newDistance / oldDistance);

	m_position = center + newPositionRelative;
	m_projectionVld = false;
}

ATD::Projection3D Camera::getProjection() const
{
	if (!m_projectionVld) {
		m_projection = 
			ATD::Projection3D(ATD::Transform3D(ATD::Vector3D(1., 1., 1.), 
						angleXyToQuaternion(m_angleFrc), m_position));
	}

	return m_projection;
}

ATD::Vector3D Camera::getXAxis() const
{
	ATD::Quaternion rotation = angleXyToQuaternion(m_angleFrc);
	return rotation.applyRotation(ATD::Vector3D(1., 0., 0.));
}

ATD::Vector3D Camera::getYAxis() const
{
	ATD::Quaternion rotation = angleXyToQuaternion(m_angleFrc);
	return rotation.applyRotation(ATD::Vector3D(0., 1., 0.));
}


