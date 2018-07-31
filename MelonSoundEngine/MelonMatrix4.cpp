#include "MelonMatrix4.h"



CMelonMatrix4::CMelonMatrix4()
{
	m00 = m01 = m02 = m03 =
	m10 = m11 = m12 = m13 =
	m20 = m21 = m22 = m23 = 0.f;
}

CMelonMatrix4::CMelonMatrix4(float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23)
{
	this->m00 = m00;
	this->m01 = m01;
	this->m02 = m02;
	this->m03 = m03;
	this->m10 = m10;
	this->m11 = m11;
	this->m12 = m12;
	this->m13 = m13;
	this->m20 = m20;
	this->m21 = m21;
	this->m22 = m22;
	this->m23 = m23;
}


CMelonMatrix4::~CMelonMatrix4()
{
}

CMelonVector3D CMelonMatrix4::GetXAxis() const
{
	return CMelonVector3D(m00, m10, m20);
}

CMelonVector3D CMelonMatrix4::GetYAxis() const
{
	return CMelonVector3D(m01, m11, m21);
}

CMelonVector3D CMelonMatrix4::GetZAxis()const
{
	return CMelonVector3D(m02, m12, m22);
}

CMelonVector3D CMelonMatrix4::GetPosition() const
{
	return CMelonVector3D(m03, m13, m23);
}

CMelonMatrix4 CMelonMatrix4::CalculateInverseRotation()
{
	float determinant = m00 * (m11 * m22 - m12 * m21)
					+ m01 * (m12 *m20 - m10 * m22)
					+ m02 *(m10 * m21 - m11 * m20);

	return CMelonMatrix4(
		(m11*m22 - m12 * m21) / determinant,
		(m21*m02 - m22 * m01) / determinant,
		(m01*m12 - m02 * m11) / determinant,
		0.f,
		(m12*m20 - m10 * m22) / determinant,
		(m22*m00 - m20 * m02) / determinant,
		(m02*m10 - m00 * m12) / determinant,
		0.f,
		(m10*m21 - m11 * m20) / determinant,
		(m20*m01 - m21 * m00) / determinant,
		(m00*m11 - m01 * m10) / determinant,
		0.f
		);
}

void CMelonMatrix4::printMatrix()
{
	std::cout << m00 << " " << m01 << " " << m02 << "\n"
		<< m10 << " " << m11 << " " << m12 << "\n"
		<< m20 << " " << m21 << " " << m22;
}
