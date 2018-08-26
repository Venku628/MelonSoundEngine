#pragma once

#include <xapobase.h>
#include <assert.h>
#include <string>
#include <deque>

#define TEST_SAMPLE_RATE 44100
#define TEST_DELAY_IN_S 0.3f

// This class is not finished yet, the reverb calculation seems to be wrong 
// Comb Reverb Filter
class __declspec(uuid("{A4945B8A-EB14-4c96-8067-DF726B528091}"))
	CReverbXAPO : public CXAPOBase
{
public:
	
	CReverbXAPO(const XAPO_REGISTRATION_PROPERTIES *pRegProperties) : CXAPOBase(pRegProperties) {};

	short m_uChannels;
	short m_uBytesPerSample;

	STDMETHOD_(void, Process)(UINT32 InputProcessParameterCount,
		const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters,
		UINT32 OutputProcessParameterCount,
		XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters,
		BOOL IsEnabled);
	
private:
	std::deque<FLOAT32> m_fReverbBuffer;
	// unsigned int m_uiReverbBufferSize;
};

