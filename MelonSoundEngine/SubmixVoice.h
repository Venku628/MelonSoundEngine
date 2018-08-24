#pragma once
#include "Voice.h"
#include "SourceVoice3D.h"

class CSubmixVoice :
	public CVoice
{
public:
	CSubmixVoice(IXAudio2 &pXAudio2);
	~CSubmixVoice();

	// temporary Creates until Voice Pool and Handles implemented
	CSourceVoice * CreateSourceVoice(const char * stFileName);
	// missing inital 3D data until implemented
	CSourceVoice3D * CreateSourceVoice3D(const char * stFileName);
	CSubmixVoice * CreateSubSubmixVoice();

	void SetVolume(float fVolume) override;
	float GetVolume() override;

	void SetSampleRate(unsigned int sampleRate) override;

	void StartPlayback() override;
	void StopPlayback() override;

	const XAUDIO2_VOICE_SENDS& GetSFXSendList() const;
	IXAudio2SubmixVoice * GetSubmixVoice() const;

private:
	IXAudio2SubmixVoice * m_pSubmixVoice;
	IXAudio2 * m_pXAudio2;

	XAUDIO2_SEND_DESCRIPTOR m_SFXSend;
	XAUDIO2_VOICE_SENDS m_SFXSendList;

	HRESULT CreateSubmixVoice();
};

