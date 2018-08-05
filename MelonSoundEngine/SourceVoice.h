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
	CSourceVoice(IXAudio2 &pXAudio2, const char * stFileName);
	CSourceVoice(IXAudio2 &pXAudio2, const char * stFileName, XAUDIO2_VOICE_SENDS * SFXSendList);
	~CSourceVoice();

	HRESULT StartPlayback();
	HRESULT StopPlayback();

	void SetVolume(float fVolume) override;

	IXAudio2SourceVoice * GetSourceVoice() const;

protected:
	
	IXAudio2SourceVoice * m_pSourceVoice;
	IXAudio2 * m_pXAudio2;
	HRESULT CreateSourceVoice(const char * stFileName);
	HRESULT CreateSourceVoice(const char * stFileName, XAUDIO2_VOICE_SENDS * SFXSendList);

	// Voicepool
	static uint32_t GetDefaultChannelMask(int channels);
	static uint32_t GetFormatTag(const WAVEFORMATEX* wfx);
	static bool IsValid(_In_ const WAVEFORMATEX* wfx);
	static unsigned int makeVoiceKey(_In_ const WAVEFORMATEX* wfx);

	unsigned int m_uiVoiceKey;
	
	// TODO: make inheritable method to create XAudio2 Source so 3DSourceVoice can use it too

	// TODO: Microsoft example, change notation if unchanged
	static HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset);
};

