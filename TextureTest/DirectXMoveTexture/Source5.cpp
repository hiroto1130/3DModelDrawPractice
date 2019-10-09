#include"Window.h"
#include"Engine.h"
#include"Singleton.h"
#include"Device.h"
#include"DrawTexture.h"
#include"Render.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmpLine, INT nCmdShow)
{

	HWND* hWnd;
	Window window = singleton<Window>::get_instance();
	DirectX directX = singleton<DirectX>::get_instance();
	Device device = singleton<Device>::get_instance();
	Texture texture = singleton<Texture>::get_instance();

	Render_ render;

	if ((hWnd = InitEngine(800, 600, hInst, &directX, window, device)) == NULL)
	{
		return 0;
	}

	timeBeginPeriod(1);//今の時間をtimeに保存。
	DWORD time = timeGetTime();
	DWORD prevtime = 0;
	MSG msg;

	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message != WM_QUIT)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				break;
			}
		}
		else
		{


			if (time - prevtime < 1000 / 60) {

			device.UpdateKeyStatus(&directX);
			//DrawStart(&directX);

			render.Render(directX);

			/*
			if (device.GetKeyStatus(DIK_UP))
			{
				render.m_PosY += 0.01;
				render.SetfPosY(render.m_PosY);
			}
			else if (device.GetKeyStatus(DIK_DOWN))
			{
				render.m_PosY -= 0.01;
				render.SetfPosY(render.m_PosY);
			}
			else if (device.GetKeyStatus(DIK_RIGHT))
			{
				render.m_PosX += 0.01f;
				render.SetfPosX(render.m_PosX);
			}
			else if (device.GetKeyStatus(DIK_LEFT))
			{
				render.m_PosX -= 0.01f;
				render.SetfPosX(render.m_PosX);
			}
			else if (device.GetKeyStatus(DIK_Z))
			{
				render.m_PosZ -= 0.01f;
				render.SetfPosZ(render.m_PosZ);
			}
			else if (device.GetKeyStatus(DIK_X))
			{
				render.m_PosZ += 0.01f;
				render.SetfPosZ(render.m_PosZ);
			}
			*/

			if (device.GetKeyStatus(DIK_UP))
			{
				render.m_fPitch += 0.01;
				render.SetfPitch(render.m_fPitch);
			}
			else if (device.GetKeyStatus(DIK_DOWN))
			{
				render.m_fPitch -= 0.01;
				render.SetfPitch(render.m_fPitch);
			}
			else if (device.GetKeyStatus(DIK_RIGHT))
			{
				render.m_fHeading += 0.01f;
				render.SetfHeading(render.m_fHeading);
			}
			else if (device.GetKeyStatus(DIK_LEFT))
			{
				render.m_fHeading -= 0.01f;
				render.SetfHeading(render.m_fHeading);
			}
			else if (device.GetKeyStatus(DIK_Z))
			{
				render.m_fBank -= 0.01f;
				render.SetfBank(render.m_fBank);
			}
			else if (device.GetKeyStatus(DIK_X))
			{
				render.m_fBank += 0.01f;
				render.SetfBank(render.m_fBank);
			}
					

			//DrawEnd(directX);

			time = timeGetTime();
		}




		prevtime = time;
		}

	}

	timeEndPeriod(1);

	// エンジン終了
	EndEngine(directX);
	render.FreeDx(directX);

	//　クラスの開放処理
	SingletonFinalizer::finalize();

	return 0;
}

