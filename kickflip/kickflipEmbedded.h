#pragma once
/*
	�g�ݍ��ݗp���\�b�h�Q
*/

#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

#include <mmsystem.h>
#include <deque>

namespace kickflip
{
#define DebugOutput( str, ... ) \
	{ \
		TCHAR c[2048]; \
		_stprintf_s( c, str, __VA_ARGS__ ); \
		OutputDebugString( c ); \
	}

	namespace Time
	{
		void Reset();
		void Update();
		unsigned int GetPerformanceCounter();

		LARGE_INTEGER GetFrameTimeCount();
		LARGE_INTEGER GetRealTimeCount();

		double GetFrameTimeSecond();
		double GetRealTimeSecond();
		unsigned int GetFrameTimeMilliSecond();
		unsigned int GetRealTimeMilliSecond();
		LONGLONG GetFrameTimeMicroSecond();
		LONGLONG GetRealTimeMicroSecond();

		double GetFrameDeltaTimeSecond();
		double GetRealDeltaTimeSecond();
		LONGLONG GetFrameDeltaTimeMicroSecond();
		LONGLONG GetRealDeltaTimeMicroSecond();

		double GetFPS();
		double GetFPSAve();

		inline LONGLONG MilliToMicroSec(int iMilliSecond)
		{
			return iMilliSecond*1000;
		}
		inline LONGLONG SecToMicroSec(float fSecond)
		{
			return static_cast<LONGLONG>(static_cast<long double>(fSecond)*static_cast<long double>(1000000));
		}
		inline int SecToMilliSec(float fSecond)
		{
			return static_cast<int>(static_cast<long double>(fSecond)*static_cast<long double>(1000));
		}
		inline double MicroToMilliSec(LONGLONG iMicroSecond)
		{
			return static_cast<long double>(iMicroSecond)/static_cast<long double>(1000);
		}
	}
}
