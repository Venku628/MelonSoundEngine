#include "MelonSoundEngine.h"



CMelonSoundEngine::CMelonSoundEngine()
{
	XAudio2Create(&m_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
}


CMelonSoundEngine::~CMelonSoundEngine()
{
	delete m_pMasteringVoice;
	delete m_pXAudio2;
}

CMelonSoundEngine & CMelonSoundEngine::GetInstance()
{
	static CMelonSoundEngine instance;
	return instance;
}

CSourceVoice * CMelonSoundEngine::CreateSourceVoice(const char * stFileName)
{
	return new CSourceVoice(* m_pXAudio2, stFileName);
}

CSourceVoice3D * CMelonSoundEngine::CreateSourceVoice3D(const char * stFileName)
{
	return nullptr;
}

CSubmixVoice * CMelonSoundEngine::CreateSubmixVoice()
{
	return new CSubmixVoice(*m_pXAudio2);
}

void CMelonSoundEngine::ClearVoicepool()
{
	for (auto it = m_voicepool.begin(); it != m_voicepool.end(); ++it)
	{
		if(it->second != nullptr)
			it->second->DestroyVoice();
	}
	m_voicepool.clear();
}

void CMelonSoundEngine::AssignVoice()
{
	// unsigned int voiceKey = 0;

}
