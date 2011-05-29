
/*
kickflipFramework アプリケーション初期化のラッパ
*/

#include "kickflipFramework.h"
#include "kickflipEmbedded.h"
#include "kickflipThread.h"

#include "kickflipDebugFont.h"

#include <iostream> 
#include <string>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// メイン
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpCmdLine, int nCmdShow)
{
	// windows情報設定
	kickflip::Framework::ms_pInstance->SetHInstance(hInstance);
	kickflip::Framework::ms_pInstance->SetCommandLine(lpCmdLine);
	// 処理移譲
	return kickflip::Framework::ms_pInstance->Boot();
}


namespace kickflip
{
	Framework* Framework::ms_pInstance = NULL;

	Framework::Framework(void)
		: m_hWnd(NULL)
		, m_hInstance(NULL)
		, m_kCommandLine()
		, m_rpGraphicDevice(NULL)
	{
		ms_pInstance = this;
	}

	Framework::~Framework(void)
	{
	}

	int Framework::Boot()
	{
		if(true == Initialize())
		{
			MainLoop();
		}

		Finalize();
		return 0;
	}

	bool Framework::InitializeWindow(_TCHAR* name, const unsigned int uiWidth, const unsigned int uiHeight)
	{
		// ウィンドウプロシージャ
		struct WP 
		{
			static LRESULT CALLBACK WndProc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam)
			{
				switch (mes)
				{
				case WM_DESTROY:
					PostQuitMessage(0);
					break;
				case WM_KEYDOWN:
					switch(wParam)
					{
						case VK_ESCAPE:
							DestroyWindow(hWnd);
							break;
					}
				default:
					return DefWindowProc(hWnd, mes, wParam, lParam);
				}
				return 0;
			}
		};

		// アプリケーションの初期化
		WNDCLASSEX wcex ={sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WP::WndProc, 0, 0, m_hInstance, NULL, NULL, (HBRUSH)(COLOR_WINDOW+1), NULL, name, NULL};
		if(!RegisterClassEx(&wcex))
			return false;
		RECT rect;
		SetRect( &rect, 0, 0, uiWidth, uiHeight );
		AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW, FALSE);
		if(!(m_hWnd = CreateWindow(name, name, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, m_hInstance, NULL)))
			return false;

		// ウィンドウ表示
		ShowWindow( m_hWnd, SW_SHOW );
		return true;
	}

	bool Framework::InitializeGraphicSystem(const unsigned int uiWidth, const unsigned int uiHeight)
	{
		m_rpGraphicDevice = new GraphicDevice();
		if(NULL == m_rpGraphicDevice) return false;
		return m_rpGraphicDevice->Initialize(m_hWnd, uiWidth, uiHeight);

	}
	bool Framework::InitializeInputSystem()
	{
		m_rpInputDevice = new InputDevice();
		if( false == m_rpInputDevice->Initialize() ) return false;
		return true;
	}

	bool Framework::Initialize()
	{
		const int h = 256;
		const int w = (int)((float)h*16.f/9.f);
		Time::Reset();
		if( false == InitializeWindow(_T("Prototype"), w, h ) ) return false;

		if( false == InitializeGraphicSystem( w, h ) ) return false;

		if( false == InitializeInputSystem() ) return false;


		DebugFont::GetInstance().Initialize(m_rpGraphicDevice);
		return true;
	}

	void Framework::MainLoop()
	{
		ExecOnceBeforeUpdate();

		bool isBreak = false;
		// main loop
		while(!isBreak)
		{
			MSG kMsg;
			if (PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
			{
				if (kMsg.message == WM_QUIT)
				{
					isBreak = true;
				}
				TranslateMessage(&kMsg);
				DispatchMessage(&kMsg);

			}
			else
			{
				Update();
				Sleep(0);
			}
		}

		ExecOnceAfterUpdate();
	}

	void Framework::Update()
	{
		Sleep(500);
		Time::Update();
		m_rpInputDevice->Update();

		LPDIRECT3DDEVICE9 pD3Dev = m_rpGraphicDevice->GetDevice();
		if(NULL!=pD3Dev)
		{
			pD3Dev->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(10,150,190), 1.0f, 0 );
			pD3Dev->BeginScene();

			DebugFont::GetInstance().Render();

			UpdateFrame();

			pD3Dev->EndScene();
			pD3Dev->Present( NULL, NULL, NULL, NULL );
		}
	}

	void Framework::Finalize()
	{
	}



}



