
#include <ATD/Core/MinMax.hpp>
#include <ATD/Core/Transform3D.hpp>
#include <ATD/Core/Quaternion.hpp>

#include <math.h>


class Camera
{
public:
	Camera();

	void RotateAround(const ATD::Vector2D &deltaFrc, 
			const ATD::Vector3D &center);

	void ChangeDistance(const double delta, 
			const ATD::Vector3D &center);

	ATD::Projection3D GetProjection() const;

private:
	ATD::Vector3D m_position;
	ATD::Vector2D m_angleFrc;

	mutable ATD::Projection3D m_projection;
	mutable bool m_projectionVld;
};


/* auxiliary: */

ATD::Quaternion AngleXyToQuaternion(const ATD::Vector2D &angleXyFrc)
{
	ATD::Quaternion rotationX = 
		ATD::Quaternion::Rotation(angleXyFrc.x, ATD::Vector3D(1., 0., 0.));

	ATD::Quaternion rotationY = 
		ATD::Quaternion::Rotation(angleXyFrc.y, ATD::Vector3D(0., 1., 0.));

	return rotationX * rotationY;
}


/* === */

Camera::Camera()
	: m_position()
	, m_angleFrc()
	, m_projection()
	, m_projectionVld(false)
{}

void Camera::RotateAround(const ATD::Vector2D &deltaFrc, 
		const ATD::Vector3D &center)
{
	double newAngleFrcX = ATD::Clamp<double>(m_angleFrc.x + deltaFrc.x, 
			-0.24, 0.24);

	double newAngleFrcY = m_angleFrc.y + deltaFrc.y;
	newAngleFrcY -= ::floor(newAngleFrcY);

	ATD::Vector2D newAngleFrc = ATD::Vector2D(newAngleFrcX, newAngleFrcY);

	ATD::Quaternion oldRotation = AngleXyToQuaternion(m_angleFrc);
	ATD::Quaternion newRotation = AngleXyToQuaternion(newAngleFrc);

	ATD::Vector3D oldPositionRelative = m_position - center;
	ATD::Vector3D newPositionRelative = 
		newRotation.ApplyRotation(oldRotation.Inverted().ApplyRotation(
					oldPositionRelative));

	m_angleFrc = newAngleFrc;
	m_position = center + newPositionRelative;
	m_projectionVld = false;
}

void Camera::ChangeDistance(const double delta, 
		const ATD::Vector3D &center)
{
	ATD::Vector3D oldPositionRelative = m_position - center;
	double oldDistance = oldPositionRelative.Length();
	double newDistance = ATD::Clamp<double>(oldDistance + delta, 0.1, 100.);
	ATD::Vector3D newPositionRelative = 
		oldPositionRelative * (newDistance / oldDistance);

	m_position = center + newPositionRelative;
	m_projectionVld = false;
}

ATD::Projection3D Camera::GetProjection() const
{
	if (!m_projectionVld) {
		m_projection = 
			ATD::Projection3D(ATD::Transform3D(ATD::Vector3D(1., 1., 1.), 
						AngleXyToQuaternion(m_angleFrc), m_position));
	}

	return m_projection;
}


