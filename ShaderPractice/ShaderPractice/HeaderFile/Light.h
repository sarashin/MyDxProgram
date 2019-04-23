#pragma once
class Light
{
public:
	XMFLOAT4 m_LightVec;
	Light();
	Light(XMFLOAT3 LightVec);
	~Light();
};

