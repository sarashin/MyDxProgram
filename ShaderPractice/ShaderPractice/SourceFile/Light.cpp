#include "stdafx.h"
#include "Light.h"


Light::Light()
{
	m_LightVec = XMFLOAT4(1.0, 1.0, 1.0, 0.0);
}

Light::Light(XMFLOAT3 LightVec)
{
	m_LightVec = XMFLOAT4(LightVec.x, LightVec.y, LightVec.z, 0.0);
}


Light::~Light()
{
}
