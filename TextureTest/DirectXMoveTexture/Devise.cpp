﻿#include"Device.h"


HRESULT Device::BuildDxDevice(HWND* hInst, DirectX* directX)
{
	//// InitD3d が初期化されているかどうか ////
	if (FAILED(InitD3d(hInst, directX)))
	{
		return E_FAIL;
	}

	//// InitDinput 初期化されているかどうか ////
	if (FAILED(InitDinput(hInst, directX)))
	{
		return E_FAIL;
	}

	//// Direct3DCreate9 が ////
	if (directX->pDirect3d == NULL)
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Device::InitD3d(HWND* hInst, DirectX* directX)
{
	if (NULL == (directX->pDirect3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return E_FAIL;
	}

	//// バッファ設定 ////

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	//// デバイス生成 ////

	if (FAILED(directX->pDirect3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, *hInst,
		D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &directX->pDevice)))
	{
		return E_FAIL;
	}

	// 3d // 
	
	// Xファイルからメッシュをロードする	
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX("Chips.x", D3DXMESH_SYSTEMMEM,
		directX->pDevice, NULL, &pD3DXMtrlBuffer, NULL,
		&directX->dwNumMaterials, &directX->pMesh)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", NULL, MB_OK);
		return E_FAIL;
	}
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	directX->pMeshMaterials = new D3DMATERIAL9[directX->dwNumMaterials];
	directX->pMeshTextures = new LPDIRECT3DTEXTURE9[directX->dwNumMaterials];

	for (DWORD i = 0; i < directX->dwNumMaterials; i++)
	{
		directX->pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
		directX->pMeshMaterials[i].Ambient = directX->pMeshMaterials[i].Diffuse;
		directX->pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(directX->pDevice,
				d3dxMaterials[i].pTextureFilename,
				&directX->pMeshTextures[i])))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
			}
		}
	}
	pD3DXMtrlBuffer->Release();
	// Zバッファー処理を有効にする
	directX->pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ライトを有効にする
	directX->pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// アンビエントライト（環境光）を設定する
	directX->pDevice->SetRenderState(D3DRS_AMBIENT, 0x00111111);
	// スペキュラ（鏡面反射）を有効にする
	directX->pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	return S_OK;

}

////// directinput //////

HRESULT Device::InitDinput(HWND* hInst, DirectX* directX)
{
	HRESULT hr;

	//// DirectInput8の作成 ////
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID * *)& directX->pDinput, NULL)))
	{
		return hr;
	}

	//// InputDeviceを作成 ////         * どの入力デバイスから情報を受け取るか
	if (FAILED(hr = directX->pDinput->CreateDevice(GUID_SysKeyboard, &directX->pkey, NULL)))
	{
		return hr;
	}

	//// デバイスのフォーマット設定,形式 ////　　　* キーボード c_dfDIKeyboard マウス c_dfDIMouse
	if (FAILED(hr = directX->pkey->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}

	//// 協調レベルを設定 ////    * フォアグラウンド、バックグラウンド 排他的、非排他的 
	if (FAILED(hr = directX->pkey->SetCooperativeLevel(*hInst, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}

	//// 権限の取得 ////
	directX->pkey->Acquire();

	return S_OK;
}

////// キーステータス更新関数 //////
void Device::UpdateKeyStatus(DirectX* directX)
{
	HRESULT hr = directX->pkey->Acquire();

	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		directX->pkey->GetDeviceState(sizeof(KeyState), &KeyState);
	}

}

////// キー入力関数 //////
bool Device::GetKeyStatus(int KeyNumber)
{

	if (KeyState[KeyNumber] & MASK_NUM)
	{

		return true;// 入力されたら true を返す

	}
	return false;// 入力されていなければ false を返す

}
