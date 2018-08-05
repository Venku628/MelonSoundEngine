#include "MelonVector3D.h"
#include <iostream>

#pragma once
class CMelonMatrix3
{
	friend class CMelonVector3D;

public:
	CMelonMatrix3();
	CMelonMatrix3(float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22);
	~CMelonMatrix3();

	CMelonVector3D operator*(const CMelonVector3D& rhs) const;

	void Update(float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22);

	CMelonVector3D GetXAxis() const;
	CMelonVector3D GetYAxis() const;
	CMelonVector3D GetZAxis() const;

	CMelonMatrix3 CalculateInverseRotation();

	

	void printMatrix();

protected:
	float m00, m01, m02,
		m10, m11, m12,
		m20, m21, m22;
};

