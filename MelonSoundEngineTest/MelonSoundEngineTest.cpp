// MelonSoundEngineTest.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//
#pragma once


#include "MelonSoundEngineAPI.h"

using namespace MelonSoundEngine;

int main()
{	
	const char * stFileName = "..\\pan.wav";
	const char * stFileName2 = "..\\error.wav";
	
	unsigned int submix1 = MelonSoundEngineAPI::CreateSubmixVoice();
	unsigned int sample = MelonSoundEngineAPI::CreateSourceVoice3D(stFileName);
	unsigned int sample2 = MelonSoundEngineAPI::CreateSourceVoice(stFileName2, false, submix1);

	MelonSoundEngineAPI::UpdatePostion(sample, 0.f, 0.f, 0.1f);
	MelonSoundEngineAPI::Tick(sample);
	MelonSoundEngineAPI::StartPlayback(sample);
	MelonSoundEngineAPI::StartPlayback(sample2);
	
	Sleep(1000000);
    return 0;
}

