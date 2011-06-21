#pragma once

/*
kickflipFramework アプリケーション初期化のラッパ
*/

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include <stdarg.h>
#include <cstddef>

#include "kickflipEmbedded.h"
#include "kickFlipGraphicDevice.h"
#include "kickFlipInputDevice.h"
#include "kickFlipThread.h"
#include "kickFlipFrameRate.h"

namespace kickflip
{
	class Framework
	{
	public:
		static Framework* ms_pInstance;
		static Framework& Get(){ return *ms_pInstance;}
		Framework(void);
		virtual ~Framework(void);

		// windows設定
		void SetHInstance(HINSTANCE hInstance){ m_hInstance = hInstance; }
		void SetCommandLine(const char* pCmdLine){ m_kCommandLine = pCmdLine; }

		virtual void InitSetting();
		// 起動
		int Boot();

		bool Initialize();
		void MainLoop();
		void Finalize();

		virtual void ExecOnceBeforeUpdate(){}
		virtual void Update();
		virtual void ExecOnceAfterUpdate(){}

		virtual void UpdateFrame(){};
		virtual void BeforePresent(){};
		virtual void AfterPresent(){};

		void SetScreenSize( unsigned int uiWidth, unsigned int uiHeight) { m_uiWidth = uiWidth; m_uiHeight = uiHeight;}
		void SetScreenHeight( unsigned int uiHeight) { SetScreenSize( static_cast<unsigned int>(static_cast<float>(uiHeight)*16.f/9.f),uiHeight); }
		unsigned int GetScreenWidth() { return m_uiWidth; }
		unsigned int GetScreenHeight() { return m_uiHeight; }
		void Shutdown(){ PostMessage(m_hWnd, WM_DESTROY, 0, 0); }
		const GraphicDeviceRPtr& GetGraphicDevice()  { return m_rpGraphicDevice; }
		const GraphicShaderRPtr& GetGraphicShader()  { return m_rpGraphicDevice->GetShader(); }
		const InputDevice::GamePad& GamePad(unsigned int idx)  { return m_rpInputDevice->GetGamePad(idx); }
		const InputDeviceRPtr& GetInputDevice()  { return m_rpInputDevice; }

	private:
		HWND m_hWnd;
		HINSTANCE m_hInstance;
		std::string m_kCommandLine;

		GraphicDeviceRPtr	m_rpGraphicDevice;
		InputDeviceRPtr		m_rpInputDevice;
		FrameRateRPtr		m_rpFrameRate;

		unsigned int m_uiWidth;
		unsigned int m_uiHeight;

	protected:
		bool InitializeWindow(_TCHAR* name, const unsigned int uiWidth, const unsigned int uiHeight);
		bool InitializeGraphicSystem(const unsigned int uiWidth, const unsigned int uiHeight);
		bool InitializeInputSystem();

	};

}