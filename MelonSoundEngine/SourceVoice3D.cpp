#include "SourceVoice3D.h"



CSourceVoice3D::~CSourceVoice3D()
{
}

void CSourceVoice3D::SetVolume(float fVolume)
{
	m_fBaseVolume = fVolume;
}

void CSourceVoice3D::SetSampleRate(unsigned int sampleRate)
{
	m_uiBaseSampleRate = sampleRate;
}

void CSourceVoice3D::UpdatePosition(float x, float y, float z)
{
	m_position.SetXYZ(x, y, z);
}

void CSourceVoice3D::UpdateVelocity(float x, float y, float z)
{
	m_velocity.SetXYZ(x, y, z);
}

void CSourceVoice3D::UpdateRotation(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
	m_rotation.Update(m00, m01, m02, m10, m11, m12, m20, m21, m22);
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

void CSourceVoice3D::SetPanParameters(SPanParameters & parameters)
{
	m_bPanEnabled = parameters.bPanEnabled;
	if (m_bPanEnabled)
	{
		m_fCenterSpeakerPanMin = parameters.fCenterSpeakerPanMin;
		m_fFrontSpeakerAngles = parameters.fFrontSpeakerAngles;
		m_fSurroundSpeakerAngles = parameters.fSurroundSpeakerAngles;
		m_fBackSpeakerAngles = parameters.fBackSpeakerAngles;
		m_fVerticalPanAngle = parameters.fVerticalPanAngle;
		m_fVerticalPanPowerMin = parameters.fVerticalPanPowerMin;
	}
}

void CSourceVoice3D::SetFalloffParameters(SFalloffParameters & parameters)
{
	m_bFalloffEnabled = parameters.bFalloffEnabled;
	if (m_bFalloffEnabled)
	{
		m_fFalloffRadius = parameters.fFalloffRadius;
		m_fMaxFalloff = parameters.fMaxFalloff;
	}
}

void CSourceVoice3D::SetDopplerEffectParameters(SDopplerEffectParameters & parameters)
{
	m_bDopplerEffectEnabled = parameters.bDopplerEffectEnbabled;
	m_fSpeedOfSound = parameters.fDopplerEffectSpeedOfSound;
}

void CSourceVoice3D::DopplerEffectTick()
{
	CMelonVector3D sourceListenerVector = m_position.CalculateVectorBetweenPoints(CListener::GetInstance().GetPosition());
	float sourceListenerDistance = sourceListenerVector.CalculateLength();
	m_pSourceVoice->SetSourceSampleRate(m_uiBaseSampleRate * (
		(m_fSpeedOfSound - (sourceListenerVector.CalculateDotProduct(m_velocity) / sourceListenerDistance)) /
		(m_fSpeedOfSound - (sourceListenerVector.CalculateDotProduct(CListener::GetInstance().GetVelocity()) / sourceListenerDistance))));
}


// TODO: use the old version of the getChannel function for Windows 7
void CSourceVoice3D::PanTick()
{
	// position translated and rotated above/below/on x-z plane
	CMelonVector3D positionInLocalSpace = CListener::GetInstance().GetOrientation() * (m_position - CListener::GetInstance().GetPosition());
	
	// angle to x axis, this assumes y = 0 after a projection so we can use the 2D formula
	float fPanAngle = atan2(positionInLocalSpace.GetZ(), positionInLocalSpace.GetX());


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
			if (fPanAngle < m_fFrontSpeakerAngles)
			{
				float fPan = (m_fFrontSpeakerAngles - fPanAngle) / (m_fFrontSpeakerAngles * 2.f);
				MELON_SPEAKER_FRONT_RIGHT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_FRONT_LEFT = sin(M_PI_2 * fPan);
			}
			else
			{
				float fPan = (fPanAngle - m_fFrontSpeakerAngles) / ((M_PI - m_fFrontSpeakerAngles) * 2.f);
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
				if (fPanAngle < m_fFrontSpeakerAngles)
				{
					float fPan = (m_fFrontSpeakerAngles - fPanAngle) / (m_fFrontSpeakerAngles * 2.f);
					MELON_SPEAKER_FRONT_LEFT = cos(M_PI_2 * fPan);
					MELON_SPEAKER_FRONT_RIGHT = sin(M_PI_2 * fPan);
				}
				else
				{
					float fPan = (fPanAngle - m_fFrontSpeakerAngles) / ((M_PI - m_fFrontSpeakerAngles) * 2.f);
					MELON_SPEAKER_FRONT_RIGHT = cos(M_PI_2 * fPan);
					MELON_SPEAKER_FRONT_LEFT = sin(M_PI_2 * fPan);
				}
			}
			
		}

		if (m_bVerticalPanEnabled)
		{
			// where 0 would mean == y axis and 180 == -y axis
			float fVerticalAngle = positionInLocalSpace.CalculateAngleToYAxis();

			if (fVerticalAngle >= m_fVerticalPanAngle)
			{
				float fVerticalPan = m_fVerticalPanAngle / fVerticalAngle * (1.f - m_fVerticalPanPowerMin);
				if (afOutputMatrix[0] < m_fVerticalPanPowerMin)
						afOutputMatrix[0] = fVerticalPan;
				if (afOutputMatrix[1] < m_fVerticalPanPowerMin)
						afOutputMatrix[1] = fVerticalPan;
			}
		}
		m_pSourceVoice->SetOutputMatrix(nullptr, 1, iDestinationChannels, afOutputMatrix);

		break;
	}
	// no direct support in windows 10
	case SPEAKER_2POINT1:
	// no hardware to test
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
			if (fPanAngle < m_fFrontSpeakerAngles)
			{
				float fPan = fPanAngle / m_fFrontSpeakerAngles;
				MELON_SPEAKER_FRONT_CENTER = cos(M_PI_2 * fPan);
				MELON_SPEAKER_FRONT_LEFT = sin(M_PI_2 * fPan);
			}
			else if (fPanAngle < m_fBackSpeakerAngles)
			{
				float fPan = (fPanAngle - m_fFrontSpeakerAngles) / (m_fBackSpeakerAngles - m_fFrontSpeakerAngles);
				MELON_SPEAKER_FRONT_LEFT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_BACK_LEFT = sin(M_PI_2 * fPan);
			}
			else
			{
				float fPan = (fPanAngle - m_fBackSpeakerAngles) / ((M_PI - m_fBackSpeakerAngles) * 2.f);
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
			else if (fPanAngle < m_fFrontSpeakerAngles)
			{
				// special case that would cause / 0

				float fPan = fPanAngle / m_fFrontSpeakerAngles;
				MELON_SPEAKER_FRONT_CENTER = cos(M_PI_2 * fPan);
				MELON_SPEAKER_FRONT_RIGHT = sin(M_PI_2 * fPan);
			}
			else if (fPanAngle < m_fBackSpeakerAngles)
			{
				float fPan = (fPanAngle - m_fFrontSpeakerAngles) / (m_fBackSpeakerAngles - m_fFrontSpeakerAngles);
				MELON_SPEAKER_FRONT_RIGHT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_BACK_RIGHT = sin(M_PI_2 * fPan);
			}
			else
			{
				float fPan = (fPanAngle - m_fBackSpeakerAngles) / ((M_PI - m_fBackSpeakerAngles) * 2.f);
				MELON_SPEAKER_BACK_RIGHT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_BACK_LEFT = sin(M_PI_2 * fPan);
			}
		}

		if (m_bVerticalPanEnabled)
		{
			// where 0 would mean == y axis and 180 == -y axis
			float fVerticalAngle = positionInLocalSpace.CalculateAngleToYAxis();

			if (fVerticalAngle >= m_fVerticalPanAngle)
			{
				float fVerticalPan = m_fVerticalPanAngle / fVerticalAngle * (1.f - m_fVerticalPanPowerMin);
				
				if (afOutputMatrix[0] < m_fVerticalPanPowerMin)
					afOutputMatrix[0] = fVerticalPan;
				if (afOutputMatrix[1] < m_fVerticalPanPowerMin)
					afOutputMatrix[1] = fVerticalPan;
				if (afOutputMatrix[2] < m_fVerticalPanPowerMin)
					afOutputMatrix[2] = fVerticalPan;
				if (afOutputMatrix[4] < m_fVerticalPanPowerMin)
					afOutputMatrix[4] = fVerticalPan;
				if (afOutputMatrix[5] < m_fVerticalPanPowerMin)
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
			if (fPanAngle < m_fFrontSpeakerAngles)
			{
				float fPan = fPanAngle / m_fFrontSpeakerAngles;
				MELON_SPEAKER_FRONT_CENTER = cos(M_PI_2 * fPan);
				MELON_SPEAKER_FRONT_LEFT = sin(M_PI_2 * fPan);
			}
			else if (fPanAngle < m_fSurroundSpeakerAngles)
			{
				float fPan = (fPanAngle - m_fFrontSpeakerAngles) / (m_fSurroundSpeakerAngles - m_fFrontSpeakerAngles);
				MELON_SPEAKER_FRONT_LEFT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_BACK_LEFT = sin(M_PI_2 * fPan);
			}
			else if (fPanAngle < m_fBackSpeakerAngles)
			{
				float fPan = (fPanAngle - m_fSurroundSpeakerAngles) / (m_fBackSpeakerAngles - m_fSurroundSpeakerAngles);
				MELON_SPEAKER_FRONT_LEFT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_BACK_LEFT = sin(M_PI_2 * fPan);
			}
			else
			{
				float fPan = (fPanAngle - m_fBackSpeakerAngles) / ((M_PI - m_fBackSpeakerAngles) * 2.f);
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
			else if (fPanAngle < m_fFrontSpeakerAngles)
			{
				// special case that would cause / 0

				float fPan = fPanAngle / m_fFrontSpeakerAngles;
				MELON_SPEAKER_FRONT_CENTER = cos(M_PI_2 * fPan);
				MELON_SPEAKER_FRONT_RIGHT = sin(M_PI_2 * fPan);
			}
			else if (fPanAngle < m_fSurroundSpeakerAngles)
			{
				float fPan = (fPanAngle - m_fFrontSpeakerAngles) / (m_fSurroundSpeakerAngles - m_fFrontSpeakerAngles);
				MELON_SPEAKER_FRONT_LEFT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_BACK_LEFT = sin(M_PI_2 * fPan);
			}
			else if (fPanAngle < m_fBackSpeakerAngles)
			{
				float fPan = (fPanAngle - m_fSurroundSpeakerAngles) / (m_fBackSpeakerAngles - m_fSurroundSpeakerAngles);
				MELON_SPEAKER_FRONT_LEFT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_BACK_LEFT = sin(M_PI_2 * fPan);
			}
			else
			{
				float fPan = (fPanAngle - m_fBackSpeakerAngles) / ((M_PI - m_fBackSpeakerAngles) * 2.f);
				MELON_SPEAKER_BACK_RIGHT = cos(M_PI_2 * fPan);
				MELON_SPEAKER_BACK_LEFT = sin(M_PI_2 * fPan);
			}
		}

		if (m_bVerticalPanEnabled)
		{
			// where 0 would mean == y axis and 180 == -y axis
			float fVerticalAngle = positionInLocalSpace.CalculateAngleToYAxis();

			if (fVerticalAngle >= m_fVerticalPanAngle)
			{
				float fVerticalPan = m_fVerticalPanAngle / fVerticalAngle * (1.f - m_fVerticalPanPowerMin);
				
				if (afOutputMatrix[0] < m_fVerticalPanPowerMin)
					afOutputMatrix[0] = fVerticalPan;
				if (afOutputMatrix[1] < m_fVerticalPanPowerMin)
					afOutputMatrix[1] = fVerticalPan;
				if (afOutputMatrix[2] < m_fVerticalPanPowerMin)
					afOutputMatrix[2] = fVerticalPan;
				if (afOutputMatrix[4] < m_fVerticalPanPowerMin)
					afOutputMatrix[4] = fVerticalPan;
				if (afOutputMatrix[5] < m_fVerticalPanPowerMin)
					afOutputMatrix[5] = fVerticalPan;
				if (afOutputMatrix[6] < m_fVerticalPanPowerMin)
					afOutputMatrix[6] = fVerticalPan;
				if (afOutputMatrix[7] < m_fVerticalPanPowerMin)
					afOutputMatrix[7] = fVerticalPan;

			}
		}
		m_pSourceVoice->SetOutputMatrix(nullptr, 1, iDestinationChannels, afOutputMatrix);
		break;
	}
	}
}
