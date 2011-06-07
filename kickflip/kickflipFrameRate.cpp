

#include "kickflipFramework.h"
#include "kickflipFrameRate.h"

namespace kickflip
{
		void FrameRate::Stabilize()
		{
			DebugPrint(10,12,"s:%d f:%d p:%d",m_uiSuccessCount,m_uiFailCount,m_uiPassedCount);
			int iIntervalMilliSec = m_uiUpdateInterval - ( Time::GetRealTimeMilliSecond() - m_uiLastUpdateTime);
			if(0<iIntervalMilliSec)
			{
				// �܂��]�T������Ƃ�
				LPDIRECT3DDEVICE9 pD3Dev = Framework::Get().GetGraphicDevice()->GetDevice();
				unsigned int iLastLine = 0;

				while(true){
					D3DRASTER_STATUS rs;
					if(D3D_OK!=pD3Dev->GetRasterStatus(0,&rs)){
						// ���s
						break;
					}

					if(TRUE == rs.InVBlank)
					{
						// �����A���@�ւɓ�������
						++m_uiSuccessCount;
						break;
					}
					else{
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

}