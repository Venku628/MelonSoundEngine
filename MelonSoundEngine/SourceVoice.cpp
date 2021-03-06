#include "SourceVoice.h"

CSourceVoice::CSourceVoice(IXAudio2SourceVoice * pSourceVoice)
{
	m_pSourceVoice = pSourceVoice;
}

CSourceVoice::~CSourceVoice()
{
	// don�t destory voice
}

HRESULT CSourceVoice::CreateSourceVoice(const char * stFileName, XAUDIO2_VOICE_SENDS * SFXSendList)
{
	// Declare Buffer structures

	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };

	// Open audio file
	HANDLE hFile = CreateFileA(
		stFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return HRESULT_FROM_WIN32(GetLastError());

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	// Locate RIFF chunk in audio file and check file type
	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD dwFiletype;
	ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (dwFiletype != fourccWAVE)
		return S_FALSE;

	// Locate fmt chunk and copy contents into WAVEFORMATEXTENSIBLE structure
	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

	m_uiVoiceKey = makeVoiceKey(&wfx.Format);

	// Locate data chunk and read content into buffer
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE * pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	// populate XAUDIO2_BUFFER
	buffer.AudioBytes = dwChunkSize;  //buffer containing audio data
	buffer.pAudioData = pDataBuffer;  //size of the audio buffer in bytes
	buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
	if (m_bLoopInfinite)
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// Create XAudio2 source voice
	HRESULT hr;
	if (FAILED(hr = m_pXAudio2->CreateSourceVoice(&m_pSourceVoice,
		(WAVEFORMATEX*)&wfx,
		0U,
		XAUDIO2_DEFAULT_FREQ_RATIO,
		NULL,
		SFXSendList
	)))
		return hr;

	// submit XAUDIO2_BUFFER to source voice
	if (FAILED(hr = m_pSourceVoice->SubmitSourceBuffer(&buffer)))
		return hr;

	return S_OK;
}

void CSourceVoice::StartPlayback()
{
	m_pSourceVoice->Start(0);
}

void CSourceVoice::StopPlayback()
{
	m_pSourceVoice->Stop(0);
}

void CSourceVoice::SetEffectChain(const XAUDIO2_EFFECT_CHAIN * pEffectChain)
{
	m_pSourceVoice->SetEffectChain(pEffectChain);
}

void CSourceVoice::SetEffectParameters(unsigned int uiEffectIndex, const void * pParameters, unsigned int uiParametersByteSize)
{
	m_pSourceVoice->SetEffectParameters(uiEffectIndex, pParameters, uiParametersByteSize);
}

void CSourceVoice::SetOutputMatrix(unsigned int uiSourceChannels, unsigned int uiDestinationChannels, const float * pfLevelMatrix)
{
	m_pSourceVoice->SetOutputMatrix(nullptr, uiSourceChannels, uiDestinationChannels, pfLevelMatrix);
}

void CSourceVoice::SetVolume(float fVolume)
{
	m_pSourceVoice->SetVolume(fVolume);
}

void CSourceVoice::SetSampleRate(unsigned int sampleRate)
{
	m_pSourceVoice->SetSourceSampleRate(sampleRate);
}

IXAudio2SourceVoice * CSourceVoice::GetSourceVoice() const
{
	return m_pSourceVoice;
}

// helper function from Microsoft examples for keygeneration
uint32_t CSourceVoice::GetDefaultChannelMask(int channels)
{
	switch (channels)
	{
	case 1: return SPEAKER_MONO;
	case 2: return SPEAKER_STEREO;
	case 3: return SPEAKER_2POINT1;
	case 4: return SPEAKER_QUAD;
	case 5: return SPEAKER_4POINT1;
	case 6: return SPEAKER_5POINT1;
	case 7: return SPEAKER_5POINT1 | SPEAKER_BACK_CENTER;
	case 8: return SPEAKER_7POINT1;
	default: return 0;
	}
}

// helper function from Microsoft examples for keygeneration
uint32_t CSourceVoice::GetFormatTag(const WAVEFORMATEX * wfx)
{
	if (wfx->wFormatTag == WAVE_FORMAT_EXTENSIBLE)
	{
		if (wfx->cbSize < (sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX)))
			return 0;

		static const GUID s_wfexBase = { 0x00000000, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };

		auto wfex = reinterpret_cast<const WAVEFORMATEXTENSIBLE*>(wfx);

		if (memcmp(reinterpret_cast<const BYTE*>(&wfex->SubFormat) + sizeof(DWORD),
			reinterpret_cast<const BYTE*>(&s_wfexBase) + sizeof(DWORD), sizeof(GUID) - sizeof(DWORD)) != 0)
		{
			return 0;
		}

		return wfex->SubFormat.Data1;
	}
	else
	{
		return wfx->wFormatTag;
	}
}

// helper function from Microsoft examples to confirm validity of a waveformatex
bool CSourceVoice::IsValid(_In_ const WAVEFORMATEX* wfx)
{
	if (!wfx)
		return false;

	if (!wfx->nChannels)
	{
		// DebugTrace("ERROR: Wave format must have at least 1 channel\n");
		return false;
	}

	if (wfx->nChannels > XAUDIO2_MAX_AUDIO_CHANNELS)
	{
		// DebugTrace("ERROR: Wave format must have less than %u channels (%u)\n", XAUDIO2_MAX_AUDIO_CHANNELS, wfx->nChannels);
		return false;
	}

	if (!wfx->nSamplesPerSec)
	{
		// DebugTrace("ERROR: Wave format cannot have a sample rate of 0\n");
		return false;
	}

	if ((wfx->nSamplesPerSec < XAUDIO2_MIN_SAMPLE_RATE)
		|| (wfx->nSamplesPerSec > XAUDIO2_MAX_SAMPLE_RATE))
	{
		// DebugTrace("ERROR: Wave format channel count must be in range %u..%u (%u)\n", XAUDIO2_MIN_SAMPLE_RATE, XAUDIO2_MAX_SAMPLE_RATE, wfx->nSamplesPerSec);
		return false;
	}

	switch (wfx->wFormatTag)
	{
	case WAVE_FORMAT_PCM:

		switch (wfx->wBitsPerSample)
		{
		case 8:
		case 16:
		case 24:
		case 32:
			break;

		default:
			// DebugTrace("ERROR: Wave format integer PCM must have 8, 16, 24, or 32 bits per sample (%u)\n", wfx->wBitsPerSample);
			return false;
		}

		if (wfx->nBlockAlign != (wfx->nChannels * wfx->wBitsPerSample / 8))
		{
			// DebugTrace("ERROR: Wave format integer PCM - nBlockAlign (%u) != nChannels (%u) * wBitsPerSample (%u) / 8\n", wfx->nBlockAlign, wfx->nChannels, wfx->wBitsPerSample);
			return false;
		}

		if (wfx->nAvgBytesPerSec != (wfx->nSamplesPerSec * wfx->nBlockAlign))
		{
			// DebugTrace("ERROR: Wave format integer PCM - nAvgBytesPerSec (%lu) != nSamplesPerSec (%lu) * nBlockAlign (%u)\n", wfx->nAvgBytesPerSec, wfx->nSamplesPerSec, wfx->nBlockAlign);
			return false;
		}

		return true;

	case WAVE_FORMAT_IEEE_FLOAT:

		if (wfx->wBitsPerSample != 32)
		{
			// DebugTrace("ERROR: Wave format float PCM must have 32-bits per sample (%u)\n", wfx->wBitsPerSample);
			return false;
		}

		if (wfx->nBlockAlign != (wfx->nChannels * wfx->wBitsPerSample / 8))
		{
			// DebugTrace("ERROR: Wave format float PCM - nBlockAlign (%u) != nChannels (%u) * wBitsPerSample (%u) / 8\n", wfx->nBlockAlign, wfx->nChannels, wfx->wBitsPerSample);
			return false;
		}

		if (wfx->nAvgBytesPerSec != (wfx->nSamplesPerSec * wfx->nBlockAlign))
		{
			// DebugTrace("ERROR: Wave format float PCM - nAvgBytesPerSec (%lu) != nSamplesPerSec (%lu) * nBlockAlign (%u)\n", wfx->nAvgBytesPerSec, wfx->nSamplesPerSec, wfx->nBlockAlign);
			return false;
		}

		return true;

	case WAVE_FORMAT_ADPCM:

		if ((wfx->nChannels != 1) && (wfx->nChannels != 2))
		{
			// DebugTrace("ERROR: Wave format ADPCM must have 1 or 2 channels (%u)\n", wfx->nChannels);
			return false;
		}

		if (wfx->wBitsPerSample != 4 /*MSADPCM_BITS_PER_SAMPLE*/)
		{
			// DebugTrace("ERROR: Wave format ADPCM must have 4 bits per sample (%u)\n", wfx->wBitsPerSample);
			return false;
		}

		if (wfx->cbSize != 32 /*MSADPCM_FORMAT_EXTRA_BYTES*/)
		{
			// DebugTrace("ERROR: Wave format ADPCM must have cbSize = 32 (%u)\n", wfx->cbSize);
			return false;
		}
		else
		{
			auto wfadpcm = reinterpret_cast<const ADPCMWAVEFORMAT*>(wfx);

			if (wfadpcm->wNumCoef != 7 /*MSADPCM_NUM_COEFFICIENTS*/)
			{
				// DebugTrace("ERROR: Wave format ADPCM must have 7 coefficients (%u)\n", wfadpcm->wNumCoef);
				return false;
			}

			bool valid = true;
			for (int j = 0; j < 7 /*MSADPCM_NUM_COEFFICIENTS*/; ++j)
			{
				// Microsoft ADPCM standard encoding coefficients
				static const short g_pAdpcmCoefficients1[] = { 256,  512, 0, 192, 240,  460,  392 };
				static const short g_pAdpcmCoefficients2[] = { 0, -256, 0,  64,   0, -208, -232 };

				if (wfadpcm->aCoef[j].iCoef1 != g_pAdpcmCoefficients1[j]
					|| wfadpcm->aCoef[j].iCoef2 != g_pAdpcmCoefficients2[j])
				{
					valid = false;
				}
			}

			if (!valid)
			{
				// DebugTrace("ERROR: Wave formt ADPCM found non-standard coefficients\n");
				return false;
			}

			if ((wfadpcm->wSamplesPerBlock < 4 /*MSADPCM_MIN_SAMPLES_PER_BLOCK*/)
				|| (wfadpcm->wSamplesPerBlock > 64000 /*MSADPCM_MAX_SAMPLES_PER_BLOCK*/))
			{
				// DebugTrace("ERROR: Wave format ADPCM wSamplesPerBlock must be 4..64000 (%u)\n", wfadpcm->wSamplesPerBlock);
				return false;
			}

			if (wfadpcm->wfx.nChannels == 1 && (wfadpcm->wSamplesPerBlock % 2))
			{
				// DebugTrace("ERROR: Wave format ADPCM mono files must have even wSamplesPerBlock\n");
				return false;
			}

			int nHeaderBytes = 7 /*MSADPCM_HEADER_LENGTH*/ * wfx->nChannels;
			int nBitsPerFrame = 4 /*MSADPCM_BITS_PER_SAMPLE*/ * wfx->nChannels;
			int nPcmFramesPerBlock = (wfx->nBlockAlign - nHeaderBytes) * 8 / nBitsPerFrame + 2;

			if (wfadpcm->wSamplesPerBlock != nPcmFramesPerBlock)
			{
				// DebugTrace("ERROR: Wave format ADPCM %u-channel with nBlockAlign = %u must have wSamplesPerBlock = %u (%u)\n", wfx->nChannels, wfx->nBlockAlign, nPcmFramesPerBlock, wfadpcm->wSamplesPerBlock);
				return false;
			}
		}
		return true;

	case WAVE_FORMAT_WMAUDIO2:
	case WAVE_FORMAT_WMAUDIO3:

#if defined(_XBOX_ONE) || (_WIN32_WINNT < _WIN32_WINNT_WIN8) || (_WIN32_WINNT >= _WIN32_WINNT_WIN10)

		if (wfx->wBitsPerSample != 16)
		{
			// DebugTrace("ERROR: Wave format xWMA only supports 16-bit data\n");
			return false;
		}

		if (!wfx->nBlockAlign)
		{
			// DebugTrace("ERROR: Wave format xWMA must have a non-zero nBlockAlign\n");
			return false;
		}

		if (!wfx->nAvgBytesPerSec)
		{
			// DebugTrace("ERROR: Wave format xWMA must have a non-zero nAvgBytesPerSec\n");
			return false;
		}

		return true;

#else
		DebugTrace("ERROR: Wave format xWMA not supported by this version of DirectXTK for Audio\n");
		return false;
#endif

	case 0x166 /* WAVE_FORMAT_XMA2 */:

#if defined(_XBOX_ONE) && defined(_TITLE)

		if (wfx->nBlockAlign != wfx->nChannels * XMA_OUTPUT_SAMPLE_BYTES)
		{
			DebugTrace("ERROR: Wave format XMA2 - nBlockAlign (%u) != nChannels(%u) * %u\n", wfx->nBlockAlign, wfx->nChannels, XMA_OUTPUT_SAMPLE_BYTES);
			return false;
		}

		if (wfx->wBitsPerSample != XMA_OUTPUT_SAMPLE_BITS)
		{
			DebugTrace("ERROR: Wave format XMA2 wBitsPerSample (%u) should be %u\n", wfx->wBitsPerSample, XMA_OUTPUT_SAMPLE_BITS);
			return false;
		}

		if (wfx->cbSize != (sizeof(XMA2WAVEFORMATEX) - sizeof(WAVEFORMATEX)))
		{
			DebugTrace("ERROR: Wave format XMA2 - cbSize must be %zu (%u)\n", (sizeof(XMA2WAVEFORMATEX) - sizeof(WAVEFORMATEX)), wfx->cbSize);
			return false;
		}
		else
		{
			auto xmaFmt = reinterpret_cast<const XMA2WAVEFORMATEX*>(wfx);

			if (xmaFmt->EncoderVersion < 3)
			{
				DebugTrace("ERROR: Wave format XMA2 encoder version (%u) - 3 or higher is required\n", xmaFmt->EncoderVersion);
				return false;
			}

			if (!xmaFmt->BlockCount)
			{
				DebugTrace("ERROR: Wave format XMA2 BlockCount must be non-zero\n");
				return false;
			}

			if (!xmaFmt->BytesPerBlock || (xmaFmt->BytesPerBlock > XMA_READBUFFER_MAX_BYTES))
			{
				DebugTrace("ERROR: Wave format XMA2 BytesPerBlock (%u) is invalid\n", xmaFmt->BytesPerBlock);
				return false;
			}

			if (xmaFmt->ChannelMask)
			{
				auto channelBits = ChannelsSpecifiedInMask(xmaFmt->ChannelMask);
				if (channelBits != wfx->nChannels)
				{
					DebugTrace("ERROR: Wave format XMA2 - nChannels=%u but ChannelMask (%08X) has %u bits set\n",
						xmaFmt->ChannelMask, wfx->nChannels, channelBits);
					return false;
				}
			}

			if (xmaFmt->NumStreams != ((wfx->nChannels + 1) / 2))
			{
				DebugTrace("ERROR: Wave format XMA2 - NumStreams (%u) != ( nChannels(%u) + 1 ) / 2\n", xmaFmt->NumStreams, wfx->nChannels);
				return false;
			}

			if ((xmaFmt->PlayBegin + xmaFmt->PlayLength) > xmaFmt->SamplesEncoded)
			{
				DebugTrace("ERROR: Wave format XMA2 play region too large (%u + %u > %u)\n", xmaFmt->PlayBegin, xmaFmt->PlayLength, xmaFmt->SamplesEncoded);
				return false;
			}

			if ((xmaFmt->LoopBegin + xmaFmt->LoopLength) > xmaFmt->SamplesEncoded)
			{
				DebugTrace("ERROR: Wave format XMA2 loop region too large (%u + %u > %u)\n", xmaFmt->LoopBegin, xmaFmt->LoopLength, xmaFmt->SamplesEncoded);
				return false;
			}
		}
		return true;

#else
		// DebugTrace("ERROR: Wave format XMA2 not supported by this version of DirectXTK for Audio\n");
		return false;
#endif

	case WAVE_FORMAT_EXTENSIBLE:
		if (wfx->cbSize < (sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX)))
		{
			// DebugTrace("ERROR: Wave format WAVE_FORMAT_EXTENSIBLE - cbSize must be %zu (%u)\n", (sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX)), wfx->cbSize);
			return false;
		}
		else
		{
			static const GUID s_wfexBase = { 0x00000000, 0x0000, 0x0010,{ 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };

			auto wfex = reinterpret_cast<const WAVEFORMATEXTENSIBLE*>(wfx);

			if (memcmp(reinterpret_cast<const BYTE*>(&wfex->SubFormat) + sizeof(DWORD),
				reinterpret_cast<const BYTE*>(&s_wfexBase) + sizeof(DWORD), sizeof(GUID) - sizeof(DWORD)) != 0)
			{
				// DebugTrace("ERROR: Wave format WAVEFORMATEXTENSIBLE encountered with unknown GUID ({%8.8lX-%4.4X-%4.4X-%2.2X%2.2X-%2.2X%2.2X%2.2X%2.2X%2.2X%2.2X})\n",
				// wfex->SubFormat.Data1, wfex->SubFormat.Data2, wfex->SubFormat.Data3,
				// wfex->SubFormat.Data4[0], wfex->SubFormat.Data4[1], wfex->SubFormat.Data4[2], wfex->SubFormat.Data4[3],
				// wfex->SubFormat.Data4[4], wfex->SubFormat.Data4[5], wfex->SubFormat.Data4[6], wfex->SubFormat.Data4[7]);
				return false;
			}

			switch (wfex->SubFormat.Data1)
			{
			case WAVE_FORMAT_PCM:

				switch (wfx->wBitsPerSample)
				{
				case 8:
				case 16:
				case 24:
				case 32:
					break;

				default:
					// DebugTrace("ERROR: Wave format integer PCM must have 8, 16, 24, or 32 bits per sample (%u)\n", wfx->wBitsPerSample);
					return false;
				}

				switch (wfex->Samples.wValidBitsPerSample)
				{
				case 0:
				case 8:
				case 16:
				case 20:
				case 24:
				case 32:
					break;

				default:
					// DebugTrace("ERROR: Wave format integer PCM must have 8, 16, 20, 24, or 32 valid bits per sample (%u)\n", wfex->Samples.wValidBitsPerSample);
					return false;
				}

				if (wfex->Samples.wValidBitsPerSample
					&& (wfex->Samples.wValidBitsPerSample > wfx->wBitsPerSample))
				{
					// DebugTrace("ERROR: Wave format ingter PCM wValidBitsPerSample (%u) is greater than wBitsPerSample (%u)\n", wfex->Samples.wValidBitsPerSample, wfx->wBitsPerSample);
					return false;
				}

				if (wfx->nBlockAlign != (wfx->nChannels * wfx->wBitsPerSample / 8))
				{
					// DebugTrace("ERROR: Wave format integer PCM - nBlockAlign (%u) != nChannels (%u) * wBitsPerSample (%u) / 8\n", wfx->nBlockAlign, wfx->nChannels, wfx->wBitsPerSample);
					return false;
				}

				if (wfx->nAvgBytesPerSec != (wfx->nSamplesPerSec * wfx->nBlockAlign))
				{
					// DebugTrace("ERROR: Wave format integer PCM - nAvgBytesPerSec (%lu) != nSamplesPerSec (%lu) * nBlockAlign (%u)\n", wfx->nAvgBytesPerSec, wfx->nSamplesPerSec, wfx->nBlockAlign);
					return false;
				}

				break;

			case WAVE_FORMAT_IEEE_FLOAT:

				if (wfx->wBitsPerSample != 32)
				{
					// DebugTrace("ERROR: Wave format float PCM must have 32-bits per sample (%u)\n", wfx->wBitsPerSample);
					return false;
				}

				switch (wfex->Samples.wValidBitsPerSample)
				{
				case 0:
				case 32:
					break;

				default:
					// DebugTrace("ERROR: Wave format float PCM must have 32 valid bits per sample (%u)\n", wfex->Samples.wValidBitsPerSample);
					return false;
				}

				if (wfx->nBlockAlign != (wfx->nChannels * wfx->wBitsPerSample / 8))
				{
					// DebugTrace("ERROR: Wave format float PCM - nBlockAlign (%u) != nChannels (%u) * wBitsPerSample (%u) / 8\n", wfx->nBlockAlign, wfx->nChannels, wfx->wBitsPerSample);
					return false;
				}

				if (wfx->nAvgBytesPerSec != (wfx->nSamplesPerSec * wfx->nBlockAlign))
				{
					// DebugTrace("ERROR: Wave format float PCM - nAvgBytesPerSec (%lu) != nSamplesPerSec (%lu) * nBlockAlign (%u)\n", wfx->nAvgBytesPerSec, wfx->nSamplesPerSec, wfx->nBlockAlign);
					return false;
				}

				break;

			case WAVE_FORMAT_ADPCM:
				// DebugTrace("ERROR: Wave format ADPCM is not supported as a WAVEFORMATEXTENSIBLE\n");
				return false;

			case WAVE_FORMAT_WMAUDIO2:
			case WAVE_FORMAT_WMAUDIO3:

#if defined(_XBOX_ONE) || (_WIN32_WINNT < _WIN32_WINNT_WIN8) || (_WIN32_WINNT >= _WIN32_WINNT_WIN10)

				if (wfx->wBitsPerSample != 16)
				{
					// DebugTrace("ERROR: Wave format xWMA only supports 16-bit data\n");
					return false;
				}

				if (!wfx->nBlockAlign)
				{
					// DebugTrace("ERROR: Wave format xWMA must have a non-zero nBlockAlign\n");
					return false;
				}

				if (!wfx->nAvgBytesPerSec)
				{
					// DebugTrace("ERROR: Wave format xWMA must have a non-zero nAvgBytesPerSec\n");
					return false;
				}

				break;

#else
				DebugTrace("ERROR: Wave format xWMA not supported by this version of DirectXTK for Audio\n");
				return false;
#endif

			case 0x166 /* WAVE_FORMAT_XMA2 */:
				// DebugTrace("ERROR: Wave format XMA2 is not supported as a WAVEFORMATEXTENSIBLE\n");
				return false;

			default:
				// DebugTrace("ERROR: Unknown WAVEFORMATEXTENSIBLE format tag (%u)\n", wfex->SubFormat.Data1);
				return false;
			}

			if (wfex->dwChannelMask)
			{
				auto channelBits = ChannelsSpecifiedInMask(wfex->dwChannelMask);
				if (channelBits != wfx->nChannels)
				{
					// DebugTrace("ERROR: WAVEFORMATEXTENSIBLE: nChannels=%u but ChannelMask has %u bits set\n", wfx->nChannels, channelBits);
					return false;
				}
			}

			return true;
		}

	default:
		// DebugTrace("ERROR: Unknown WAVEFORMATEX format tag (%u)\n", wfx->wFormatTag);
		return false;
	}
}

// function from Microsoft examples to generate a unique key for a waveformatex
unsigned int CSourceVoice::makeVoiceKey(_In_ const WAVEFORMATEX * wfx)
{
	assert(IsValid(wfx));

	if (wfx->nChannels > 0x7F)
		return 0;

	union KeyGen
	{
		struct
		{
			unsigned int tag : 9;
			unsigned int channels : 7;
			unsigned int bitsPerSample : 8;
		} pcm;

		struct
		{
			unsigned int tag : 9;
			unsigned int channels : 7;
			unsigned int samplesPerBlock : 16;
		} adpcm;

#if defined(_XBOX_ONE) && defined(_TITLE)
		struct
		{
			unsigned int tag : 9;
			unsigned int channels : 7;
			unsigned int encoderVersion : 8;
		} xma;
#endif

		unsigned int key;
	} result;

	static_assert(sizeof(KeyGen) == sizeof(unsigned int), "KeyGen is invalid");

	result.key = 0;

	if (wfx->wFormatTag == WAVE_FORMAT_EXTENSIBLE)
	{
		// We reuse EXTENSIBLE only if it is equivalent to the standard form
		auto wfex = reinterpret_cast<const WAVEFORMATEXTENSIBLE*>(wfx);
		if (wfex->Samples.wValidBitsPerSample != 0 && wfex->Samples.wValidBitsPerSample != wfx->wBitsPerSample)
			return 0;

		if (wfex->dwChannelMask != 0 && wfex->dwChannelMask != GetDefaultChannelMask(wfx->nChannels))
			return 0;
	}

	uint32_t tag = GetFormatTag(wfx);
	switch (tag)
	{
	case WAVE_FORMAT_PCM:
		static_assert(WAVE_FORMAT_PCM < 0x1ff, "KeyGen tag is too small");
		result.pcm.tag = WAVE_FORMAT_PCM;
		result.pcm.channels = wfx->nChannels;
		result.pcm.bitsPerSample = wfx->wBitsPerSample;
		break;

	case WAVE_FORMAT_IEEE_FLOAT:
		static_assert(WAVE_FORMAT_IEEE_FLOAT < 0x1ff, "KeyGen tag is too small");

		if (wfx->wBitsPerSample != 32)
			return 0;

		result.pcm.tag = WAVE_FORMAT_IEEE_FLOAT;
		result.pcm.channels = wfx->nChannels;
		result.pcm.bitsPerSample = 32;
		break;

	case WAVE_FORMAT_ADPCM:
		static_assert(WAVE_FORMAT_ADPCM < 0x1ff, "KeyGen tag is too small");
		result.adpcm.tag = WAVE_FORMAT_ADPCM;
		result.adpcm.channels = wfx->nChannels;

		{
			auto wfadpcm = reinterpret_cast<const ADPCMWAVEFORMAT*>(wfx);
			result.adpcm.samplesPerBlock = wfadpcm->wSamplesPerBlock;
		}
		break;

#if defined(_XBOX_ONE) && defined(_TITLE)
	case WAVE_FORMAT_XMA2:
		static_assert(WAVE_FORMAT_XMA2 < 0x1ff, "KeyGen tag is too small");
		result.xma.tag = WAVE_FORMAT_XMA2;
		result.xma.channels = wfx->nChannels;

		{
			auto xmaFmt = reinterpret_cast<const XMA2WAVEFORMATEX*>(wfx);

			if ((xmaFmt->LoopBegin > 0)
				|| (xmaFmt->PlayBegin > 0))
				return 0;

			result.xma.encoderVersion = xmaFmt->EncoderVersion;
		}
		break;
#endif

	default:
		return 0;
	}

	return result.key;
}

// function from Microsoft coding guide to find specific chungs in a RIFF-file
HRESULT CSourceVoice::FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;

}

// function from Microsoft coding guide to read chunkdata after having found it with FindChunk(..)
HRESULT CSourceVoice::ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}
