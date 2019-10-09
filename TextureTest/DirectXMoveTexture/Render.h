#ifndef RENDER_H
#define RENDER_H

#include <windows.h>
#include <d3dx9.h>
#include"DirectX.h"

class Render_
{
private:
	D3DXMATRIXA16 matWorld, matRotation, matPosition, matRotation2;
	D3DXMATRIXA16 matView, matProj;
	D3DLIGHT9 light;

	float fPosX = 0;
	float fPosY = 0;
	float fPosZ = 0;

	float fHeading = 0;
	float fPitch   = 0;
	float fBank    = 0;

public:
	void Render(DirectX directX);

	VOID FreeDx(DirectX directX);

	float m_PosX = 0;
	float m_PosY = 0;
	float m_PosZ = 0;

	float m_fHeading = 0;
	float m_fPitch   = 0;
	float m_fBank    = 0;
	

	int GetfPosX();
	int GetfPosY();
	int GetfPosZ();

	void SetfPosX(float posX);
	void SetfPosY(float posY);
	void SetfPosZ(float posZ);

	int GetfHeading();
	int GetfPitch();
	int GetfBank();

	void SetfHeading(float Heading);
	void SetfPitch(float Pitch);
	void SetfBank(float Bank);

};

#endif
