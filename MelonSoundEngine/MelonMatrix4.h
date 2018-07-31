#include "MelonVector3D.h"
#include <iostream>

#pragma once
class CMelonMatrix4
{
public:
	CMelonMatrix4();
	CMelonMatrix4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23);
	~CMelonMatrix4();

	CMelonVector3D GetXAxis() const;
	CMelonVector3D GetYAxis() const;
	CMelonVector3D GetZAxis() const;
	CMelonVector3D GetPosition() const;

	CMelonMatrix4 CalculateInverseRotation();

	void printMatrix();

private:
	float m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23;
};

