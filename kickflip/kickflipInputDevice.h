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
#include "kickflipDebugFont.h"

namespace kickflip
{
	SmartPtr(InputDevice);
	class InputDevice :public ReferenceObject
	{
	public:
		InputDevice(void)
		{}

		virtual ~InputDevice(void){}

		class GamePad
		{
		private:
			friend class InputDevice;
		public:
			enum { MAX_NUM = 4};
			enum {
				L_UP			= 0x0001,	//	XINPUT_GAMEPAD_DPAD_UP   
				L_DOWN			= 0x0002,	//	XINPUT_GAMEPAD_DPAD_DOWN 
				L_LEFT			= 0x0004,	//	XINPUT_GAMEPAD_DPAD_LEFT 
				L_RIGHT			= 0x0008,	//	XINPUT_GAMEPAD_DPAD_RIGHT

				START			= 0x0010,	//	XINPUT_GAMEPAD_START
				BACK			= 0x0020,	//	XINPUT_GAMEPAD_BACK 

				L_THUMB			= 0x0040,	//	XINPUT_GAMEPAD_LEFT_THUMB 
				R_THUMB			= 0x0080,	//	XINPUT_GAMEPAD_RIGHT_THUMB

				L1				= 0x0100,	//	XINPUT_GAMEPAD_LEFT_SHOULDER 
				R1				= 0x0200,	//	XINPUT_GAMEPAD_RIGHT_SHOULDER

				R_DOWN			= 0x1000,	//	XINPUT_GAMEPAD_A
				R_RIGHT			= 0x2000,	//	XINPUT_GAMEPAD_B
				R_LEFT			= 0x4000,	//	XINPUT_GAMEPAD_X
				R_UP			= 0x8000,	//	XINPUT_GAMEPAD_Y

				// アナログ拡張
				L2				= 0x10000,
				R2				= 0x20000,

				// 別名
				A				= R_DOWN,
				B				= R_RIGHT,
				X				= R_LEFT,
				Y				= R_UP,
			};
			typedef unsigned int Buttons;

			struct InputState{
				Buttons on;
				Buttons off;
				Buttons pressed;
				Buttons released;
				BYTE l2;
				BYTE r2;
				SHORT lx;
				SHORT ly;
				SHORT rx;
				SHORT ry;
			};
		public:
			typedef std::vector<InputState> InputStateLog;
		private:
			bool m_bIsConnect;
			XINPUT_CAPABILITIES m_kCaps;
			InputState m_kInputState;
			InputStateLog m_kInputStateLog;
		public:
			GamePad()
				: m_bIsConnect(false)
			{
				Reset();
			}
			virtual ~GamePad(){}
			void Reset()
			{
				memset(&m_kCaps,0,sizeof(XINPUT_CAPABILITIES));
				memset(&m_kInputState,0,sizeof(InputState));
				m_kInputStateLog.clear();
			}

			void UpdateState()
			{
				const unsigned int max = m_kInputStateLog.size();
				// offはすでに設定してある
				for(unsigned int i=0; max>i; i++)
				{
					InputState& kStatePre = m_kInputState;
					if(0<i) kStatePre = m_kInputStateLog[i-1];
					InputState& kState = m_kInputStateLog[i];
					// 前回offで今回onのもの
					kState.pressed = kStatePre.off & kState.on;
					// 前回onで今回offのもの
					kState.released = kStatePre.on & kState.off;
				}
			}

		private:
			const InputState& GetState() const { return m_kInputState;}
		public:
			const InputStateLog& GetStateLog() const { return m_kInputStateLog;}
			const bool IsConnect() const { return m_bIsConnect;}
			const bool IsOn(const Buttons kButton) const { return 0!=(m_kInputState.on&kButton);}
			const bool IsPressed(const Buttons kButton) const { return 0!=(m_kInputState.pressed&kButton);}

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
			DWORD WINAPI DebugXInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState)
			{
				if(0!=dwUserIndex) return !ERROR_SUCCESS;
				static int i=0;
				XINPUT_GAMEPAD kGamePad;
				memset(&kGamePad,0,sizeof(XINPUT_GAMEPAD));
				switch(i%4)
				{
				case 0:
					kGamePad.wButtons|= GamePad::A;
					break;
				case 1:
					kGamePad.wButtons|= GamePad::A|GamePad::B;
					break;
				case 2:
					kGamePad.wButtons|= GamePad::B;
					break;
				case 3:
					break;
				}
				i++;
				pState->Gamepad = kGamePad;

				return ERROR_SUCCESS;
			}

			virtual unsigned int Execute(kickflip::Thread* pThread)
			{
				while(true)
				{
					int iIntervalMilliSec = m_uiUpdateInterval - ( Time::GetRealTimeMilliSecond() - m_uiLastUpdateTime);
					if(static_cast<int>(m_uiUpdateInterval)<iIntervalMilliSec) iIntervalMilliSec = m_uiUpdateInterval;
//					DebugTrace("sleep:%dms\n",iIntervalMilliSec);
					if(0>=iIntervalMilliSec)
					{
						// 過ぎたので速攻
						break;
					}else
					if(2>=iIntervalMilliSec)
					{
						// 残り2ms以下はもう抜ける
						Sleep(iIntervalMilliSec);
						break;
					}else{
						// 半分やって様子見
//						Sleep( iIntervalMilliSec/2 );
						Sleep( iIntervalMilliSec );
					}
				}

				DebugTrace("update:%dms\n",Time::GetRealTimeMilliSecond()-m_uiLastUpdateTime);
				m_uiLastUpdateTime = Time::GetRealTimeMilliSecond();

				for(auto idx = 0; GamePad::MAX_NUM>idx; idx++)
				{
					GamePad& kPad = m_kGamePad[idx];
					XINPUT_STATE	kInputState;
					const bool		bIsLastConnect = kPad.m_bIsConnect;
//					kPad.m_bIsConnect = ( XInputGetState(idx,&kInputState) == ERROR_SUCCESS ) ? true : false;
					kPad.m_bIsConnect = ( DebugXInputGetState(idx,&kInputState) == ERROR_SUCCESS ) ? true : false;

					if( false == kPad.m_bIsConnect ) continue;

					bool bIsInsert = ( !bIsLastConnect &&  kPad.m_bIsConnect ) ? true : false;

					if( bIsInsert )
					{
						kPad.Reset();
						XInputGetCapabilities( idx, XINPUT_FLAG_GAMEPAD, &kPad.m_kCaps );
					}
					GamePad::InputState kMyState;
					kMyState.on = kInputState.Gamepad.wButtons;
					kMyState.lx = kInputState.Gamepad.sThumbLX;
					kMyState.ly = kInputState.Gamepad.sThumbLY;
					kMyState.rx = kInputState.Gamepad.sThumbRX;
					kMyState.ry = kInputState.Gamepad.sThumbRY;
					kMyState.l2 = kInputState.Gamepad.bLeftTrigger;
					kMyState.r2 = kInputState.Gamepad.bRightTrigger;
					// 拡張
					kMyState.on |= ( XINPUT_GAMEPAD_TRIGGER_THRESHOLD < kMyState.l2 ) ? GamePad::L2 : 0;
					kMyState.on |= ( XINPUT_GAMEPAD_TRIGGER_THRESHOLD < kMyState.r2 ) ? GamePad::R2 : 0;

					kMyState.off = ~kMyState.on;
					kMyState.pressed = 0;
					kMyState.released = 0;
					Lock();
					kPad.m_kInputStateLog.push_back(kMyState);
					Unlock();
				}
//				DebugTrace("exec:%dms\n",Time::GetRealTimeMilliSecond()-m_uiLastUpdateTime);

				Sleep(0);

				return 0;
			}
			InputDevice::GamePad& GetPad(unsigned int idx)
			{
				return m_kGamePad[idx];
			}

		private:
			InputDevice::GamePad m_kGamePad[InputDevice::GamePad::MAX_NUM];
			unsigned int m_uiLastUpdateTime;
			unsigned int m_uiUpdateInterval;
		};

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

			m_rpThread = Thread::Create( new InputStabilizer(), Thread::TIME_CRITICAL );
			if ( NULL == m_rpThread ) return false;

			m_rpThread->Resume();

			return true;
		}
		void Update()
		{
			m_rpThread->Lock();
			InputStabilizerRPtr rpInputStabilizer = m_rpThread->GetThreadFunction();
			for(auto idx = 0; GamePad::MAX_NUM>idx; idx++)
			{
				GamePad& kPadMapLog = rpInputStabilizer->GetPad(idx);
				m_kGamePad[idx] = kPadMapLog;

				kPadMapLog.m_kInputStateLog.clear();
			}
			m_rpThread->Unlock();

			for( auto idx = 0; GamePad::MAX_NUM>idx; idx++ )
			{
				GamePad& kPad = m_kGamePad[idx];
				kPad.UpdateState();
				if(true == kPad.m_kInputStateLog.empty())
				{
					// 履歴がなければ自分自身が最新
				}else{
					// 最新の状態をスタビライザーにフィードバック
					rpInputStabilizer->GetPad(idx).m_kInputState = kPad.m_kInputStateLog.back();
					// カレントのステータスは過去をすべて含む
					kPad.m_kInputState.on = 0;
					kPad.m_kInputState.pressed = 0;
					for(auto ite = kPad.m_kInputStateLog.begin(); kPad.m_kInputStateLog.end()!=ite; ite++)
					{
						kPad.m_kInputState.on|=ite->on;
						kPad.m_kInputState.pressed|=ite->pressed;
					}
				}


			}

		}
		const GamePad& GetGamePad(unsigned int idx) 
		{
			if(GamePad::MAX_NUM<=idx) return m_kGamePadEmpty;

			return m_kGamePad[idx];
		}
		std::string getStringFromButtons(unsigned int  kButtons)
		{
			std::string res;
			if(kButtons&GamePad::L_UP) res+="L_UP|";
			if(kButtons&GamePad::L_DOWN) res+="L_DOWN|";
			if(kButtons&GamePad::L_LEFT) res+="L_LEFT|";
			if(kButtons&GamePad::L_RIGHT) res+="L_RIGHT|";
			
			if(kButtons&GamePad::START) res+="START|";
			if(kButtons&GamePad::BACK) res+="BACK|";

			if(kButtons&GamePad::L_THUMB) res+="L_THUMB|";
			if(kButtons&GamePad::R_THUMB) res+="R_THUMB|";

			if(kButtons&GamePad::L1) res+="L1|";
			if(kButtons&GamePad::R1) res+="R1|";

			if(kButtons&GamePad::A) res+="A|";
			if(kButtons&GamePad::B) res+="B|";
			if(kButtons&GamePad::X) res+="X|";
			if(kButtons&GamePad::Y) res+="Y|";

			if(kButtons&GamePad::L2) res+="L2|";
			if(kButtons&GamePad::R2) res+="R2|";

			if(res.empty())
			{
				res="NONE";
			}else{
				res.erase(res.length()-1);
			}
			return res;
		}
		void DebugPrintGamePad(unsigned int x, unsigned int y)
		{
			unsigned int iLine = y;
			for(int i=0;i<InputDevice::GamePad::MAX_NUM; i++)
			{
				const GamePad& kGamePad = m_kGamePad[i];
				if(true == kGamePad.IsConnect())
				{

					std::string strButton;
					unsigned int idx =0;
					char buf[2048];
					sprintf_s(buf," cur:[{%s}/{%s}/{%s}]",getStringFromButtons(kGamePad.GetState().on).c_str(),getStringFromButtons(kGamePad.GetState().pressed).c_str(),getStringFromButtons(kGamePad.GetState().released).c_str());
					strButton+=buf;

					for(auto ite = kGamePad.GetStateLog().begin(); kGamePad.GetStateLog().end()!=ite; ite++)
					{
						sprintf_s(buf," %d:[{%s}/{%s}/{%s}]",idx,getStringFromButtons(ite->on).c_str(),getStringFromButtons(ite->pressed).c_str(),getStringFromButtons(ite->released).c_str());
						strButton+=buf;
						idx++;
					}

					DebugPrint(x,y++,"pad(%d):%d [%s]", i, kGamePad.GetStateLog().size(),strButton.c_str());
//					DebugTrace("pad(%d):%d [%s]\n", i, kGamePad.GetStateLog().size(),strButton.c_str());
				}else{
					DebugPrint(x,y++,"pad(%d):OFF", i);
				}
			}
		}

	};
}

