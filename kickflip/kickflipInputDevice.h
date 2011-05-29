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
		InputDevice(void)
			: m_pInputStabilizer(NULL)
		{}

		virtual ~InputDevice(void){}

		class GamePad
		{
		private:
			friend class InputDevice;
		public:
			enum { MAX_NUM = 4};

			struct InputState{
				XINPUT_GAMEPAD on;
				XINPUT_GAMEPAD off;
				XINPUT_GAMEPAD pressed;
				XINPUT_GAMEPAD released;
			};

		public:
			GamePad()
				: m_bIsConnect(false)
			{
				Reset();
			}
			virtual ~GamePad(){}
			void UpdateState()
			{
				const unsigned int max = m_kInputStateLog.size();
				for(unsigned int i=0; max>i; i++)
				{
					InputState& kState = m_kInputStateLog[i];
					kState.off.wButtons = ~kState.on.wButtons;
				}

				for(unsigned int i=1; max>i; i++)
				{
					InputState& kStatePre = m_kInputStateLog[i-1];
					InputState& kState = m_kInputStateLog[i];

					kState.pressed.wButtons = kStatePre.off.wButtons & kState.on.wButtons;
					kState.released.wButtons = ~kState.pressed.wButtons;
				}
			}
			typedef std::vector<InputState> InputStateLog;
		private:
			bool m_bIsConnect;
			XINPUT_CAPABILITIES m_kCaps;
			InputState m_kInputState;
			InputStateLog m_kInputStateLog;

		public:
			void Reset()
			{
				memset(&m_kInputState,0,sizeof(InputState));
				memset(&m_kCaps,0,sizeof(XINPUT_CAPABILITIES));
			}
			const InputState& GetState() const { return m_kInputState;}
			const InputStateLog& GetStateLog() const { return m_kInputStateLog;}
			const bool IsConnect() const { return m_bIsConnect;}

		};
	private:
		SmartPtr(InputStabilizer);
		class InputStabilizer :public ThreadFunction
		{
		public:
			InputStabilizer()
				: ThreadFunction(true,0)
				, m_uiLastUpdateTime(Time::GetRealTimeMilliSecond())
				, m_uiUpdateInterval(1000/60)
			{}
			virtual ~InputStabilizer()
			{}
			void SetUpdateInterval(const unsigned int uiUpdateIntervalMillisecond)
			{
				m_uiUpdateInterval = uiUpdateIntervalMillisecond;
			}
			virtual unsigned int Execute(kickflip::Thread* pThread)
			{
				while(true)
				{
					int iIntervalMilliSec = m_uiUpdateInterval - ( Time::GetRealTimeMilliSecond() - m_uiLastUpdateTime);
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

				DebugOutput("update:%dms\n",Time::GetRealTimeMilliSecond()-m_uiLastUpdateTime);
				m_uiLastUpdateTime = Time::GetRealTimeMilliSecond();

				m_kLock.Enter();
				for(auto idx = 0; GamePad::MAX_NUM>idx; idx++)
				{
					GamePad& kPad = m_kGamePad[idx];
					XINPUT_STATE	kInputState;
					const bool		bIsLastConnect = kPad.m_bIsConnect;
					kPad.m_bIsConnect = ( XInputGetState(idx,&kInputState) == ERROR_SUCCESS ) ? true : false;

					if( false == kPad.m_bIsConnect ) continue;

					bool bIsInsert = ( !bIsLastConnect &&  kPad.m_bIsConnect ) ? true : false;

					if( bIsInsert )
					{
						kPad.Reset();
						XInputGetCapabilities( idx, XINPUT_FLAG_GAMEPAD, &kPad.m_kCaps );
					}
					GamePad::InputState kState;
					kState.on = kInputState.Gamepad;
					kPad.m_kInputStateLog.push_back(kState);
				}
				m_kLock.Exit();

				return 0;
			}
			InputDevice::GamePad& GetPad(unsigned int idx)
			{
				return m_kGamePad[idx];
			}

			Lock m_kLock;
		private:
			InputDevice::GamePad m_kGamePad[InputDevice::GamePad::MAX_NUM];
			unsigned int m_uiLastUpdateTime;
			unsigned int m_uiUpdateInterval;
		};

	private:
		GamePad m_kGamePad[GamePad::MAX_NUM];
		GamePad m_kGamePadEmpty;
		ThreadRPtr m_rpThread;
		InputStabilizer* m_pInputStabilizer;

	public:
		bool Initialize()
		{
			m_kGamePadEmpty.Reset();
			for(auto idx = 0; GamePad::MAX_NUM>idx; idx++)
			{
				m_kGamePad[idx].Reset();
			}

			m_rpThread = Thread::Create( new InputStabilizer() );
			if ( NULL == m_rpThread ) return false;

			m_pInputStabilizer = static_cast<InputStabilizer*>( static_cast<ThreadFunction*>( m_rpThread->GetFunction() ) );
			m_rpThread->Resume();

			return true;
		}
		void Update()
		{

			m_pInputStabilizer->m_kLock.Enter();
			for(auto idx = 0; GamePad::MAX_NUM>idx; idx++)
			{
				GamePad& kPadMapLog = m_pInputStabilizer->GetPad(idx);
				m_kGamePad[idx] = kPadMapLog;

				kPadMapLog.m_kInputStateLog.clear();
			}
			m_pInputStabilizer->m_kLock.Exit();

			for( auto idx = 0; GamePad::MAX_NUM>idx; idx++ )
			{
				GamePad& kPad = m_kGamePad[idx];
				kPad.UpdateState();
				if(true == kPad.m_kInputStateLog.empty())
				{
					// 履歴がなければ自分自身が最新
				}else{
					// 履歴の一番最近のものを取得
					m_kGamePad[idx].m_kInputState = m_kGamePad[idx].m_kInputStateLog.back();
				}
			}

		}
		const GamePad& GetGamePad(unsigned int idx) 
		{
			if(GamePad::MAX_NUM<=idx) return m_kGamePadEmpty;

			return m_kGamePad[idx];
		}

	};
}

