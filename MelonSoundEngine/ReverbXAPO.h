#pragma once

#include <xapobase.h>
#include <assert.h>
#include <string>
#include <deque>

#define TEST_SAMPLE_RATE 44100
#define TEST_DELAY_IN_S 0.3f

class __declspec(uuid("{A4945B8A-EB14-4c96-8067-DF726B528091}"))
	CustomXAPO : public CXAPOBase
{
public:
	
	CustomXAPO(const XAPO_REGISTRATION_PROPERTIES *pRegProperties) : CXAPOBase(pRegProperties) {};

	short m_uChannels;
	short m_uBytesPerSample;

	/*
	STDMETHOD(LockForProcess) (UINT32 InputLockedParameterCount,
		const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters,
		UINT32 OutputLockedParameterCount,
		const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters);
	*/

	STDMETHOD_(void, Process)(UINT32 InputProcessParameterCount,
		const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters,
		UINT32 OutputProcessParameterCount,
		XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters,
		BOOL IsEnabled);
	
private:
	std::deque<FLOAT32> m_fReverbBuffer;
	// unsigned int m_uiReverbBufferSize;
};

