#pragma once

#include "SourceVoice.h"
#include "SubmixVoice.h"
#include <unordered_map>
#include <map>
#include <set>
#include "MelonMatrix3.h"
#include "IVoiceNotify.h"

// from DirectXTK and msnd programming guide
//--------------------------------------------------------------------------------------

// this is given to voices to get callbacks for the voicepool
struct VoiceCallback : public IXAudio2VoiceCallback
{
	HANDLE m_hBufferEnd;
	VoiceCallback() {}

	~VoiceCallback()
	{
		CloseHandle(m_hBufferEnd);
	}

	STDMETHOD_(void, OnVoiceProcessingPassStart) (UINT32) override {}
	STDMETHOD_(void, OnVoiceProcessingPassEnd)() override {}
	STDMETHOD_(void, OnStreamEnd)() override {
		SetEvent(m_hBufferEnd);
	}
	STDMETHOD_(void, OnBufferStart)(void*) override {}

	STDMETHOD_(void, OnBufferEnd)(void* context) override
	{
		SetEvent(m_hBufferEnd);
		
	}

	STDMETHOD_(void, OnLoopEnd)(void*) override {}
	STDMETHOD_(void, OnVoiceError)(void*, HRESULT) override {}

};
//--------------------------------------------------------------------------------------

class CMelonSoundEngine
{
public:
	// singleton
	static CMelonSoundEngine& GetInstance();
	CMelonSoundEngine(CMelonSoundEngine const&) = delete;
	void operator=(CMelonSoundEngine const&) = delete;


	unsigned int CreateSourceVoice(const char * stFileName, bool bLoopsInfinitley = false, unsigned int uiTargetSubmixHandle = 0);
	unsigned int CreateSourceVoice3D(const char * stFileName, bool bLoopsInfinitley = false, unsigned int uiTargetSubmixHandle = 0);
	unsigned int CreateSubmixVoice(unsigned int uiChannelNumberInput = 1, unsigned int uiSampleRate = DEFAULT_SAMPLE_RATE, unsigned int uiTargetSubmixHandle = 0);

	void StartPlayback(unsigned int uiTarget);
	void StopPlayback(unsigned int uiTarget);

	void SetVolume(unsigned int uiTarget, float fVolume);
	void SetEffectChain(unsigned int uiTarget, const XAUDIO2_EFFECT_CHAIN * pEffectChain);
	void SetEffectParameters(unsigned int uiTarget, unsigned int uiEffectIndex, const void * pParameters, unsigned int uiParametersByteSize);

	void UpdatePostion(unsigned int uiTarget, float x, float y, float z);
	void UpdateVelocity(unsigned int uiTarget, float x, float y, float z);
	void UpdateRotation(unsigned int uiTarget, 
		float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22);
	// A non targeted version of this would tick all objects, but that would make no sense if some targets didn´t move
	void Tick(unsigned int uiTarget);
	void SetPanParameters(unsigned int uiTarget, SPanParameters & parameters);
	void SetFalloffParameters(unsigned int uiTarget, SFalloffParameters & parameters);
	void SetDopplerEffectParameters(unsigned int uiTarget, SDopplerEffectParameters & parameters);


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
	IXAudio2SourceVoice * OpenSourceFile(const char * stFileName, bool bLoopsInfinitley = false, const XAUDIO2_VOICE_SENDS * SFXSendList = nullptr);

	IXAudio2MasteringVoice * m_pMasteringVoice;
	IXAudio2 * m_pXAudio2;
	std::unordered_map<unsigned int, IXAudio2SourceVoice*> m_voicepool;
	std::unordered_multimap<unsigned int, IXAudio2SourceVoice*> m_oneShotList;
	std::map<unsigned int, CVoice*> m_uiHandleMap;

	VoiceCallback m_voiceCallback;
	std::set<IVoiceNotify*> m_notifyObjectList;
	std::set<IVoiceNotify*> m_notifyUpdateList;

	// Voicepool
	static uint32_t GetDefaultChannelMask(int channels);
	static uint32_t GetFormatTag(const WAVEFORMATEX* wfx);
	static bool IsValid(_In_ const WAVEFORMATEX* wfx);
	static unsigned int makeVoiceKey(_In_ const WAVEFORMATEX* wfx);

	static HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset);
};

