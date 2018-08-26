#include "XAudio2Basics.h"
#pragma once

class CListener
{
public:
	CListener();
	CListener(float x, float y, float z);
	~CListener();

	void SetChannelMask(DWORD dwChannelMask);
	void UpdatePosition(float x, float y, float z);
	void UpdateVelocity(float x, float y, float z);
	void UpdateOrientation(float m00, float m01, float m02,
						float m10, float m11, float m12,
						float m20, float m21, float m22);
	const CMelonVector3D & GetPosition() const;
	const CMelonVector3D & GetVelocity() const;
	const CMelonMatrix3 & GetOrientation() const;
	const DWORD GetChannelMask() const;

	static CListener& GetInstance();

	CMelonVector3D CalculatePointOnPanPlane(CMelonVector3D& sourcePoint);
	float CalculateAngleToPanPlane(CMelonVector3D& sourcePoint, CMelonVector3D& pointOnPlane);
	float CalculateAngleToOrientation(CMelonVector3D& pointOnPlane);

private:
	CMelonVector3D m_position;
	CMelonVector3D m_velocity;
	CMelonMatrix3 m_rotation;

	DWORD m_dwChannelMask;

	// precalculated for orthagonal projection onto plane for pan
	float m_orientationTopDotOrientationTop;
};

