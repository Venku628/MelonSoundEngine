// MelonSoundEngineTest.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//
#pragma once

#include <Windows.h>
#include <xaudio2.h>
#include <comdef.h>

#include "MelonSoundEngine.h"

int main()
{

	CMelonSoundEngine::GetInstance();
	DWORD dwChannelMask;
	CMelonSoundEngine::GetInstance().GetMasteringVoice()->GetChannelMask(&dwChannelMask);
	CListener::GetInstance().SetChannelMask(dwChannelMask);
	
	const char * stFileName = "..\\pan.wav";
	const char * stFileName2 = "..\\error.wav";
	
	unsigned int submix1 = CMelonSoundEngine::GetInstance().CreateSubmixVoice();
	unsigned int sample = CMelonSoundEngine::GetInstance().CreateSourceVoice3D(stFileName);
	unsigned int sample2 = CMelonSoundEngine::GetInstance().CreateSourceVoice(stFileName2, false, submix1);

	// IXAudio2MasteringVoice * master = CMelonSoundEngine::GetInstance().GetMasteringVoice();
	// IXAudio2 * xaudio = CMelonSoundEngine::GetInstance().GetXAudio2();

	CMelonSoundEngine::GetInstance().UpdatePostion(sample, 0.f, 0.f, 0.1f);

	// submix1->SetVolume(0.1f);
	CMelonSoundEngine::GetInstance().Tick(sample);
	CMelonSoundEngine::GetInstance().StartPlayback(sample);
	CMelonSoundEngine::GetInstance().StartPlayback(sample2);
	
	

	while(true)
	CMelonSoundEngine::GetInstance().Update();

	// Kill the Sounds before this runs out or Windows gets very sad, no longer a problem once pointers etc are handled properly
	Sleep(1000000);
    return 0;
}

