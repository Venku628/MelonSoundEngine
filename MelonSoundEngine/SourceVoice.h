#pragma once

#include "Voice.h"

// TODO: include static in class if possible
template <typename T> WORD ChannelsSpecifiedInMask(T x)
{
	WORD bitCount = 0;
	while (x) { ++bitCount; x &= (x - 1); }
	return bitCount;
}

class CSourceVoice :
	public CVoice
{
public:
	// CSourceVoice(IXAudio2 &pXAudio2, const char * stFileName, bool bLoopsInfinitley = false, XAUDIO2_VOICE_SENDS * SFXSendList = nullptr);
	CSourceVoice(IXAudio2SourceVoice * pSourceVoice);
	~CSourceVoice();

	void StartPlayback() override;
	void StopPlayback() override;

	void SetEffectChain(const XAUDIO2_EFFECT_CHAIN * pEffectChain) override;
	void SetEffectParameters(unsigned int uiEffectIndex, const void * pParameters, unsigned int uiParametersByteSize) override;

	// 1.f means full, 0 means silence
	void SetVolume(float fVolume) override;
	void SetSampleRate(unsigned int sampleRate) override;


	IXAudio2SourceVoice * GetSourceVoice() const;

	// These functions do nothing in this Voice, they should get a debug call in the future. This disrupts the class hierarchy, but speeds up the handle calls.
	const XAUDIO2_VOICE_SENDS * GetSFXSendList() { return nullptr; };

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

protected:
	IXAudio2SourceVoice * m_pSourceVoice;
	IXAudio2 * m_pXAudio2;
	HRESULT CreateSourceVoice(const char * stFileName, XAUDIO2_VOICE_SENDS * SFXSendList = nullptr);

	// Voicepool
	static uint32_t GetDefaultChannelMask(int channels);
	static uint32_t GetFormatTag(const WAVEFORMATEX* wfx);
	static bool IsValid(_In_ const WAVEFORMATEX* wfx);
	static unsigned int makeVoiceKey(_In_ const WAVEFORMATEX* wfx);

	unsigned int m_uiVoiceKey;
	bool m_bLoopInfinite = false;

	static HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset);
};

