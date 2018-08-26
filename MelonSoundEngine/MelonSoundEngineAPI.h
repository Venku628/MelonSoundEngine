#pragma once

#include "MelonSoundEngine.h"

#ifdef MELONSOUNDENGINE_EXPORTS  
#define MELONSOUNDENGINE_API __declspec(dllexport)   
#else  
#define MELONSOUNDENGINE_API __declspec(dllimport)   
#endif  

namespace MelonSoundEngine
{
	// This class is exported from the MelonSoundEngine.dll
	class MelonSoundEngineAPI
	{
	public:
		static MELONSOUNDENGINE_API void Init();

		static MELONSOUNDENGINE_API unsigned int CreateSourceVoice(const char * stFileName, bool bLoopsInfinitley = false, unsigned int uiTargetSubmixHandle = 0);
		// Only Mono WAV-Files for now
		static MELONSOUNDENGINE_API unsigned int CreateSourceVoice3D(const char * stFileName, bool bLoopsInfinitley = false, unsigned int uiTargetSubmixHandle = 0);
		static MELONSOUNDENGINE_API unsigned int CreateSubmixVoice(unsigned int uiChannelNumberInput = 1, unsigned int uiSampleRate = DEFAULT_SAMPLE_RATE, unsigned int uiTargetSubmixHandle = 0);

		static MELONSOUNDENGINE_API void StartPlayback(unsigned int uiTarget);
		static MELONSOUNDENGINE_API void StopPlayback(unsigned int uiTarget);

		// 1 is max, 0 is min
		static MELONSOUNDENGINE_API void SetVolume(unsigned int uiTarget, float fVolume);
		// global Volume
		static MELONSOUNDENGINE_API void SetVolume(float fVolume);
		static MELONSOUNDENGINE_API void SetEffectChain(unsigned int uiTarget, const XAUDIO2_EFFECT_CHAIN * pEffectChain);
		static MELONSOUNDENGINE_API void SetEffectParameters(unsigned int uiTarget, unsigned int uiEffectIndex, const void * pParameters, unsigned int uiParametersByteSize);
		static MELONSOUNDENGINE_API void SetOutputMatrix(unsigned int uiTarget, unsigned int uiSourceChannels, unsigned int uiDestinationChannels, const float * pfLevelMatrix);

		static MELONSOUNDENGINE_API void UpdatePostion(unsigned int uiTarget, float x, float y, float z);
		static MELONSOUNDENGINE_API void UpdateVelocity(unsigned int uiTarget, float x, float y, float z);
		static MELONSOUNDENGINE_API void UpdateRotation(unsigned int uiTarget,
			float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22);
		// A non targeted version of this would tick all objects, but that would make no sense if some targets didn´t move
		static MELONSOUNDENGINE_API void Tick(unsigned int uiTarget);
		static MELONSOUNDENGINE_API void SetPanParameters(unsigned int uiTarget, SPanParameters & parameters);
		static MELONSOUNDENGINE_API void SetFalloffParameters(unsigned int uiTarget, SFalloffParameters & parameters);
		static MELONSOUNDENGINE_API void SetDopplerEffectParameters(unsigned int uiTarget, SDopplerEffectParameters & parameters);

	};
}
