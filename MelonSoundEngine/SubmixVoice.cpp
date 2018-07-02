#include "SubmixVoice.h"



CSubmixVoice::CSubmixVoice(IXAudio2 &pXAudio2)
{
	// TODO: think of a better way to do this
	m_pXAudio2 = &pXAudio2;

	CreateSubmixVoice();

	m_SFXSend = { 0, m_pSubmixVoice };
	m_SFXSendList = { 1, &m_SFXSend };

	
	
}


CSubmixVoice::~CSubmixVoice()
{
	
	delete m_pSubmixVoice;
}

CSourceVoice * CSubmixVoice::CreateSourceVoice(const char * stFileName)
{
	return new CSourceVoice(*m_pXAudio2, stFileName, &m_SFXSendList);
}

CSourceVoice3D * CSubmixVoice::CreateSourceVoice3D(const char * stFileName)
{
	return new CSourceVoice3D(*m_pXAudio2, stFileName, &m_SFXSendList);
}

CSubmixVoice * CSubmixVoice::CreateSubSubmixVoice()
{
	return nullptr;
}

void CSubmixVoice::SetVolume(float fVolume)
{
	m_pSubmixVoice->SetVolume(fVolume);
}


const XAUDIO2_VOICE_SENDS& CSubmixVoice::GetSFXSendList() const
{
	return m_SFXSendList;
}

// TODO: Do I need to handle HRESULTs?
HRESULT CSubmixVoice::CreateSubmixVoice()
{
	// TODO: what to do with the channel number?
	return m_pXAudio2->CreateSubmixVoice(&m_pSubmixVoice, 1, 44100, 0, 0, 0, 0);
}
