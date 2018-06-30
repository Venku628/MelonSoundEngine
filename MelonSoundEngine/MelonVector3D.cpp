#include "MelonVector3D.h"



CMelonVector3D::CMelonVector3D()
{
	m_fX = 0.f;
	m_fY = 0.f;
	m_fZ = 0.f;
}

CMelonVector3D::CMelonVector3D(float x, float y, float z)
{
	m_fX = x;
	m_fY = y;
	m_fZ = z;
}


CMelonVector3D::~CMelonVector3D()
{
}

// Basicly  other - this = new vector
CMelonVector3D CMelonVector3D::CalculateVectorBetweenPoints(CMelonVector3D & other) const
{
	return CMelonVector3D(other.m_fX - m_fX, other.m_fY - m_fY, other.m_fZ - m_fZ);
}

float CMelonVector3D::CalculateDotProduct(CMelonVector3D & other) const
{
	return (m_fX*other.m_fX + m_fY*other.m_fY + m_fZ*other.m_fZ);
}

float CMelonVector3D::CalculateDistance(CMelonVector3D& other) const
{
	// doesn´t use CalculateLength because creating a vector first is not efficent
	return sqrt((m_fX - other.m_fX)*(m_fX - other.m_fX) + (m_fY - other.m_fY)*(m_fY - other.m_fY) + (m_fZ - other.m_fZ)*(m_fZ - other.m_fZ));
}

float CMelonVector3D::CalculateLength() const
{
	return sqrt(m_fX*m_fX + m_fY*m_fY + m_fZ*m_fZ);
}

float CMelonVector3D::GetX() const
{
	return m_fX;
}

float CMelonVector3D::GetY() const
{
	return m_fY;
}

float CMelonVector3D::GetZ() const
{
	return m_fZ;
}

void CMelonVector3D::SetX(float x)
{
	m_fX = x;
}

void CMelonVector3D::SetY(float y)
{
	m_fY = y;
}

void CMelonVector3D::SetZ(float z)
{
	m_fZ = z;
}

void CMelonVector3D::SetXYZ(float x, float y, float z)
{
	m_fX = x;
	m_fY = y;
	m_fZ = z;
}
