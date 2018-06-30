#include "SourceVoice3D.h"



CSourceVoice3D::~CSourceVoice3D()
{
	delete m_position;
	delete m_velocity;
	delete m_orientationFront;
	delete m_orientationTop;
}

void CSourceVoice3D::SetVolume(float fVolume)
{
	m_pSourceVoice->SetVolume(fVolume);
}

void CSourceVoice3D::UpdatePosition(float x, float y, float z)
{
	m_position->SetXYZ(x, y, z);
}

void CSourceVoice3D::UpdateVelocity(float x, float y, float z)
{
	m_velocity->SetXYZ(x, y, z);
}

void CSourceVoice3D::UpdateOrientationFront(float x, float y, float z)
{
	m_orientationFront->SetXYZ(x, y, z);
}

void CSourceVoice3D::UpdateOrientationTop(float x, float y, float z)
{
	m_orientationTop->SetXYZ(x, y, z);
}

const CMelonVector3D * CSourceVoice3D::GetPositionPointer() const
{
	return m_position;
}

const CMelonVector3D * CSourceVoice3D::GetVelocityPointer() const
{
	return m_velocity;
}

const CMelonVector3D * CSourceVoice3D::GetOrientationFrontPointer() const
{
	return m_orientationFront;
}

const CMelonVector3D * CSourceVoice3D::GetOrientationTopPointer() const
{
	return m_orientationTop;
}

void CSourceVoice3D::Tick()
{
	if (m_bDopplerEffectEnabled)
	{
		DopplerEffectTick();
	}


}

void CSourceVoice3D::DopplerEffectTick()
{
	 
}
