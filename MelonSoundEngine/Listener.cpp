#include "Listener.h"



CListener::CListener()
{
	m_position = CMelonVector3D(0.f, 0.f, 0.f);
	m_velocity = CMelonVector3D(1.f, 0.f, 0.f);
	m_rotation = CMelonMatrix3();

}

CListener::CListener(float x, float y, float z)
{
	m_position = CMelonVector3D(x, y, z);
	m_velocity = CMelonVector3D(0.f, 0.f, 0.f);
	m_rotation = CMelonMatrix3();
}


CListener::~CListener()
{
}

void CListener::SetChannelMask(DWORD dwChannelMask)
{
	m_dwChannelMask = dwChannelMask;
}

void CListener::UpdatePosition(float x, float y, float z)
{
	m_position.SetXYZ(x, y, z);
}

void CListener::UpdateVelocity(float x, float y, float z)
{
	m_velocity.SetXYZ(x, y, z);
}

void CListener::UpdateOrientation(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
	m_rotation.Update(m00, m01, m02, m10, m11, m12, m20, m21, m22);
	m_orientationTopDotOrientationTop = m01 * m01 + m11 * m11 * m21 * m21;
}

const CMelonVector3D & CListener::GetPosition() const
{
	return m_position;
}

const CMelonVector3D & CListener::GetVelocity() const
{
	return m_velocity;
}

const CMelonMatrix3& CListener::GetOrientation() const
{
	return m_rotation;
}

const DWORD CListener::GetChannelMask() const
{
	return m_dwChannelMask;
}

CListener & CListener::GetInstance()
{
	static CListener instance;
	return instance;
}

// Calculates a point on the pan plane using orthogonal projection.
CMelonVector3D CListener::CalculatePointOnPanPlane(CMelonVector3D & sourcePoint)
{
	//									(Sourcepos - Listenerpos) * Plane Normalvector
	// Sourcepos - Plane Normalvector * ---------------------------------------------- = Point projected on Plane
	//									Plane Normalvector * Plane Normalvector        <--precaclutalted
	return sourcePoint - m_rotation.GetYAxis() * ((sourcePoint - m_position) * m_rotation.GetYAxis() / m_orientationTopDotOrientationTop);
}

float CListener::CalculateAngleToPanPlane(CMelonVector3D & sourcePoint, CMelonVector3D & pointOnPlane)
{


	return 0.0f;
}

float CListener::CalculateAngleToOrientation(CMelonVector3D & pointOnPlane)
{
	return 0.0f;
}


