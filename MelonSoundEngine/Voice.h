#pragma once

#include "XAudio2Basics.h"



struct SFalloffParameters {
	float fFalloffRadius = 10.f;
	float fMaxFalloff = 0.4f;
	bool bFalloffEnabled = false;
};

struct SPanParameters {
	float fCenterSpeakerPanMin = 0.f;
	float fFrontSpeakerAngles = M_PI_4; // 30 degrees would be 0.523599f;
	float fSurroundSpeakerAngles = M_PI_2;
	float fBackSpeakerAngles = 1.91986f; // 110 degrees
	float fVerticalPanAngle = M_PI_4;
	float fVerticalPanPowerMin = 0.1f;
	bool bVerticalPanEnabled = false;
	bool bPanEnabled = true;
};

struct SDopplerEffectParameters {
	float fDopplerEffectSpeedOfSound = DEFAULT_SPEED_OF_SOUND;
	bool bDopplerEffectEnbabled = true;
};

class CVoice
{
public:
	// general methods

	// 1.f means full, 0 means silence
	virtual void SetVolume(float fVolume) = 0;
	// 1.f means full, 0 means silence
	// virtual float GetVolume() = 0;
	virtual void SetEffectChain(const XAUDIO2_EFFECT_CHAIN * pEffectChain) = 0;
	virtual void SetEffectParameters(unsigned int uiEffectIndex, const void * pParameters, unsigned int uiParametersByteSize) = 0;

	// SubmixVoice method
	virtual const XAUDIO2_VOICE_SENDS * GetSFXSendList() = 0;

	// SourceVoice methods (and therefore also SourceVoice3D)

	virtual void SetSampleRate(unsigned int sampleRate) = 0;
	virtual void StartPlayback() = 0;
	virtual void StopPlayback() = 0;

	// SourceVoice3D methods

	virtual void UpdatePosition(float x, float y, float z) = 0;
	virtual void UpdateVelocity(float x, float y, float z) = 0;
	virtual void UpdateRotation(
		float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22) = 0;
	virtual void Tick() = 0;
	virtual void SetPanParameters(SPanParameters & parameters) = 0;
	virtual void SetFalloffParameters(SFalloffParameters & parameters) = 0;
	virtual void SetDopplerEffectParameters(SDopplerEffectParameters & parameters) = 0;
};

