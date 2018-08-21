#pragma once

#include "SourceVoice.h"
#include "SubmixVoice.h"
#include <unordered_map>
#include <map>
#include "MelonMatrix3.h"
#include "IVoiceNotify.h"

// from DirectXTK
//--------------------------------------------------------------------------------------
struct handle_closer { void operator()(HANDLE h) noexcept { if (h) CloseHandle(h); } };
typedef std::unique_ptr<void, handle_closer> ScopedHandle;

struct VoiceCallback : public IXAudio2VoiceCallback
{
	VoiceCallback() noexcept(false)
	{
		mBufferEnd.reset(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE));
		if (!mBufferEnd)
		{
			throw std::exception("CreateEvent");
		}
	}

	virtual ~VoiceCallback()
	{
	}

	STDMETHOD_(void, OnVoiceProcessingPassStart) (UINT32) override {}
	STDMETHOD_(void, OnVoiceProcessingPassEnd)() override {}
	STDMETHOD_(void, OnStreamEnd)() override {}
	STDMETHOD_(void, OnBufferStart)(void*) override {}

	STDMETHOD_(void, OnBufferEnd)(void* context) override
	{
		if (context)
		{
			auto inotify = reinterpret_cast<IVoiceNotify*>(context);
			inotify->OnBufferEnd();
			SetEvent(mBufferEnd.get());
		}
	}

	STDMETHOD_(void, OnLoopEnd)(void*) override {}
	STDMETHOD_(void, OnVoiceError)(void*, HRESULT) override {}

	ScopedHandle mBufferEnd;
};
//--------------------------------------------------------------------------------------

class CMelonSoundEngine
{
public:
	// singleton
	static CMelonSoundEngine& GetInstance();
	CMelonSoundEngine(CMelonSoundEngine const&) = delete;
	void operator=(CMelonSoundEngine const&) = delete;

	// temporary Creates until Voice Pool and Handles implemented
	CSourceVoice * CreateSourceVoice(const char * stFileName);
	CSourceVoice3D * CreateSourceVoice3D(const char * stFileName);
	CSubmixVoice * CreateSubmixVoice();

	IXAudio2 * GetXAudio2() const;
	IXAudio2MasteringVoice * GetMasteringVoice() const;

	unsigned int CreateVoiceHandle(CVoice * voice); 


	bool Update();
	void TrimVoicePool();
	void DestroyAllVoices();

	// void AssignVoice();

private:
	CMelonSoundEngine();
	~CMelonSoundEngine();

	unsigned int GetUniqueVoiceHandle() const;
	IXAudio2SourceVoice * OpenSourceFile(const char * stFileName, XAUDIO2_VOICE_SENDS * SFXSendList = nullptr, bool bLoopsInfinitley = false);

	IXAudio2MasteringVoice * m_pMasteringVoice;
	IXAudio2 * m_pXAudio2;
	std::unordered_multimap<unsigned int, IXAudio2SourceVoice*> m_voicepool;
	std::unordered_multimap<unsigned int, IXAudio2SourceVoice*> m_oneShotList;
	std::map<unsigned int, CVoice*> m_uiHandleMap;

	VoiceCallback m_voiceCallback;
	

	unsigned int m_uiMaxVoiceInstances = 20, m_uiVoiceInstances = 0;


	// Voicepool
	static uint32_t GetDefaultChannelMask(int channels);
	static uint32_t GetFormatTag(const WAVEFORMATEX* wfx);
	static bool IsValid(_In_ const WAVEFORMATEX* wfx);
	static unsigned int makeVoiceKey(_In_ const WAVEFORMATEX* wfx);

	static HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset);
};

