#include "SourceVoice3D.h"



CSourceVoice3D::~CSourceVoice3D()
{
}

void CSourceVoice3D::SetVolume(float fVolume)
{
	m_pSourceVoice->SetVolume(fVolume);
}

void CSourceVoice3D::UpdatePosition(float x, float y, float z)
{
	m_position.SetXYZ(x, y, z);
}

void CSourceVoice3D::UpdateVelocity(float x, float y, float z)
{
	m_velocity.SetXYZ(x, y, z);
}

const CMelonVector3D & CSourceVoice3D::GetPosition() const
{
	return m_position;
}

const CMelonVector3D & CSourceVoice3D::GetVelocity() const
{
	return m_velocity;
}


const CMelonMatrix3 & CSourceVoice3D::GetOrientation() const
{
	return m_orientation;
}

void CSourceVoice3D::Tick()
{
	PanTick();


	if (m_bFalloffEnabled)
	{
		if (m_fFalloffRadius < m_position.CalculateDistance(CListener::GetInstance().GetPosition()))
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
	CMelonVector3D sourceListenerVector = m_position.CalculateVectorBetweenPoints(CListener::GetInstance().GetPosition());
	float sourceListenerDistance = sourceListenerVector.CalculateLength();
	float debug = (DEBUG_SPEED_OF_SOUND - (sourceListenerVector.CalculateDotProduct(m_velocity) / sourceListenerDistance)) / 
		(DEBUG_SPEED_OF_SOUND - (sourceListenerVector.CalculateDotProduct(CListener::GetInstance().GetVelocity()) / sourceListenerDistance));
	m_pSourceVoice->SetSourceSampleRate(m_uiBaseSampleRate * debug);
}


// TODO: use the old version of the getChannel function for Windows 7
void CSourceVoice3D::PanTick()
{
	// position translated and rotated above/below/on x-z plane
	CMelonVector3D positionInLocalSpace = CListener::GetInstance().GetOrientation() * (m_position - CListener::GetInstance().GetPosition());
	
	// angle to x axis, this assumes y = 0 after a projection so we can use the 2D formula
	float fPanAngle = atan2(positionInLocalSpace.GetZ(), positionInLocalSpace.GetX());


	// TODO: calculate and apply output matrix
	// Debug assumes a 5.1 System for now
	const int iDestinationChannels = 6;
	float afOutputMatrix[iDestinationChannels] = { 0.f };
	

	SPanParameter debug_pan;

	// left side if panangle greater than 0, right side if smaller
	if (fPanAngle > 0.f)
	{
		if (fPanAngle < debug_pan.FrontSpeakerAngles)
		{
			float fPan = debug_pan.FrontSpeakerAngles / fPanAngle;
			MELON_SPEAKER_FRONT_CENTER = cos(M_PI_2 * fPan);
			MELON_SPEAKER_FRONT_LEFT = sin(M_PI_2 * fPan);
		}
		else if (fPanAngle < debug_pan.BackSpeakerAngles)
		{
			float fPan = (debug_pan.BackSpeakerAngles - debug_pan.FrontSpeakerAngles) / (fPanAngle - debug_pan.FrontSpeakerAngles);
			MELON_SPEAKER_FRONT_LEFT = cos(M_PI_2 * fPan);
			MELON_SPEAKER_BACK_LEFT = sin(M_PI_2 * fPan);
		}
		else
		{
			float fPan = (M_PI - debug_pan.BackSpeakerAngles) * 2.f / (fPanAngle - debug_pan.BackSpeakerAngles);
			MELON_SPEAKER_BACK_LEFT= cos(M_PI_2 * fPan);
			MELON_SPEAKER_BACK_RIGHT = sin(M_PI_2 * fPan);
		}
	}
	else
	{
		fPanAngle = -fPanAngle;
		if (fPanAngle == 0.f)
		{
			MELON_SPEAKER_FRONT_CENTER = 1.f;
		}
		else if (fPanAngle < debug_pan.FrontSpeakerAngles)
		{
			// special case that would cause / 0
			
			float fPan = debug_pan.FrontSpeakerAngles / fPanAngle;
			MELON_SPEAKER_FRONT_CENTER = cos(M_PI_2 * fPan);
			MELON_SPEAKER_FRONT_RIGHT = sin(M_PI_2 * fPan);
		}
		else if (fPanAngle < debug_pan.BackSpeakerAngles)
		{
			float fPan = (debug_pan.BackSpeakerAngles - debug_pan.FrontSpeakerAngles) / (fPanAngle - debug_pan.FrontSpeakerAngles);
			MELON_SPEAKER_FRONT_RIGHT = cos(M_PI_2 * fPan);
			MELON_SPEAKER_BACK_RIGHT = sin(M_PI_2 * fPan);
		}
		else
		{
			float fPan = (M_PI - debug_pan.BackSpeakerAngles) * 2.f / (fPanAngle - debug_pan.BackSpeakerAngles);
			MELON_SPEAKER_BACK_RIGHT = cos(M_PI_2 * fPan);
			MELON_SPEAKER_BACK_LEFT = sin(M_PI_2 * fPan);
		}
	}

	if (debug_pan.VerticalPanEnabled)
	{
		// where 0 would mean == y axis and 180 == -y axis
		float fVerticalAngle = positionInLocalSpace.CalculateAngleToYAxis();

		if (fVerticalAngle >= debug_pan.VerticalPanAngle)
		{
			float fVerticalPan = debug_pan.VerticalPanAngle / fVerticalAngle * (debug_pan.VerticalPanPowerMax - debug_pan.VerticalPanPowerMin);
			for (int i = 0; i < iDestinationChannels; i++)
			{
				if (afOutputMatrix[i] < debug_pan.VerticalPanPowerMin)
					afOutputMatrix[i] = fVerticalPan;
			}
		}
	}

	m_pSourceVoice->SetOutputMatrix(nullptr, 1, iDestinationChannels, afOutputMatrix);
}
