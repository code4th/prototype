
/*
kickflipFramework アプリケーション初期化のラッパ
*/

// Directx内のnewが別個にやってるのでnewをdefineする前に一旦読み込む
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include "kickflipFramework.h"
#include "kickflipEmbedded.h"
#include "kickflipThread.h"

#include "kickflipDebugFont.h"

#include <iostream> 
#include <string>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")


// メイン
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE , LPTSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
    // デバッグ時はメモリリークを調べる
    ::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif
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

	void Framework::InitSetting()
	{
		SetScreenHeight(720);
	}

	int Framework::Boot()
	{
		InitSetting();
		if(true == Initialize())
		{
//			SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
			timeBeginPeriod(1);
			MainLoop();
			timeEndPeriod(1);
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
		Time::Reset();
		if( false == InitializeWindow(_T("Prototype"), GetScreenWidth(), GetScreenHeight() ) ) return false;

		if( false == InitializeGraphicSystem( GetScreenWidth(), GetScreenHeight() ) ) return false;

		if( false == InitializeInputSystem() ) return false;

		m_rpFrameRate = new FrameRate();

		DebugFont::GetInstance().Initialize();
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
			}
		}

		ExecOnceAfterUpdate();
	}

	void Framework::Update()
	{
		Time::Update();
		m_rpInputDevice->Update();

		LPDIRECT3DDEVICE9 pD3Dev = m_rpGraphicDevice->GetDevice();
		if(NULL!=pD3Dev)
		{

			if(D3D_OK == pD3Dev->BeginScene())
			{
				pD3Dev->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(10,150,190), 1.0f, 0 );

				DebugFont::GetInstance().Render();

				UpdateFrame();

				pD3Dev->EndScene();

				m_rpFrameRate->Stabilize();

			}

			BeforePresent();
			pD3Dev->Present( NULL, NULL, NULL, NULL );
			AfterPresent();
		}

	}

	void Framework::Finalize()
	{
	}

}



