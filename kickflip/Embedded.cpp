

#include "Embedded.h"
#include <deque>
#include <string>
#include <vector>

namespace kickflip
{

	std::string cformat( char *format, ...)
	{
		int bufsize = 1024; // 適当なサイズ
		std::vector<char>   buff(bufsize);
		va_list args;

		// 適当なバッファサイズで先ずは、vsnprintfを試す。
		// 出力がバッファサイズ以上の場合、VC++ .NET 2003の場合は -1、
		// glibc2.1以降は、書き込みに必要なサイズを返す。
		va_start(args, format);
		int vssize = vsnprintf_s( &buff[0], bufsize, bufsize, format, args);
		va_end(args);

		// vsnprintfが成功した場合終了する。
		if ( vssize >= 0 && vssize < bufsize ) {
			buff.resize(vssize);
			return std::string( buff.begin(), buff.end() );
		}

#ifdef _WIN32
		// VC++ .NET 2003 書き込みに必要なサイズを取得する。
		va_start(args, format);
		vssize = _vscprintf( format, args);
		va_end(args);
#endif

		if ( vssize < 0 ) throw std::runtime_error(format);

		// サイズを再割り当てし、再度試す
		buff.resize(vssize + 1);
		va_start(args, format);
		vssize = vsnprintf_s( &buff[0], vssize + 1, vssize + 1, format, args);
		va_end(args);
		if ( vssize < 0 ) throw std::runtime_error(format);
		buff.resize(vssize);
		return std::string( buff.begin(), buff.end() );
	}

	namespace Time
	{
		LARGE_INTEGER m_iFrequency;
		LARGE_INTEGER m_iInitialCounter;

		LARGE_INTEGER m_iFrameCounter;
		LARGE_INTEGER m_iFrameCounterLast;

		LONGLONG m_iFrameDelta;
		double m_fFrameDeltaSecond;
		LONGLONG m_iFrameDeltaMicroSecond;

		typedef std::deque<double> FPSLog;
		FPSLog m_fFPSLog;
		const unsigned int iMaxFPSLog = 30;
		double m_fFPSAve;

		LARGE_INTEGER GetFrameTimeCount() { return m_iFrameCounter; }
		LARGE_INTEGER GetRealTimeCount()
		{
			LARGE_INTEGER iCounter;
			QueryPerformanceCounter(&iCounter);
			return iCounter;
		}
		void Reset()
		{
			QueryPerformanceFrequency(&m_iFrequency);
			m_iFrameCounterLast = m_iFrameCounter = m_iInitialCounter = GetRealTimeCount();
			m_fFPSLog.clear();
			m_fFPSAve = 0.f;
			Update();
		}
		unsigned int GetPerformanceCounter()
		{
			LARGE_INTEGER iCounter;
			QueryPerformanceCounter(&iCounter);
			return iCounter.LowPart;
		}
		void Update()
		{
			m_iFrameCounterLast = m_iFrameCounter;
			m_iFrameCounter = GetRealTimeCount();

			m_iFrameDelta = m_iFrameCounter.QuadPart - m_iFrameCounterLast.QuadPart;
			m_fFrameDeltaSecond = static_cast<double>(
				static_cast<long double>(m_iFrameDelta) /
				static_cast<long double>(m_iFrequency.QuadPart) );
			m_iFrameDeltaMicroSecond = static_cast<LONGLONG>(
				static_cast<long double>(m_iFrameDelta*1000000) /
				static_cast<long double>(m_iFrequency.QuadPart) );

			static bool bIsFirst = true;
			if(false == bIsFirst)
			{
				if(0.f<m_fFrameDeltaSecond)
					m_fFPSLog.push_back(1.f/m_fFrameDeltaSecond);
				while(iMaxFPSLog<m_fFPSLog.size()) m_fFPSLog.pop_front();
			}else{
				bIsFirst = false;
			}

			if(0<m_fFPSLog.size())
			{
				m_fFPSAve = 0.f;
				for(FPSLog::iterator ite = m_fFPSLog.begin(); m_fFPSLog.end()!=ite; ite++)
					m_fFPSAve+=*ite;
				m_fFPSAve/=m_fFPSLog.size();
			}
		}
		double GetFrameTimeSecond()
		{
			return static_cast<double>(
				static_cast<long double>(GetFrameTimeCount().QuadPart - m_iInitialCounter.QuadPart) / 
				static_cast<long double>(m_iFrequency.QuadPart) );
		}
		double GetRealTimeSecond()
		{
			return static_cast<double>(
				static_cast<long double>(GetRealTimeCount().QuadPart - m_iInitialCounter.QuadPart) / 
				static_cast<long double>(m_iFrequency.QuadPart) );
		}
		unsigned int GetFrameTimeMilliSecond()
		{
			return static_cast<unsigned int>(
				static_cast<long double>(GetFrameTimeCount().QuadPart - m_iInitialCounter.QuadPart)*1000 /
				static_cast<long double>(m_iFrequency.QuadPart) );
		}
		unsigned int GetRealTimeMilliSecond()
		{
			return static_cast<unsigned int>(
				static_cast<long double>(GetRealTimeCount().QuadPart - m_iInitialCounter.QuadPart)*1000 /
				static_cast<long double>(m_iFrequency.QuadPart) );
		}
		LONGLONG GetFrameTimeMicroSecond()
		{
			return static_cast<LONGLONG>(
				static_cast<long double>(GetFrameTimeCount().QuadPart - m_iInitialCounter.QuadPart)*1000000 /
				static_cast<long double>(m_iFrequency.QuadPart) );
		}
		LONGLONG GetRealTimeMicroSecond()
		{
			return static_cast<LONGLONG>(
				static_cast<long double>(GetRealTimeCount().QuadPart - m_iInitialCounter.QuadPart)*1000000 /
				static_cast<long double>(m_iFrequency.QuadPart) );
		}
		double GetFrameDeltaTimeSecond()
		{
			return m_fFrameDeltaSecond;
		}
		double GetRealDeltaTimeSecond()
		{
			return static_cast<double>(
				static_cast<long double>(GetRealTimeCount().QuadPart - m_iFrameCounterLast.QuadPart) /
				static_cast<long double>(m_iFrequency.QuadPart) );
		}
		LONGLONG GetFrameDeltaTimeMicroSecond()
		{
			return m_iFrameDeltaMicroSecond;
		}
		LONGLONG GetRealDeltaTimeMicroSecond()
		{
			return static_cast<LONGLONG>(
				static_cast<long double>(GetRealTimeCount().QuadPart - m_iFrameCounterLast.QuadPart)*1000000 /
				static_cast<long double>(m_iFrequency.QuadPart) );
		}

		double GetFPS()
		{
			if(0<m_fFPSLog.size())
				return m_fFPSLog.back();
			return 0.f;
		}
		double GetFPSAve()
		{
			return m_fFPSAve;
		}

	}
}