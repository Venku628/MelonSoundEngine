#include "SubmixVoice.h"



CSubmixVoice::CSubmixVoice(IXAudio2SubmixVoice * pSubmixVoice)
{
	m_pSubmixVoice = pSubmixVoice;
	m_SFXSend = { 0, m_pSubmixVoice };
	m_SFXSendList = { 1, &m_SFXSend };
}


CSubmixVoice::~CSubmixVoice()
{
	delete m_pSubmixVoice;
}

void CSubmixVoice::SetVolume(float fVolume)
{
	m_pSubmixVoice->SetVolume(fVolume);
}

void CSubmixVoice::SetEffectChain(const XAUDIO2_EFFECT_CHAIN * pEffectChain)
{
	m_pSubmixVoice->SetEffectChain(pEffectChain);
}

void CSubmixVoice::SetEffectParameters(unsigned int uiEffectIndex, const void * pParameters, unsigned int uiParametersByteSize)
{
	m_pSubmixVoice->SetEffectParameters(uiEffectIndex, pParameters, uiParametersByteSize);
}

void CSubmixVoice::SetOutputMatrix(unsigned int uiSourceChannels, unsigned int uiDestinationChannels, const float * pfLevelMatrix)
{
	m_pSubmixVoice->SetOutputMatrix(nullptr, uiSourceChannels, uiDestinationChannels, pfLevelMatrix);
}

const XAUDIO2_VOICE_SENDS * CSubmixVoice::GetSFXSendList()
{
	return &m_SFXSendList;
}

IXAudio2SubmixVoice * CSubmixVoice::GetSubmixVoice() const
{
	return m_pSubmixVoice;
}

HRESULT CSubmixVoice::CreateSubmixVoice()
{
	// TODO: what to do with the channel number?
	return m_pXAudio2->CreateSubmixVoice(&m_pSubmixVoice, 1, 44100, 0, 0, 0, 0);
}
