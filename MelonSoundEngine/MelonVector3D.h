#define _USE_MATH_DEFINES

#include <cmath>

#define ANGLE_TOLERANCE 0.99995f

#pragma once
class CMelonVector3D
{
	friend class CMelonMatrix3;

public:
	CMelonVector3D();
	CMelonVector3D(float x, float y, float z);
	~CMelonVector3D();

	CMelonVector3D Normalize() const;

	CMelonVector3D CalculateVectorBetweenPoints(const CMelonVector3D& other) const;
	float CalculateAngleBetweenVectors(const CMelonVector3D& other) const;
	float CalculateAngleToYAxis() const;
	float CalculateDotProduct(const CMelonVector3D& other) const;
	float CalculateDistance(const CMelonVector3D& other) const;
	float CalculateLength() const;

	float operator*(const CMelonVector3D& rhs) const;
	CMelonVector3D operator+(const CMelonVector3D& rhs) const;
	CMelonVector3D operator-(const CMelonVector3D& rhs) const;
	CMelonVector3D operator*(const float rhs) const;
	CMelonVector3D operator/(const float rhs) const;

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

