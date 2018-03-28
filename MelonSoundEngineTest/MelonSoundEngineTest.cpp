// MelonSoundEngineTest.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//
#pragma once

#include <Windows.h>
#include <xaudio2.h>
#include <comdef.h>

#include "SourceVoice.h"

int main()
{

	CoInitialize(nullptr);

	IXAudio2* pXAudio2 = NULL;
	HRESULT hr;
	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
		return hr;

	IXAudio2MasteringVoice* pMasterVoice = NULL;
	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
		return hr;

	pMasterVoice->SetVolume(0.05);

	const char * stFileName = "..\\sample.wav";
	const char * stFileName2 = "..\\error.wav";
	CSourceVoice sample(*pXAudio2, stFileName);
	CSourceVoice sample2(*pXAudio2, stFileName2);

	sample.StartPlayback();
	sample2.StartPlayback();


	Sleep(10000);
    return 0;
}

