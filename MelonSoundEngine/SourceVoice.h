#pragma once

#include "Voice.h"

class CSourceVoice :
	public CVoice
{
public:
	CSourceVoice(IXAudio2 &pXAudio2, const char * stFileName);
	~CSourceVoice();

	HRESULT StartPlayback();
	HRESULT StopPlayback();

private:
	IXAudio2SourceVoice * m_pSourceVoice;
	IXAudio2 * m_pXAudio2;
	HRESULT CreateSourceVoice(const char * stFileName);
	HRESULT CreateSourceVoice(const char * stFileName, XAUDIO2_VOICE_SENDS SFXSendList);
	
	// TODO: Microsoft example, change notation if unchanged
	static HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset);
};

