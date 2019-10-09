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

	//timeBeginPeriod(1);//今の時間をtimeに保存。
	//DWORD time = timeGetTime();
	//DWORD prevtime = 0;
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


			//if (time - prevtime < 1000 / 60) {

			device.UpdateKeyStatus(&directX);
			//DrawStart(&directX);



			render.Render(directX);

			//DrawEnd(directX);

			//time = timeGetTime();
		//}




		//prevtime = time;
		}

	}

	//timeEndPeriod(1);

	// エンジン終了
	EndEngine(directX);
	render.FreeDx(directX);

	//　クラスの開放処理
	SingletonFinalizer::finalize();

	return 0;
}

