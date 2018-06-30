// MelonSoundEngineTest.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//
#pragma once

#include <Windows.h>
#include <xaudio2.h>
#include <comdef.h>

#include "MelonSoundEngine.h"

int main()
{
	CoInitialize(nullptr);
	
	const char * stFileName = "..\\sample.wav";
	const char * stFileName2 = "..\\error.wav";
	// CSourceVoice * sample = CMelonSoundEngine::GetInstance().CreateSourceVoice(stFileName);
	// CSourceVoice * sample2 = CMelonSoundEngine::GetInstance().CreateSourceVoice(stFileName2);
	CSubmixVoice * submix1 = CMelonSoundEngine::GetInstance().CreateSubmixVoice();
	CSourceVoice * sample = submix1->CreateSourceVoice(stFileName);
	CSourceVoice * sample2 = submix1->CreateSourceVoice(stFileName2);

	submix1->SetVolume(0.1f);

	sample->StartPlayback();
	sample2->StartPlayback();

	// Kill the Sounds before this runs out or Windows gets very sad, no longer a problem once pointers etc are handled properly
	Sleep(1000000);
    return 0;
}

