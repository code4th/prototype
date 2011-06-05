#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

#include "kickflipSmartPointer.h"
#include "kickflipThread.h"
#include "kickflipEmbedded.h"
#include "kickflipDebugFont.h"

namespace kickflip
{
	SmartPtr(FrameRate);
	class FrameRate :public ReferenceObject
	{
	public:
		FrameRate(GraphicDeviceRPtr rpGraphicDevice)
			: m_rpGraphicDevice(rpGraphicDevice)
			, m_uiLastUpdateTime(Time::GetRealTimeMilliSecond())
			, m_uiUpdateInterval(1000/60)
			, m_uiSuccessCount(0)
			, m_uiFailCount(0)
			, m_uiPassedCount(0)

		{}

		virtual ~FrameRate(void){}

		void Stabilize()
		{
			DebugPrint(10,12,"s:%d f:%d p:%d",m_uiSuccessCount,m_uiFailCount,m_uiPassedCount);
			int iIntervalMilliSec = m_uiUpdateInterval - ( Time::GetRealTimeMilliSecond() - m_uiLastUpdateTime);
			if(0<iIntervalMilliSec)
			{
				// �܂��]�T������Ƃ�
				LPDIRECT3DDEVICE9 pD3Dev = m_rpGraphicDevice->GetDevice();
				D3DRASTER_STATUS rs;
				unsigned int iLastLine = 0;
				while(true){
					if(D3D_OK!=pD3Dev->GetRasterStatus(0,&rs)){
						// ���s
						break;
					}
					if(TRUE == rs.InVBlank)
					{
						// �����A���@�ւɓ�������
						++m_uiSuccessCount;
						break;
					}else{
						// �܂����Ԃ���Ȃ�
						if(iLastLine>rs.ScanLine)
						{
							// �s���߂���!
							++m_uiPassedCount;
							break;
						}else{
							// ���݂̃��C����ۑ�
							iLastLine = rs.ScanLine;
							if(700>iLastLine)
								Sleep(1);
						}
					}
				};
			}else{
				// �����߂��Ă���̂Ői�߂Ă��܂��܂�
				++m_uiFailCount;
			}

			m_uiLastUpdateTime = Time::GetRealTimeMilliSecond();

		}
	private:
		GraphicDeviceRPtr	m_rpGraphicDevice;
		unsigned int m_uiLastUpdateTime;
		unsigned int m_uiUpdateInterval;
	public:
		unsigned int m_uiSuccessCount;
		unsigned int m_uiFailCount;
		unsigned int m_uiPassedCount;

	};
}

