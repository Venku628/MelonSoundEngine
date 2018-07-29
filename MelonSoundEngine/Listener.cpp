#include "Listener.h"



CListener::CListener()
{
	m_position = new CMelonVector3D(1.f, 0.f, 0.f);
	m_velocity = new CMelonVector3D(1.f, 0.f, 0.f);
	m_orientationFront = new CMelonVector3D(0.f, 0.f, 0.f);
	m_orientationTop = new CMelonVector3D(0.f, 0.f, 0.f);
}

CListener::CListener(float x, float y, float z)
{
	m_position = new CMelonVector3D(x, y, z);
}


CListener::~CListener()
{
	delete m_position;
	delete m_velocity;
	delete m_orientationFront;
	delete m_orientationTop;
}

void CListener::UpdatePosition(float x, float y, float z)
{
	m_position->SetXYZ(x, y, z);
}

void CListener::UpdateVelocity(float x, float y, float z)
{
	m_velocity->SetXYZ(x, y, z);
}

void CListener::UpdateOrientationFront(float x, float y, float z)
{
	m_orientationFront->SetXYZ(x, y, z);
	m_orientationTopDotOrientationTop = x * x + y * y + z * z;
}

void CListener::UpdateOrientationTop(float x, float y, float z)
{
	m_orientationTop->SetXYZ(x, y, z);
}

const CMelonVector3D * CListener::GetPositionPointer() const
{
	return m_position;
}

const CMelonVector3D * CListener::GetVelocityPointer() const
{
	return m_velocity;
}

const CMelonVector3D * CListener::GetOrientationFrontPointer() const
{
	return m_orientationFront;
}

const CMelonVector3D * CListener::GetOrientationTopPointer() const
{
	return m_orientationTop;
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
	return sourcePoint - *m_orientationTop * ((sourcePoint - *m_position) * *m_orientationTop / m_orientationTopDotOrientationTop);
}

float CListener::CalculateAngleToPanPlane(CMelonVector3D & sourcePoint, CMelonVector3D & pointOnPlane)
{


	return 0.0f;
}

float CListener::CalculateAngleToOrientation(CMelonVector3D & pointOnPlane)
{
	return 0.0f;
}


