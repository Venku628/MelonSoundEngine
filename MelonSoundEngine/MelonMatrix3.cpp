#include "MelonMatrix3.h"



CMelonMatrix3::CMelonMatrix3()
{
	m00 = m11 = m22 = 1.f;

	m01 = m02 = 
	m10 = m12 =
	m20 = m21 = 0.f;
}

CMelonMatrix3::CMelonMatrix3(float m00, float m01, float m02, 
	float m10, float m11, float m12, 
	float m20, float m21, float m22)
{
	this->m00 = m00;
	this->m01 = m01;
	this->m02 = m02;
	this->m10 = m10;
	this->m11 = m11;
	this->m12 = m12;
	this->m20 = m20;
	this->m21 = m21;
	this->m22 = m22;
}


CMelonMatrix3::~CMelonMatrix3()
{
}

CMelonVector3D CMelonMatrix3::operator*(const CMelonVector3D & rhs) const
{
	return CMelonVector3D(rhs.m_fX*m00 + rhs.m_fY*m01 + rhs.m_fZ*m02,
						rhs.m_fX*m10 + rhs.m_fY*m11 + rhs.m_fZ*m12,
						rhs.m_fX*m20 + rhs.m_fY*m21 + rhs.m_fZ*m22);
}

void CMelonMatrix3::Update(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
	this->m00 = m00;
	this->m01 = m01;
	this->m02 = m02;
	this->m10 = m10;
	this->m11 = m11;
	this->m12 = m12;
	this->m20 = m20;
	this->m21 = m21;
	this->m22 = m22;
}

CMelonVector3D CMelonMatrix3::GetXAxis() const
{
	return CMelonVector3D(m00, m10, m20);
}

CMelonVector3D CMelonMatrix3::GetYAxis() const
{
	return CMelonVector3D(m01, m11, m21);
}

CMelonVector3D CMelonMatrix3::GetZAxis()const
{
	return CMelonVector3D(m02, m12, m22);
}


CMelonMatrix3 CMelonMatrix3::CalculateInverseRotation()
{
	float determinant = m00 * (m11 * m22 - m12 * m21)
					+ m01 * (m12 *m20 - m10 * m22)
					+ m02 *(m10 * m21 - m11 * m20);

	return CMelonMatrix3(
		(m11*m22 - m12 * m21) / determinant, //
		(m21*m02 - m22 * m01) / determinant,
		(m01*m12 - m02 * m11) / determinant,
		(m12*m20 - m10 * m22) / determinant, //
		(m22*m00 - m20 * m02) / determinant,
		(m02*m10 - m00 * m12) / determinant,
		(m10*m21 - m11 * m20) / determinant, //
		(m20*m01 - m21 * m00) / determinant,
		(m00*m11 - m01 * m10) / determinant
		);
}

// Debug 
void CMelonMatrix3::printMatrix()
{
	std::cout << m00 << " " << m01 << " " << m02 << "\n"
		<< m10 << " " << m11 << " " << m12 << "\n"
		<< m20 << " " << m21 << " " << m22 << "\n";
}
