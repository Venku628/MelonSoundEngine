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
	if (m_bFalloffEnabled)
	{
		if (m_fFalloffRadius < m_position->CalculateDistance(*CListener::GetInstance().GetPositionPointer()))
		{
			if (m_bVoiceMuted)
			{
				m_bVoiceMuted = false;
				m_pSourceVoice->SetVolume(m_fVolume);
			}

			if (m_bDopplerEffectEnabled)
			{
				DopplerEffectTick();
			}
			// Room for more Tick stuff
		}
		else if (!m_bVoiceMuted)
		{
			m_pSourceVoice->GetVolume(&m_fVolume);
			m_pSourceVoice->SetVolume(0.f);
			m_bVoiceMuted = true;
		}
	}
	else
	{
		if (m_bDopplerEffectEnabled)
		{
			DopplerEffectTick();
		}
		// Room for more Tick stuff
	}
}

// TODO: get non constant speed of sound from somewhere and remove debug
void CSourceVoice3D::DopplerEffectTick()
{
	CMelonVector3D sourceListenerVector = m_position->CalculateVectorBetweenPoints(*CListener::GetInstance().GetPositionPointer());
	float sourceListenerDistance = sourceListenerVector.CalculateLength();
	float debug = (DEBUG_SPEED_OF_SOUND - (sourceListenerVector.CalculateDotProduct(*m_velocity) / sourceListenerDistance)) / 
		(DEBUG_SPEED_OF_SOUND - (sourceListenerVector.CalculateDotProduct(*CListener::GetInstance().GetVelocityPointer()) / sourceListenerDistance));
	m_pSourceVoice->SetSourceSampleRate(m_uiBaseSampleRate * debug);
}

void CSourceVoice3D::PanTick()
{
	CMelonVector3D positionOnPanPlane = CListener::GetInstance().CalculatePointOnPanPlane(*m_position);
	float anglePositionPanPlane = m_position->CalculateVectorBetweenPoints(positionOnPanPlane).CalculateAngleBetweenVectors(m_position->CalculateVectorBetweenPoints(positionOnPanPlane));
	float anglePositionOnPanPlaneOrientation = 0.f;


	// TODO: calculate and apply output matrix


}
