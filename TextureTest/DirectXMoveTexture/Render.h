#ifndef RENDER_H
#define RENDER_H

#include <windows.h>
#include <d3dx9.h>
#include"DirectX.h"

class Render_
{
public:
	D3DXMATRIXA16 matWorld, matRotation, matPosition;
	D3DXMATRIXA16 matView, matProj;
	D3DLIGHT9 light;

	float fPosX;
	float fPosY;
	float fPosZ;

public:
	void Render(DirectX directX, float fPosX, float fPosY, float fPosZ);

	VOID FreeDx(DirectX directX);

	 int GetfPosX();
	 int GetfPosY();
	 int GetfPosZ();

	 void SetfPosX(float fposX);
	 void SetfPosY(float fposY);
	 void SetfPosZ(float fposZ);

};

#endif
