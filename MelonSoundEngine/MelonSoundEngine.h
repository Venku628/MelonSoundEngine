#pragma once

#include "SourceVoice.h"
#include "SubmixVoice.h"

class CMelonSoundEngine
{
public:
	CMelonSoundEngine();
	~CMelonSoundEngine();

private:
	IXAudio2MasteringVoice * m_pMasteringVoice;
	IXAudio2 * m_pXAudio2;
};

