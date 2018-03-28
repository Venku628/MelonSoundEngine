#pragma once
#include "XAudio2Basics.h"

class CSubmixVoice
{
public:
	CSubmixVoice();
	~CSubmixVoice();

private:
	IXAudio2SubmixVoice * m_pSubmixVoice;
	IXAudio2 * m_pXAudio2;
};

