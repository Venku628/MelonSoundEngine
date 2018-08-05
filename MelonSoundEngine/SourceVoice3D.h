#pragma once
#include "SourceVoice.h"
#include "Listener.h"

#define DEBUG_SPEED_OF_SOUND 343

#define MELON_SPEAKER_FRONT_LEFT afOutputMatrix[0]
#define MELON_SPEAKER_FRONT_RIGHT afOutputMatrix[1]
#define MELON_SPEAKER_FRONT_CENTER afOutputMatrix[2]
#define MELON_SPEAKER_LFE afOutputMatrix[3]
#define MELON_SPEAKER_BACK_LEFT afOutputMatrix[4]
#define MELON_SPEAKER_BACK_RIGHT afOutputMatrix[5]
#define MELON_SPEAKER_SURROUND_LEFT afOutputMatrix[6]
#define MELON_SPEAKER_SURROUND_RIGHT afOutputMatrix[7]

struct SPanParameter {
	float CenterSpeakerPanMin = 0.f; 
	float FrontSpeakerAngles = 0.523599f; // 30 degrees
	float SurroundSpeakerAngles = M_PI_2;
	float BackSpeakerAngles = 1.91986f; // 110 degrees
	float VerticalPanAngle = M_PI_4;
	float VerticalPanPowerMin = 0.1f;
	float VerticalPanPowerMax = 1.f;
	bool VerticalPanEnabled = true;
};

class CSourceVoice3D :
	public CSourceVoice
{
public:
	CSourceVoice3D(IXAudio2 &pXAudio2, const char * stFileName) : CSourceVoice(pXAudio2, stFileName) 
	{
		m_position = CMelonVector3D(0.f, 0.f, 0.f);
		m_velocity = CMelonVector3D(0.f, 0.f, 0.f);
		m_orientation = CMelonMatrix3();
	};
	CSourceVoice3D(IXAudio2 &pXAudio2, const char * stFileName, XAUDIO2_VOICE_SENDS * SFXSendList) : CSourceVoice(pXAudio2, stFileName, SFXSendList) 
	{
		m_position = CMelonVector3D(0.f, 0.f, 0.f);
		m_velocity = CMelonVector3D(0.f, 0.f, 0.f);
		m_orientation = CMelonMatrix3();
	};
	~CSourceVoice3D();

	void SetVolume(float fVolume) override;

	void UpdatePosition(float x, float y, float z);
	void UpdateVelocity(float x, float y, float z);

	const CMelonVector3D & GetPosition() const;
	const CMelonVector3D & GetVelocity() const;
	const CMelonMatrix3 & GetOrientation() const;

	void Tick();

private:
	CMelonVector3D m_position;
	CMelonVector3D m_velocity;
	CMelonMatrix3 m_orientation;

	// TODO: make a Parameter struct out of these variables
	// Default for Debugging
	UINT32 m_uiBaseSampleRate = 44100;
	float m_fVolume;
	float m_fFalloffRadius = 10.f;
	float m_fMaxFalloff = 0.8f;

	// Default for Debugging
	bool m_bFalloffEnabled = false;
	bool m_bDopplerEffectEnabled = true;

	bool m_bVoiceMuted = false;

	void DopplerEffectTick();
	void PanTick();
};

