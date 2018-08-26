#pragma once
#include "Voice.h"
#include "SourceVoice3D.h"

class CSubmixVoice :
	public CVoice
{
public:
	CSubmixVoice(IXAudio2SubmixVoice * pSubmixVoice);
	~CSubmixVoice();

	void SetVolume(float fVolume) override;
	void SetEffectChain(const XAUDIO2_EFFECT_CHAIN * pEffectChain) override;
	void SetEffectParameters(unsigned int uiEffectIndex, const void * pParameters, unsigned int uiParametersByteSize) override;

	const XAUDIO2_VOICE_SENDS * GetSFXSendList() override;
	IXAudio2SubmixVoice * GetSubmixVoice() const;

	// These functions do nothing in this Voice, they should get a debug call in the future. This disrupts the class hierarchy, but speeds up the handle calls.
	void StartPlayback() override {};
	void StopPlayback() override {};
	void SetSampleRate(unsigned int sampleRate) override {};

	void UpdatePosition(float x, float y, float z) override {};
	void UpdateVelocity(float x, float y, float z) override {};
	void UpdateRotation(
		float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22) override {};
	void Tick() override {};
	void SetPanParameters(SPanParameters & parameters) override {};
	void SetFalloffParameters(SFalloffParameters & parameters) override {};
	void SetDopplerEffectParameters(SDopplerEffectParameters & parameters) override {};

private:
	IXAudio2SubmixVoice * m_pSubmixVoice;
	IXAudio2 * m_pXAudio2;

	XAUDIO2_SEND_DESCRIPTOR m_SFXSend;
	XAUDIO2_VOICE_SENDS m_SFXSendList;

	HRESULT CreateSubmixVoice();
};

