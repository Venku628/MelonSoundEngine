#pragma once
#include "SourceVoice.h"
#include "Listener.h"

#define MELON_SPEAKER_FRONT_LEFT afOutputMatrix[0]
#define MELON_SPEAKER_FRONT_RIGHT afOutputMatrix[1]
#define MELON_SPEAKER_FRONT_CENTER afOutputMatrix[2]
#define MELON_SPEAKER_LFE afOutputMatrix[3]
#define MELON_SPEAKER_BACK_LEFT afOutputMatrix[4]
#define MELON_SPEAKER_BACK_RIGHT afOutputMatrix[5]
#define MELON_SPEAKER_SURROUND_LEFT afOutputMatrix[6]
#define MELON_SPEAKER_SURROUND_RIGHT afOutputMatrix[7]

class CSourceVoice3D :
	public CSourceVoice
{
public:
	CSourceVoice3D(IXAudio2SourceVoice * pSourceVoice) : CSourceVoice(pSourceVoice)
	{
		m_position = CMelonVector3D(0.f, 0.f, 0.f);
		m_velocity = CMelonVector3D(0.f, 0.f, 0.f);
		m_rotation = CMelonMatrix3();
	};
	~CSourceVoice3D();

	// 1.f means full, 0 means silence
	void SetVolume(float fVolume) override;
	void SetSampleRate(unsigned int sampleRate) override;


	void UpdatePosition(float x, float y, float z) override;
	void UpdateVelocity(float x, float y, float z) override;
	void UpdateRotation(float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22) override;

	void Tick() override;

	void SetPanParameters(SPanParameters & parameters) override;
	void SetFalloffParameters(SFalloffParameters & parameters) override;
	void SetDopplerEffectParameters(SDopplerEffectParameters & parameters) override;

protected:
	CMelonMatrix3 m_rotation;
	CMelonVector3D m_position;
	CMelonVector3D m_velocity;

	UINT32 m_uiBaseSampleRate = 44100;
	float m_fBaseVolume = 1.f;
	float m_fVolume = 1.f;
	float m_fFalloffRadius = 10.f;
	float m_fMaxFalloff = 0.8f;

	float m_fCenterSpeakerPanMin = 0.f;
	float m_fFrontSpeakerAngles = M_PI_4; // 30 degrees would be 0.523599f;
	float m_fSurroundSpeakerAngles = M_PI_2;
	float m_fBackSpeakerAngles = 1.91986f; // 110 degrees
	float m_fVerticalPanAngle = M_PI_4;
	float m_fVerticalPanPowerMin = 0.1f;
	
	float m_fSpeedOfSound = DEFAULT_SPEED_OF_SOUND;

	bool m_bFalloffEnabled = false;
	bool m_bDopplerEffectEnabled = true;
	bool m_bPanEnabled = true;

	bool m_bVerticalPanEnabled = false;

	bool m_bVoiceMuted = false;

	void DopplerEffectTick();
	void PanTick();
};

