#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

#include "SmartPointer.h"
#include "Thread.h"
#include "Embedded.h"
#include "Debug/DebugFont.h"

namespace kickflip
{
	SmartPtr(FrameRate);
	class FrameRate :public ReferenceObject
	{
	public:
		FrameRate()
			: m_uiLastUpdateTime(Time::GetRealTimeMilliSecond())
			, m_uiUpdateInterval(1000/60)
			, m_uiSuccessCount(0)
			, m_uiFailCount(0)
			, m_uiPassedCount(0)

		{}

		virtual ~FrameRate(void){}

		void Stabilize();
	private:
		unsigned int m_uiLastUpdateTime;
		unsigned int m_uiUpdateInterval;
	public:
		unsigned int m_uiSuccessCount;
		unsigned int m_uiFailCount;
		unsigned int m_uiPassedCount;

	};
}

