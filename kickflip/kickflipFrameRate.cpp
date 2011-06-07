

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
				// まだ余裕があるとき
				LPDIRECT3DDEVICE9 pD3Dev = Framework::Get().GetGraphicDevice()->GetDevice();
				unsigned int iLastLine = 0;

				while(true){
					D3DRASTER_STATUS rs;
					if(D3D_OK!=pD3Dev->GetRasterStatus(0,&rs)){
						// 失敗
						break;
					}

					if(TRUE == rs.InVBlank)
					{
						// 垂直帰線機関に入ったよ
						++m_uiSuccessCount;
						break;
					}
					else{
						// まだ時間じゃない
						if(iLastLine>rs.ScanLine)
						{
							// 行き過ぎた!
							++m_uiPassedCount;
							break;
						}else{
							// 現在のラインを保存
							iLastLine = rs.ScanLine;
							if(700>iLastLine)
								Sleep(1);
						}
					}
				};

			}else{
				// もう過ぎているので進めてしまいます
				++m_uiFailCount;
			}
			m_uiLastUpdateTime = Time::GetRealTimeMilliSecond();

		}

}