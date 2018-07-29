#include "XAudio2Basics.h"
#pragma once

class CListener
{
public:
	CListener();
	CListener(float x, float y, float z);
	~CListener();

	void UpdatePosition(float x, float y, float z);
	void UpdateVelocity(float x, float y, float z);
	void UpdateOrientationFront(float x, float y, float z);
	void UpdateOrientationTop(float x, float y, float z);
	const CMelonVector3D * GetPositionPointer() const;
	const CMelonVector3D * GetVelocityPointer() const;
	const CMelonVector3D * GetOrientationFrontPointer() const;
	const CMelonVector3D * GetOrientationTopPointer() const;

	static CListener& GetInstance();

	CMelonVector3D CalculatePointOnPanPlane(CMelonVector3D& sourcePoint);
	float CalculateAngleToPanPlane(CMelonVector3D& sourcePoint, CMelonVector3D& pointOnPlane);
	float CalculateAngleToOrientation(CMelonVector3D& pointOnPlane);

private:
	CMelonVector3D * m_position;
	CMelonVector3D * m_velocity;
	CMelonVector3D * m_orientationTop;
	CMelonVector3D * m_orientationFront;

	// precalculated for orthagonal projection onto plane for pan
	float m_orientationTopDotOrientationTop;
};

