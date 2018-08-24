#pragma once
#include "SourceVoice.h"
#include "Listener.h"

#define DEFAULT_SPEED_OF_SOUND 343
#define DEFAULT_SAMPLE_RATE 44100

#define MELON_SPEAKER_FRONT_LEFT afOutputMatrix[0]
#define MELON_SPEAKER_FRONT_RIGHT afOutputMatrix[1]
#define MELON_SPEAKER_FRONT_CENTER afOutputMatrix[2]
#define MELON_SPEAKER_LFE afOutputMatrix[3]
#define MELON_SPEAKER_BACK_LEFT afOutputMatrix[4]
#define MELON_SPEAKER_BACK_RIGHT afOutputMatrix[5]
#define MELON_SPEAKER_SURROUND_LEFT afOutputMatrix[6]
#define MELON_SPEAKER_SURROUND_RIGHT afOutputMatrix[7]

struct SFalloffParameter {
	float fFalloffRadius = 10.f;
	float fMaxFalloff = 0.4f;
};

struct SPanParameter {
	float fCenterSpeakerPanMin = 0.f; 
	float fFrontSpeakerAngles = M_PI_4; // 30 degrees would be 0.523599f;
	float fSurroundSpeakerAngles = M_PI_2;
	float fBackSpeakerAngles = 1.91986f; // 110 degrees
	float fVerticalPanAngle = M_PI_4;
	float fVerticalPanPowerMin = 0.1f;
	bool bVerticalPanEnabled = false;
};

struct SSourceVoice3DParameter {
	SFalloffParameter falloffParameter;
	SPanParameter panParameter;
	float fDopplerEffectSpeedOfSound = DEFAULT_SPEED_OF_SOUND;
	float fBaseVolume = 1.f;
	UINT32 uiBaseSampleRate = DEFAULT_SAMPLE_RATE;
	bool bFalloffEnabled = false;
	bool bDopplerEffectEnabled = true;
	bool bPanEnabled = true;
};


class CSourceVoice3D :
	public CSourceVoice
{
public:
	CSourceVoice3D(IXAudio2 &pXAudio2, const char * stFileName, bool bLoopsInfinitley = false, XAUDIO2_VOICE_SENDS * SFXSendList = nullptr) : CSourceVoice(pXAudio2, stFileName, bLoopsInfinitley, SFXSendList) 
	{
		m_position = CMelonVector3D(0.f, 0.f, 0.f);
		m_velocity = CMelonVector3D(0.f, 0.f, 0.f);
		m_orientation = CMelonMatrix3();
	};
	~CSourceVoice3D();

	// 1.f means full, 0 means silence
	void SetVolume(float fVolume) override;
	// 1.f means full, 0 means silence
	float GetVolume() override;


	void UpdatePosition(float x, float y, float z);
	void UpdateVelocity(float x, float y, float z);

	const CMelonVector3D & GetPosition() const;
	const CMelonVector3D & GetVelocity() const;
	const CMelonMatrix3 & GetOrientation() const;

	void Tick();

protected:
	CMelonMatrix3 m_orientation;
	CMelonVector3D m_position;
	CMelonVector3D m_velocity;

	UINT32 m_uiBaseSampleRate = 44100;
	float m_fBaseVolume = 1.f;
	float m_fVolume = 1.f;
	float m_fFalloffRadius = 10.f;
	float m_fMaxFalloff = 0.8f;

	bool m_bFalloffEnabled = false;
	bool m_bDopplerEffectEnabled = true;
	bool m_bPanEnabled = true;

	bool m_bVoiceMuted = false;

	void DopplerEffectTick();
	void PanTick();
};

