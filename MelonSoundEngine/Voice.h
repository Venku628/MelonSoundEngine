#pragma once

#include "XAudio2Basics.h"

class CVoice
{
public:
	// 1.f means full, 0 means silence
	virtual void SetVolume(float fVolume) = 0;
	// 1.f means full, 0 means silence
	virtual float GetVolume() = 0;

	virtual void SetSampleRate(unsigned int sampleRate) = 0;

	
	virtual void StartPlayback() = 0;
	virtual void StopPlayback() = 0;

	/*

	virtual void UpdatePosition(float x, float y, float z);
	virtual void UpdateVelocity(float x, float y, float z);

	virtual const CMelonVector3D & GetPosition() const;
	virtual const CMelonVector3D & GetVelocity() const;
	virtual const CMelonMatrix3 & GetOrientation() const;

	virtual void SetPosition(CMelonVector3D position);
	virtual void SetVelocity(CMelonVector3D velocity);
	virtual void SetRotation(CMelonMatrix3 rotation);

	// maybe use the Parameter structs for fewer functions instead?
	virtual void Tick();

	virtual void SetFalloffRadius(float radius);
	virtual void SetFalloffMax(float falloffMax);

	virtual void SetFalloffEnabled(bool enabled);

	virtual void SetDopplerEnabled(bool enabled);

	virtual void SetPanEnabled(bool enabled);
	virtual void SetPanVerticalEnabled(bool enabled);
	virtual void SetPanVerticalAngle(float verticalAngle);

	


	*/
};

