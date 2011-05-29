#pragma once

/*
kickflipFramework アプリケーション初期化のラッパ
*/

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include <stdarg.h>
#include <cstddef>

#include "kickFlipGraphicDevice.h"
#include "kickFlipInputDevice.h"
#include "kickFlipThread.h"

namespace kickflip
{

	class Framework
	{
	public:
		static Framework* ms_pInstance;

		Framework(void);
		virtual ~Framework(void);

		// windows設定
		void SetHInstance(HINSTANCE hInstance){ m_hInstance = hInstance; }
		void SetCommandLine(const char* pCmdLine){ m_kCommandLine = pCmdLine; }

		// 起動
		int Boot();

		bool Initialize();
		void MainLoop();
		void Finalize();

		virtual void ExecOnceBeforeUpdate(){}
		virtual void Update();
		virtual void ExecOnceAfterUpdate(){}

		virtual void UpdateFrame(){};


		void Shutdown(){ PostMessage(m_hWnd, WM_DESTROY, 0, 0); }
		LPDIRECT3DDEVICE9 GetGraphicDevice() { return m_rpGraphicDevice->GetDevice(); }
		GraphicShader& GetGraphicShader() { return m_rpGraphicDevice->GetShader(); }
		const InputDevice::GamePad& GetGamePad(unsigned int idx) { return m_rpInputDevice->GetGamePad(idx); }

	private:
		HWND m_hWnd;
		HINSTANCE m_hInstance;
		std::string m_kCommandLine;

		GraphicDeviceRPtr	m_rpGraphicDevice;
		InputDeviceRPtr		m_rpInputDevice;

	protected:
		bool InitializeWindow(_TCHAR* name, const unsigned int uiWidth, const unsigned int uiHeight);
		bool InitializeGraphicSystem(const unsigned int uiWidth, const unsigned int uiHeight);
		bool InitializeInputSystem();
/*
		SmartPtr(GamePadWatcher);
		class GamePadWatcher :public ThreadFunction
		{
		public:
			GamePadWatcher()
				: ThreadFunction(true,0)
				, m_iLastUpdateTime(Time::GetRealTimeMilliSecond())
			{
			}
			virtual ~GamePadWatcher()
			{
			}
			virtual unsigned int Execute(kickflip::Thread* pThread)
			{
				const unsigned int iFPS = 1000/60;
				while(true)
				{
					int iIntervalMilliSec = iFPS - ( Time::GetRealTimeMilliSecond() - m_iLastUpdateTime);
					if(0>iIntervalMilliSec)
					{
						// 過ぎたので速攻
						Sleep(0);
						break;
					}else
					if(2>=iIntervalMilliSec)
					{
						// 残り2ms以下はもう抜ける
						Sleep(iIntervalMilliSec);
						break;
					}else{
						// 半分やって様子見
						Sleep( iIntervalMilliSec/2 );
					}
				}
				DebugOutput("update:%dms\n",Time::GetRealTimeMilliSecond()-m_iLastUpdateTime);
				m_iLastUpdateTime = Time::GetRealTimeMilliSecond();

				m_kLock.Enter();
				for(auto idx = 0; GamePad::MAX_NUM>idx; idx++)
				{
					GamePad& kPad = m_kGamePad[idx];
					XINPUT_STATE	InputState;
					const bool		bIsLastConnect = kPad.m_bIsConnect;
					kPad.m_bIsConnect = ( XInputGetState(idx,&InputState) == ERROR_SUCCESS ) ? true : false;

					if( false == kPad.m_bIsConnect ) continue;

					bool bIsInsert = ( !bIsLastConnect &&  kPad.m_bIsConnect ) ? true : false;

					if( bIsInsert )
					{
						kPad.Reset();
						XInputGetCapabilities( idx, XINPUT_FLAG_GAMEPAD, &kPad.m_kCaps );
					}
					kPad.m_kInputStateLog.push_back(InputState.Gamepad);

				}
				m_kLock.Exit();

				return 0;
			}
			GamePad& GetPad(unsigned int idx)
			{
				return m_kGamePad[idx];
			}

			Lock m_kLock;
		private:
			GamePad m_kGamePad[GamePad::MAX_NUM];
			unsigned int m_iLastUpdateTime;
		};
		GamePadWatcherRPtr m_rpGamePadWatcher;
		*/
	};

}