#include <math.h>

#pragma once
class CMelonVector3D
{
public:
	CMelonVector3D();
	CMelonVector3D(float x, float y, float z);
	~CMelonVector3D();

	CMelonVector3D CalculateVectorBetweenPoints(const CMelonVector3D& other) const;
	float CalculateDotProduct(const CMelonVector3D& other) const;
	float CalculateDistance(const CMelonVector3D& other) const;
	float CalculateLength() const;

	float GetX() const;
	float GetY() const;
	float GetZ() const;
	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	void SetXYZ(float x, float y, float z);

protected:
	float m_fX, m_fY, m_fZ;
};

