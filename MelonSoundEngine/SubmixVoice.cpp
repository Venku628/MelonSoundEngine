#include "SubmixVoice.h"



CSubmixVoice::CSubmixVoice(IXAudio2 &pXAudio2)
{
	m_SFXSend = { 0, m_pSubmixVoice };
	m_SFXSendList = { 1, &m_SFXSend };

	// TODO: think of a better way to do this
	m_pXAudio2 = &pXAudio2;

	

	CreateSubmixVoice();
}


CSubmixVoice::~CSubmixVoice()
{
	
	delete m_pSubmixVoice;
}

CSourceVoice * CSubmixVoice::CreateSourceVoice(const char * stFileName)
{
	return nullptr;
}

CSourceVoice3D * CSubmixVoice::CreateSourceVoice3D(const char * stFileName)
{
	return nullptr;
}

CSubmixVoice * CSubmixVoice::CreateSubSubmixVoice()
{
	return nullptr;
}


const XAUDIO2_VOICE_SENDS& CSubmixVoice::GetSFXSendList() const
{
	return m_SFXSendList;
}

// TODO: Do I need to handle HRESULTs?
HRESULT CSubmixVoice::CreateSubmixVoice()
{
	// TODO: what to do with the channel number?
	return m_pXAudio2->CreateSubmixVoice(&m_pSubmixVoice, 1, 44100, 0, 0, 0, 0);;
}
