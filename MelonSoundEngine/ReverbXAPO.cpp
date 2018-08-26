#include "ReverbXAPO.h"

// modified version of a DirectX SDK Sample
STDMETHODIMP_(void) CReverbXAPO::Process(UINT32 InputProcessParameterCount, const XAPO_PROCESS_BUFFER_PARAMETERS * pInputProcessParameters, 
	UINT32 OutputProcessParameterCount, XAPO_PROCESS_BUFFER_PARAMETERS * pOutputProcessParameters, BOOL IsEnabled)
{
	assert(IsLocked());
	assert(InputProcessParameterCount == 1);
	assert(OutputProcessParameterCount == 1);
	assert(NULL != pInputProcessParameters);
	assert(NULL != pOutputProcessParameters);


	XAPO_BUFFER_FLAGS inFlags = pInputProcessParameters[0].BufferFlags;
	XAPO_BUFFER_FLAGS outFlags = pOutputProcessParameters[0].BufferFlags;

	// assert buffer flags are legitimate
	assert(inFlags == XAPO_BUFFER_VALID || inFlags == XAPO_BUFFER_SILENT);
	assert(outFlags == XAPO_BUFFER_VALID || outFlags == XAPO_BUFFER_SILENT);

	// check input APO_BUFFER_FLAGS
	switch (inFlags)
	{
	case XAPO_BUFFER_VALID:
	{
		void* pvSrc = pInputProcessParameters[0].pBuffer;
		assert(pvSrc != NULL);

		void* pvDst = pOutputProcessParameters[0].pBuffer;
		assert(pvDst != NULL);

		FLOAT32* __restrict pData = (FLOAT32* __restrict)pInputProcessParameters[0].pBuffer;

		// this is where things happen
		int uiReverbStartSample = TEST_DELAY_IN_S * TEST_SAMPLE_RATE;

		if (uiReverbStartSample + pInputProcessParameters[0].ValidFrameCount < m_fReverbBuffer.size())
		{
			for (UINT32 i = 0; i < pInputProcessParameters[0].ValidFrameCount * 1; ++i) // * 1 this is channel dependend as far as i know, the compiler should fix this
			{
				// Something here is wrong
				pData[i] = ((m_fReverbBuffer[uiReverbStartSample + i] * 0.8f) * 0.5f) + (pData[i] * 0.5f);

				m_fReverbBuffer.emplace_front(pData[i]);
			}
		}
		else
		{
			for (UINT32 i = 0; i < pInputProcessParameters[0].ValidFrameCount * 1; ++i)
			{
				m_fReverbBuffer.emplace_front(pData[i]);
			}
		}

		// clear memory so the deque does not grow indefinitley
		if (m_fReverbBuffer.size() > TEST_SAMPLE_RATE)
		{
			m_fReverbBuffer.erase(m_fReverbBuffer.cbegin() + TEST_SAMPLE_RATE, m_fReverbBuffer.cend());
		}

		memcpy(pvDst, pvSrc, pInputProcessParameters[0].ValidFrameCount * m_uChannels * m_uBytesPerSample);
		break;
	}
	case XAPO_BUFFER_SILENT:
	{
		// All that needs to be done for this case is setting the
		// output buffer flag to XAPO_BUFFER_SILENT which is done below.
		break;
	}
	}

	// set destination valid frame count, and buffer flags
	pOutputProcessParameters[0].ValidFrameCount = pInputProcessParameters[0].ValidFrameCount; // set destination frame count same as source
	pOutputProcessParameters[0].BufferFlags = pInputProcessParameters[0].BufferFlags;     // set destination buffer flags same as source

}


