#include"Render.h"


void  Render_::Render(DirectX directX,float fPosX,float fPosY,float fPosZ)
{
	// -------------------- ワールドトランスフォーム（絶対座標変換）-------------------- //
	// 自動回転

	/*
	D3DXMatrixRotationY(
		&matWorld,             // 操作結果を入れるD3DXMATRIX構造体へのポインタ(Y座標用)
		0                       // 回転角度(ラジアン)
	);
	D3DXMatrixRotationX(
		&matRotation, // 操作結果を入れるD3DXMATRIX構造体へのポインタ(X座標用)
		0.5f          // 回転角度(ラジアン)
	);

	D3DXMatrixMultiply(
		&matWorld,   // X , Y の結果を入れる構造体のポインタ
		&matWorld,   // Y座標の結果が入ったD3DXMATRIX構造体のポインタ
		&matRotation // X座標の結果が入ったD3DXMATRIX構造体のポインタ
	);

	directX.pDevice->SetTransform(
		D3DTS_WORLD, // デバイス状態変数(D3DTs\WORLD は 0〜255の範囲のインデックス値に変換)
		&matWorld    // 変更するD3DXMATRIX構造体のポインタ
	);
	*/

	// -------------------- ワールドトランスフォーム（絶対座標変換）-------------------- //
	// 操作

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matPosition, fPosX, fPosY, fPosZ);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);
	directX.pDevice->SetTransform(D3DTS_WORLD, &matWorld);


	// -------------------- ビュートランスフォーム（視点座標変換）--------------------- // 

	D3DXVECTOR3 vecEyePt( //カメラ（視点）位置
		0.0f, // X座標
		1.0f, // Y座標
		-3.0f // Z座標
	); 

	D3DXVECTOR3 vecLookatPt( //注視位置
		0.0f, // X座標
		0.0f, // Y座標
		0.0f // Z座標
	);

	D3DXVECTOR3 vecUpVec( //上方位置 (0.0f,1.0f,0.0f) が通常値
		0.0f, // X座標
		3.0f, // Y座標
		0.0f  // Z座標
	);

	D3DXMatrixLookAtLH(
		&matView,     // カメラの情報を入れるD3DXMATRIX構造体のポインタ
		&vecEyePt,    // カメラの位置の情報を持ったD3DXMATRIX構造体へのポインタ
		&vecLookatPt, // カメラの注視位置の情報を持ったD3DXMATRIX構造体へのポインタ
		&vecUpVec     // 上方位置の情報を持ったD3DXMATRIX構造体へのポインタ
	);

	directX.pDevice->SetTransform(
		D3DTS_VIEW, // デバイス状態変数(D3DTS_VIEW は ビュー変換行列として設定される変換行列)
		&matView	// 変更するD3DXMATRIX構造体のポインタ
	);

	// -------------------- プロジェクショントランスフォーム（射影変換）-------------------- //
	
	D3DXMatrixPerspectiveFovLH(
		&matProj,      // 結果を入れるD3DXMATRIX構造体へのポインタ
		D3DX_PI / 4,   // Y軸方向の視野(ラジアン単位)
		1.0f,		   // アスペクト比(ビュースペースの幅を高さで割ったもの)
		1.0f, 		   // 近景面のZ値(手前)
		100.0f		   // 遠景面のZ値(奥)
	);

	directX.pDevice->SetTransform(
		D3DTS_PROJECTION, // デバイス状態変数(投影変換行列として設定される変換行列)
		&matProj          // 変更するD3DXMATRIX構造体のポインタ
	);
	
	// ライトをあてる 白色で鏡面反射ありに設定

	D3DXVECTOR3 vecDirection( // 光を当てる方向
		0, // X座標
		0, // Y座標
		1  // Z座標
	);

	ZeroMemory(&light, sizeof(D3DLIGHT9)); // 中身の初期化
	light.Type = D3DLIGHT_DIRECTIONAL; // 光源の種類の指定(D3DLIGHT_DIRECTIONAL は 指向性光源)
	
	// ライトの照射している色 //
	light.Diffuse.r = 1.0f; // r値
	light.Diffuse.g = 1.0f; // g値
	light.Diffuse.b = 1.0f; // b値
	
	// スペキュラー(D3DLIGHT_DIRECTIONAL)の色の設定 //
	light.Specular.r = 1.0f; // r値
	light.Specular.g = 1.0f; // g値
	light.Specular.b = 1.0f; // b値

	D3DXVec3Normalize( // 3Dベクトルの正規化されたバージョンの作成
		(D3DXVECTOR3*)& light.Direction,  // 結果を入れるD3DXVECTOR3構造体へのポインタ(ここではSetLightで使用する為D3DLIGHT9構造体で作っている)
		&vecDirection                     // 設定情報が入った3DXVECTOR3構造体へのポインタ
	);

	light.Range = 200.0f; // ライトの有効距離

	directX.pDevice->SetLight(
		0,     // 設定する照明プロパティのセットのゼロベースのインデックス
		&light // 設定する照明の情報を持ったD3DLIGHT9構造体へのポインタ 
	);
	directX.pDevice->LightEnable(
		0,    // 照明パラメーターのセットのゼロベースのインデックス
		TRUE  // 照明情報のセットが有効か無効か
	);


	// -------------------- レンダリング -------------------- //

	directX.pDevice->Clear(
		0, // pRectsの配列内の長方形の数
		NULL, // クリアする四角形を記述するD3DRECT構造体の配列へのポインター
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // クリアするサーフェスを指定
		D3DCOLOR_XRGB(100, 100, 100), // カラーのレンダーターゲットをクリア
		1.0f, // 新しいz値までの深度バッファーをクリア
		0 // ステンシルバッファを0〜2ⁿ-1の範囲のこの新しい値にクリア
	);
	int a;

	if (SUCCEEDED( a = (directX.pDevice->BeginScene()))) // レンダリング完了チェック
	{
		for (DWORD i = 0; i < directX.dwNumMaterials; i++)
		{
			directX.pDevice->SetMaterial(&directX.pMeshMaterials[i]); // 設定するマテリアルプロパティの情報を持ったD3DMATERIAL9構造体へのポインタ
			directX.pDevice->SetTexture(
				0, // ゼロベースのサンプラー番号
				directX.pMeshTextures[i] // 設定されるテクスチャを表すIDirect3DBaseTexture9へのポインタ
			);
			directX.pMesh->DrawSubset(i); // メッシュのサブセットを描画
		}
		directX.pDevice->EndScene();
	}
	directX.pDevice->Present(NULL, NULL, NULL, NULL); // 開始されたシーンを終了
}


// ---------------------------------------------------------------------------------------------


#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
//
//VOID FreeDx()
// 作成したDirectXオブジェクトの開放
VOID Render_::FreeDx(DirectX directX)
{
	SAFE_RELEASE(directX.pMesh);
	SAFE_RELEASE(directX.pDevice);
	SAFE_RELEASE(directX.pDirect3d);
}]

int GetfPosX();
int GetfPosY();
int GetfPosZ();



