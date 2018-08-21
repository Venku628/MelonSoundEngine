#include "SourceVoice3D.h"



CSourceVoice3D::~CSourceVoice3D()
{
}

void CSourceVoice3D::SetVolume(float fVolume)
{
	m_fBaseVolume = fVolume;
	m_pSourceVoice->SetVolume(fVolume);
}

float CSourceVoice3D::GetVolume()
{
	return m_fBaseVolume;
}

HRESULT CSourceVoice3D::StartPlayback()
{
	return m_pSourceVoice->Start();
}

HRESULT CSourceVoice3D::StopPlayback()
{
	return m_pSourceVoice->Stop();
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
	if (m_bFalloffEnabled)
	{
		float fdistance = m_position.CalculateDistance(CListener::GetInstance().GetPosition());
		if (m_fFalloffRadius > fdistance)
		{
			if (m_bVoiceMuted)
			{
				m_bVoiceMuted = false;
			}

			m_pSourceVoice->SetVolume(((1.f - fdistance / m_fFalloffRadius) * (1.f - m_fMaxFalloff)) * m_fBaseVolume);

			if (m_bDopplerEffectEnabled)
			{
				DopplerEffectTick();
			}

			if (m_bPanEnabled)
			{
				PanTick();
			}
		}
		else if (!m_bVoiceMuted)
		{
			m_pSourceVoice->SetVolume(0.f);
			m_bVoiceMuted = true;
		}
	}
	else
	{
		m_pSourceVoice->SetVolume(m_fBaseVolume);

		if (m_bDopplerEffectEnabled)
		{
			DopplerEffectTick();
		}

		if (m_bPanEnabled)
		{
			PanTick();
		}
	}
}

// TODO: get non constant speed of sound from somewhere
void CSourceVoice3D::DopplerEffectTick()
{
	CMelonVector3D sourceListenerVector = m_position.CalculateVectorBetweenPoints(CListener::GetInstance().GetPosition());
	float sourceListenerDistance = sourceListenerVector.CalculateLength();
	m_pSourceVoice->SetSourceSampleRate(m_uiBaseSampleRate * (
		(DEFAULT_SPEED_OF_SOUND - (sourceListenerVector.CalculateDotProduct(m_velocity) / sourceListenerDistance)) /
		(DEFAULT_SPEED_OF_SOUND - (sourceListenerVector.CalculateDotProduct(CListener::GetInstance().GetVelocity()) / sourceListenerDistance))));
}


// TODO: use the old version of the getChannel function for Windows 7
void CSourceVoice3D::PanTick()
{
	// position translated and rotated above/below/on x-z plane
	CMelonVector3D positionInLocalSpace = CListener::GetInstance().GetOrientation() * (m_position - CListener::GetInstance().GetPosition());
	
	// angle to x axis, this assumes y = 0 after a projection so we can use the 2D formula
	float fPanAngle = atan2(positionInLocalSpace.GetZ(), positionInLocalSpace.GetX());

	SPanParameter debug_pan;
	const DWORD dwChannelMask = CListener::GetInstance().GetChannelMask();
    

	// TODO: calculate and apply output matrix
	// Debug assumes a 5.1 System for now

	switch (dwChannelMask)
	{
	case SPEAKER_MONO:
		// input to output is 1.0f, the only difference in volume is caused the falloffradius
		break;
	case SPEAKER_STEREO:
	{
		const int iDestinationChannels = 2;
		float afOutputMatrix[iDestinationChannels] = { 0.f };

		// left side if panangle greater than 0, right side if smaller
		if (fPanAngle > 0.f)
		{
			if (fPanAngle < debug_pan.fFrontSpeakerAngles)
			{
				float fPan = (debug_pan.fFrontSpeakerAngles - fPanAngle) / (debug_pan.fFrontSpeakerAngles * 2.f);
				MELON_SPEAKER_FRONT_RIGHT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_FRONT_LEFT = sin(M_PI_2 * fPan);
			}
			else
			{
				float fPan = (fPanAngle - debug_pan.fFrontSpeakerAngles) / ((M_PI - debug_pan.fFrontSpeakerAngles) * 2.f);
				MELON_SPEAKER_FRONT_LEFT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_FRONT_RIGHT = sin(M_PI_2 * fPan);
			}
		}
		else
		{
			fPanAngle = -fPanAngle;
			if (fPanAngle == 0.f)
			{
				MELON_SPEAKER_FRONT_LEFT = 0.6f;
				MELON_SPEAKER_FRONT_RIGHT = 0.6f;
			}
			else
			{
				if (fPanAngle < debug_pan.fFrontSpeakerAngles)
				{
					float fPan = (debug_pan.fFrontSpeakerAngles - fPanAngle) / (debug_pan.fFrontSpeakerAngles * 2.f);
					MELON_SPEAKER_FRONT_LEFT = cos(M_PI_2 * fPan);
					MELON_SPEAKER_FRONT_RIGHT = sin(M_PI_2 * fPan);
				}
				else
				{
					float fPan = (fPanAngle - debug_pan.fFrontSpeakerAngles) / ((M_PI - debug_pan.fFrontSpeakerAngles) * 2.f);
					MELON_SPEAKER_FRONT_RIGHT = cos(M_PI_2 * fPan);
					MELON_SPEAKER_FRONT_LEFT = sin(M_PI_2 * fPan);
				}
			}
			
		}

		if (debug_pan.bVerticalPanEnabled)
		{
			// where 0 would mean == y axis and 180 == -y axis
			float fVerticalAngle = positionInLocalSpace.CalculateAngleToYAxis();

			if (fVerticalAngle >= debug_pan.fVerticalPanAngle)
			{
				float fVerticalPan = debug_pan.fVerticalPanAngle / fVerticalAngle * (1.f - debug_pan.fVerticalPanPowerMin);
				if (afOutputMatrix[0] < debug_pan.fVerticalPanPowerMin)
						afOutputMatrix[0] = fVerticalPan;
				if (afOutputMatrix[1] < debug_pan.fVerticalPanPowerMin)
						afOutputMatrix[1] = fVerticalPan;
			}
		}
		m_pSourceVoice->SetOutputMatrix(nullptr, 1, iDestinationChannels, afOutputMatrix);

		break;
	}
	// no direct support in windows 10
	case SPEAKER_2POINT1:
	// it is unclear to me what the outputmatrix of these options would look like, the documentation on msdn offers no examples or explanations
	case SPEAKER_SURROUND:
	case SPEAKER_QUAD:
		break;
	// no direct support in windows 10
	case SPEAKER_4POINT1:
		break;

	case SPEAKER_5POINT1:
	case SPEAKER_5POINT1_SURROUND:
	{
		const int iDestinationChannels = 6;
		float afOutputMatrix[iDestinationChannels] = { 0.f };

		// left side if panangle greater than 0, right side if smaller
		if (fPanAngle > 0.f)
		{
			if (fPanAngle < debug_pan.fFrontSpeakerAngles)
			{
				float fPan = fPanAngle / debug_pan.fFrontSpeakerAngles;
				MELON_SPEAKER_FRONT_CENTER = cos(M_PI_2 * fPan);
				MELON_SPEAKER_FRONT_LEFT = sin(M_PI_2 * fPan);
			}
			else if (fPanAngle < debug_pan.fBackSpeakerAngles)
			{
				float fPan = (fPanAngle - debug_pan.fFrontSpeakerAngles) / (debug_pan.fBackSpeakerAngles - debug_pan.fFrontSpeakerAngles);
				MELON_SPEAKER_FRONT_LEFT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_BACK_LEFT = sin(M_PI_2 * fPan);
			}
			else
			{
				float fPan = (fPanAngle - debug_pan.fBackSpeakerAngles) / ((M_PI - debug_pan.fBackSpeakerAngles) * 2.f);
				MELON_SPEAKER_BACK_LEFT = cos(M_PI_2 * fPan);
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
			else if (fPanAngle < debug_pan.fFrontSpeakerAngles)
			{
				// special case that would cause / 0

				float fPan = fPanAngle / debug_pan.fFrontSpeakerAngles;
				MELON_SPEAKER_FRONT_CENTER = cos(M_PI_2 * fPan);
				MELON_SPEAKER_FRONT_RIGHT = sin(M_PI_2 * fPan);
			}
			else if (fPanAngle < debug_pan.fBackSpeakerAngles)
			{
				float fPan = (fPanAngle - debug_pan.fFrontSpeakerAngles) / (debug_pan.fBackSpeakerAngles - debug_pan.fFrontSpeakerAngles);
				MELON_SPEAKER_FRONT_RIGHT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_BACK_RIGHT = sin(M_PI_2 * fPan);
			}
			else
			{
				float fPan = (fPanAngle - debug_pan.fBackSpeakerAngles) / ((M_PI - debug_pan.fBackSpeakerAngles) * 2.f);
				MELON_SPEAKER_BACK_RIGHT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_BACK_LEFT = sin(M_PI_2 * fPan);
			}
		}

		if (debug_pan.bVerticalPanEnabled)
		{
			// where 0 would mean == y axis and 180 == -y axis
			float fVerticalAngle = positionInLocalSpace.CalculateAngleToYAxis();

			if (fVerticalAngle >= debug_pan.fVerticalPanAngle)
			{
				float fVerticalPan = debug_pan.fVerticalPanAngle / fVerticalAngle * (1.f - debug_pan.fVerticalPanPowerMin);
				
				if (afOutputMatrix[0] < debug_pan.fVerticalPanPowerMin)
					afOutputMatrix[0] = fVerticalPan;
				if (afOutputMatrix[1] < debug_pan.fVerticalPanPowerMin)
					afOutputMatrix[1] = fVerticalPan;
				if (afOutputMatrix[2] < debug_pan.fVerticalPanPowerMin)
					afOutputMatrix[2] = fVerticalPan;
				if (afOutputMatrix[4] < debug_pan.fVerticalPanPowerMin)
					afOutputMatrix[4] = fVerticalPan;
				if (afOutputMatrix[5] < debug_pan.fVerticalPanPowerMin)
					afOutputMatrix[5] = fVerticalPan;
			}
		}
		m_pSourceVoice->SetOutputMatrix(nullptr, 1, iDestinationChannels, afOutputMatrix);

		break;
	}
	case SPEAKER_7POINT1:
	case SPEAKER_7POINT1_SURROUND:
	{
		const int iDestinationChannels = 8;
		float afOutputMatrix[iDestinationChannels] = { 0.f };

		// left side if panangle greater than 0, right side if smaller
		if (fPanAngle > 0.f)
		{
			if (fPanAngle < debug_pan.fFrontSpeakerAngles)
			{
				float fPan = fPanAngle / debug_pan.fFrontSpeakerAngles;
				MELON_SPEAKER_FRONT_CENTER = cos(M_PI_2 * fPan);
				MELON_SPEAKER_FRONT_LEFT = sin(M_PI_2 * fPan);
			}
			else if (fPanAngle < debug_pan.fSurroundSpeakerAngles)
			{
				float fPan = (fPanAngle - debug_pan.fFrontSpeakerAngles) / (debug_pan.fSurroundSpeakerAngles - debug_pan.fFrontSpeakerAngles);
				MELON_SPEAKER_FRONT_LEFT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_BACK_LEFT = sin(M_PI_2 * fPan);
			}
			else if (fPanAngle < debug_pan.fBackSpeakerAngles)
			{
				float fPan = (fPanAngle - debug_pan.fSurroundSpeakerAngles) / (debug_pan.fBackSpeakerAngles - debug_pan.fSurroundSpeakerAngles);
				MELON_SPEAKER_FRONT_LEFT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_BACK_LEFT = sin(M_PI_2 * fPan);
			}
			else
			{
				float fPan = (fPanAngle - debug_pan.fBackSpeakerAngles) / ((M_PI - debug_pan.fBackSpeakerAngles) * 2.f);
				MELON_SPEAKER_BACK_LEFT = cos(M_PI_2 * fPan);
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
			else if (fPanAngle < debug_pan.fFrontSpeakerAngles)
			{
				// special case that would cause / 0

				float fPan = fPanAngle / debug_pan.fFrontSpeakerAngles;
				MELON_SPEAKER_FRONT_CENTER = cos(M_PI_2 * fPan);
				MELON_SPEAKER_FRONT_RIGHT = sin(M_PI_2 * fPan);
			}
			else if (fPanAngle < debug_pan.fSurroundSpeakerAngles)
			{
				float fPan = (fPanAngle - debug_pan.fFrontSpeakerAngles) / (debug_pan.fSurroundSpeakerAngles - debug_pan.fFrontSpeakerAngles);
				MELON_SPEAKER_FRONT_LEFT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_BACK_LEFT = sin(M_PI_2 * fPan);
			}
			else if (fPanAngle < debug_pan.fBackSpeakerAngles)
			{
				float fPan = (fPanAngle - debug_pan.fSurroundSpeakerAngles) / (debug_pan.fBackSpeakerAngles - debug_pan.fSurroundSpeakerAngles);
				MELON_SPEAKER_FRONT_LEFT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_BACK_LEFT = sin(M_PI_2 * fPan);
			}
			else
			{
				float fPan = (fPanAngle - debug_pan.fBackSpeakerAngles) / ((M_PI - debug_pan.fBackSpeakerAngles) * 2.f);
				MELON_SPEAKER_BACK_RIGHT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_BACK_LEFT = sin(M_PI_2 * fPan);
			}
		}

		if (debug_pan.bVerticalPanEnabled)
		{
			// where 0 would mean == y axis and 180 == -y axis
			float fVerticalAngle = positionInLocalSpace.CalculateAngleToYAxis();

			if (fVerticalAngle >= debug_pan.fVerticalPanAngle)
			{
				float fVerticalPan = debug_pan.fVerticalPanAngle / fVerticalAngle * (1.f - debug_pan.fVerticalPanPowerMin);
				
				if (afOutputMatrix[0] < debug_pan.fVerticalPanPowerMin)
					afOutputMatrix[0] = fVerticalPan;
				if (afOutputMatrix[1] < debug_pan.fVerticalPanPowerMin)
					afOutputMatrix[1] = fVerticalPan;
				if (afOutputMatrix[2] < debug_pan.fVerticalPanPowerMin)
					afOutputMatrix[2] = fVerticalPan;
				if (afOutputMatrix[4] < debug_pan.fVerticalPanPowerMin)
					afOutputMatrix[4] = fVerticalPan;
				if (afOutputMatrix[5] < debug_pan.fVerticalPanPowerMin)
					afOutputMatrix[5] = fVerticalPan;
				if (afOutputMatrix[6] < debug_pan.fVerticalPanPowerMin)
					afOutputMatrix[6] = fVerticalPan;
				if (afOutputMatrix[7] < debug_pan.fVerticalPanPowerMin)
					afOutputMatrix[7] = fVerticalPan;

			}
		}
		m_pSourceVoice->SetOutputMatrix(nullptr, 1, iDestinationChannels, afOutputMatrix);
		break;
	}
	}
}
