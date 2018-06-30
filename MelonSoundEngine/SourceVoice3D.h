#pragma once
#include "SourceVoice.h"
#include "Listener.h"

#define DEBUG_SPEED_OF_SOUND 343

class CSourceVoice3D :
	public CSourceVoice
{
public:
	CSourceVoice3D(IXAudio2 &pXAudio2, const char * stFileName) : CSourceVoice(pXAudio2, stFileName) {};
	~CSourceVoice3D();

	void SetVolume(float fVolume) override;

	void UpdatePosition(float x, float y, float z);
	void UpdateVelocity(float x, float y, float z);
	void UpdateOrientationFront(float x, float y, float z);
	void UpdateOrientationTop(float x, float y, float z);
	const CMelonVector3D * GetPositionPointer() const;
	const CMelonVector3D * GetVelocityPointer() const;
	const CMelonVector3D * GetOrientationFrontPointer() const;
	const CMelonVector3D * GetOrientationTopPointer() const;

	void Tick();

private:
	CMelonVector3D * m_position;
	CMelonVector3D * m_velocity;
	CMelonVector3D * m_orientationFront;
	CMelonVector3D * m_orientationTop;

	// Default for Debugging
	UINT32 m_uiBaseSampleRate = 44100;

	// Default for Debugging
	bool m_bDopplerEffectEnabled = true;

	void DopplerEffectTick();
};

