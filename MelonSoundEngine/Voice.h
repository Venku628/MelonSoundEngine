#pragma once

#include "XAudio2Basics.h"

class CVoice
{
public:
	// 1.f means full, 0 means silence
	virtual void SetVolume(float fVolume) = 0;
	// 1.f means full, 0 means silence
	virtual float GetVolume() = 0;

	/*
	virtual HRESULT StartPlayback() = 0;
	virtual HRESULT StopPlayback() = 0;

	virtual void UpdatePosition(float x, float y, float z);
	virtual void UpdateVelocity(float x, float y, float z);

	virtual const CMelonVector3D & GetPosition() const;
	virtual const CMelonVector3D & GetVelocity() const;
	virtual const CMelonMatrix3 & GetOrientation() const;


	*/
};

