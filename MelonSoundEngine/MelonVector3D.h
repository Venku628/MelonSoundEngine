#define _USE_MATH_DEFINES

#include <cmath>

#define ANGLE_TOLERANCE 0.99995f

#pragma once
class CMelonVector3D
{
public:
	CMelonVector3D();
	CMelonVector3D(float x, float y, float z);
	~CMelonVector3D();

	CMelonVector3D Normalize() const;

	CMelonVector3D CalculateVectorBetweenPoints(const CMelonVector3D& other) const;
	float CalculateAngleBetweenVectors(const CMelonVector3D& other) const;
	float CalculateDotProduct(const CMelonVector3D& other) const;
	float CalculateDistance(const CMelonVector3D& other) const;
	float CalculateLength() const;

	float operator*(const CMelonVector3D& rhs);
	CMelonVector3D operator+(const CMelonVector3D& rhs);
	CMelonVector3D operator-(const CMelonVector3D& rhs);
	CMelonVector3D operator*(const float rhs);
	CMelonVector3D operator/(const float rhs);

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

