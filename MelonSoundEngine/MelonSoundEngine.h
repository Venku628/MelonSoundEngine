#pragma once

#include "SourceVoice.h"
#include "SubmixVoice.h"

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

private:
	IXAudio2MasteringVoice * m_pMasteringVoice;
	IXAudio2 * m_pXAudio2;

	CMelonSoundEngine();
	~CMelonSoundEngine();
};

