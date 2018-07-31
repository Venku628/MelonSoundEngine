#pragma once

#include "SourceVoice.h"
#include "SubmixVoice.h"
#include <unordered_map>
#include "MelonMatrix4.h"

class CMelonSoundEngine
{
public:
	// classic lazy singleton
	static CMelonSoundEngine& GetInstance();

	CMelonSoundEngine(CMelonSoundEngine const&) = delete;
	void operator=(CMelonSoundEngine const&) = delete;

	// temporary Creates until Voice Pool and Handles implemented
	CSourceVoice * CreateSourceVoice(const char * stFileName);
	// missing inital 3D data until implemented
	CSourceVoice3D * CreateSourceVoice3D(const char * stFileName);
	CSubmixVoice * CreateSubmixVoice();

	void ClearVoicepool();

	void AssignVoice();

private:
	IXAudio2MasteringVoice * m_pMasteringVoice;
	IXAudio2 * m_pXAudio2;

	std::unordered_multimap<unsigned int, IXAudio2SourceVoice*> m_voicepool;

	unsigned int m_maxVoiceInstances = 20, m_voiceInstances = 0;

	CMelonSoundEngine();
	~CMelonSoundEngine();
};

