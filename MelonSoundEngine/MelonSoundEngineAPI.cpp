#include "MelonSoundEngineAPI.h"

namespace MelonSoundEngine
{
	void MelonSoundEngineAPI::Init()
	{
		CMelonSoundEngine::GetInstance();
	}
	unsigned int MelonSoundEngineAPI::CreateSourceVoice(const char * stFileName, bool bLoopsInfinitley, unsigned int uiTargetSubmixHandle)
	{
		return CMelonSoundEngine::GetInstance().CreateSourceVoice(stFileName, bLoopsInfinitley, uiTargetSubmixHandle);
	}
	unsigned int MelonSoundEngineAPI::CreateSourceVoice3D(const char * stFileName, bool bLoopsInfinitley, unsigned int uiTargetSubmixHandle)
	{
		return CMelonSoundEngine::GetInstance().CreateSourceVoice3D(stFileName, bLoopsInfinitley, uiTargetSubmixHandle);
	}
	unsigned int MelonSoundEngineAPI::CreateSubmixVoice(unsigned int uiChannelNumberInput, unsigned int uiSampleRate, unsigned int uiTargetSubmixHandle)
	{
		return CMelonSoundEngine::GetInstance().CreateSubmixVoice(uiChannelNumberInput, uiSampleRate, uiTargetSubmixHandle);
	}
	void MelonSoundEngineAPI::StartPlayback(unsigned int uiTarget)
	{
		CMelonSoundEngine::GetInstance().StartPlayback(uiTarget);
	}
	void MelonSoundEngineAPI::StopPlayback(unsigned int uiTarget)
	{
		CMelonSoundEngine::GetInstance().StopPlayback(uiTarget);
	}
	void MelonSoundEngineAPI::SetVolume(unsigned int uiTarget, float fVolume)
	{
		CMelonSoundEngine::GetInstance().SetVolume(uiTarget, fVolume);
	}
	void MelonSoundEngineAPI::SetVolume(float fVolume)
	{
		CMelonSoundEngine::GetInstance().GetMasteringVoice()->SetVolume(fVolume);
	}
	void MelonSoundEngineAPI::SetEffectChain(unsigned int uiTarget, const XAUDIO2_EFFECT_CHAIN * pEffectChain)
	{
		CMelonSoundEngine::GetInstance().SetEffectChain(uiTarget, pEffectChain);
	}
	void MelonSoundEngineAPI::SetEffectParameters(unsigned int uiTarget, unsigned int uiEffectIndex, const void * pParameters, unsigned int uiParametersByteSize)
	{
		CMelonSoundEngine::GetInstance().SetEffectParameters(uiTarget, uiEffectIndex, pParameters, uiParametersByteSize);
	}
	void MelonSoundEngineAPI::SetOutputMatrix(unsigned int uiTarget, unsigned int uiSourceChannels, unsigned int uiDestinationChannels, const float * pfLevelMatrix)
	{
		CMelonSoundEngine::GetInstance().SetOutputMatrix(uiTarget, uiSourceChannels, uiDestinationChannels, pfLevelMatrix);
	}
	void MelonSoundEngineAPI::UpdatePostion(unsigned int uiTarget, float x, float y, float z)
	{
		CMelonSoundEngine::GetInstance().UpdatePostion(uiTarget, x, y, z);
	}
	void MelonSoundEngineAPI::UpdateVelocity(unsigned int uiTarget, float x, float y, float z)
	{
		CMelonSoundEngine::GetInstance().UpdateVelocity(uiTarget, x, y, z);
	}
	void MelonSoundEngineAPI::UpdateRotation(unsigned int uiTarget, float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
	{
		CMelonSoundEngine::GetInstance().UpdateRotation(uiTarget, m00, m01, m02, m10, m11, m12, m20, m21, m22);
	}
	void MelonSoundEngineAPI::Tick(unsigned int uiTarget)
	{
		CMelonSoundEngine::GetInstance().Tick(uiTarget);
	}
	void MelonSoundEngineAPI::SetPanParameters(unsigned int uiTarget, SPanParameters & parameters)
	{
		CMelonSoundEngine::GetInstance().SetPanParameters(uiTarget, parameters);
	}
	void MelonSoundEngineAPI::SetFalloffParameters(unsigned int uiTarget, SFalloffParameters & parameters)
	{
		CMelonSoundEngine::GetInstance().SetFalloffParameters(uiTarget, parameters);
	}
	void MelonSoundEngineAPI::SetDopplerEffectParameters(unsigned int uiTarget, SDopplerEffectParameters & parameters)
	{
		CMelonSoundEngine::GetInstance().SetDopplerEffectParameters(uiTarget, parameters);
	}
}