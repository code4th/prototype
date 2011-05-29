#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <XInput.h>
#include <tchar.h>

#include <vector>
#include <map>
#include "kickflipSmartPointer.h"
#include "kickflipThread.h"
#include "kickflipEmbedded.h"

namespace kickflip
{
	SmartPtr(InputDevice);
	class InputDevice :public ReferenceObject
	{
	public:
		InputDevice(void){}

		virtual ~InputDevice(void){}

		class GamePad
		{
		private:
			friend class InputDevice;
		public:
			enum { MAX_NUM = 4};
		public:
			GamePad()
				: m_bIsConnect(false)
			{
				Reset();
			}
			virtual ~GamePad(){}
			typedef std::vector<XINPUT_GAMEPAD> InputStateLog;
		private:
			bool m_bIsConnect;
			XINPUT_GAMEPAD m_kInputState;
			XINPUT_CAPABILITIES m_kCaps;
			InputStateLog m_kInputStateLog;

		public:
			void Reset()
			{
				memset(&m_kInputState,0,sizeof(XINPUT_GAMEPAD));
				memset(&m_kCaps,0,sizeof(XINPUT_CAPABILITIES));
			}
			const XINPUT_GAMEPAD& GetState() const { return m_kInputState;}
			const InputStateLog& GetStateLog() const { return m_kInputStateLog;}
			const bool IsConnect() const { return m_bIsConnect;}

		};
	private:

	private:
		GamePad m_kGamePad[GamePad::MAX_NUM];
		GamePad m_kGamePadEmpty;
		ThreadRPtr m_rpThread;

	public:
		bool Initialize()
		{
			m_kGamePadEmpty.Reset();
			for(auto idx = 0; GamePad::MAX_NUM>idx; idx++)
			{
				m_kGamePad[idx].Reset();
			}
			return true;
		}
		void Update()
		{
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
				kPad.m_kInputState = InputState.Gamepad;
			}

/*

			m_rpGamePadWatcher->m_kLock.Enter();
			for(auto idx = 0; GamePad::MAX_NUM>idx; idx++)
			{
				GamePad& kPadMapLog = m_rpGamePadWatcher->GetPad(idx);
				m_kGamePad[idx] = kPadMapLog;

				kPadMapLog.m_kInputStateLog.clear();
			}
			m_rpGamePadWatcher->m_kLock.Exit();

			for(auto idx = 0; GamePad::MAX_NUM>idx; idx++)
			{
				GamePad& kPad = m_kGamePad[idx];
				if(true == kPad.m_kInputStateLog.empty())
				{
					// —š—ğ‚ª‚È‚¯‚ê‚Î©•ª©g‚ªÅV
				}else{
					// —š—ğ‚Ìˆê”ÔÅ‹ß‚Ì‚à‚Ì‚ğæ“¾
					m_kGamePad[idx].m_kInputState = m_kGamePad[idx].m_kInputStateLog.back();
				}
			}
*/
		}
		const GamePad& GetGamePad(unsigned int idx) 
		{
			if(GamePad::MAX_NUM<=idx) return m_kGamePadEmpty;

			return m_kGamePad[idx];
		}

	};
}

