#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

#include <vector>
#include "kickflipSmartPointer.h"
#include "kickflipGraphicDevice.h"

namespace kickflip
{
	class DebugFont
	{
	public:
		static DebugFont& GetInstance()
		{
			static DebugFont kInstance;
			return kInstance;
		}
		virtual ~DebugFont(void)
		{
		}

	private:
		DebugFont(void)
			: m_rpGDev(NULL)
			, m_pD3DFont(NULL)

		{}

		GraphicDeviceRPtr m_rpGDev;
		ID3DXFont *m_pD3DFont;		// フォントオブジェクトポインタ
		D3DXFONT_DESC m_kD3DFD;	// フォント属性


		struct DebugFontObject : public ReferenceObject
		{
			std::string str;	// 文字
			int iX;				// 表示位置
			int iY;

			DebugFontObject(int iX_, int iY_, const char* str_)
				: str(str_)
				, iX(iX_)
				, iY(iY_)
			{}
		};
		typedef RefPtr<DebugFontObject> DebugFontObjectRPtr;
		std::vector<DebugFontObjectRPtr> m_kFontObjectList;

	public:
		void Initialize(GraphicDeviceRPtr rpGDev)
		{
			m_rpGDev = rpGDev; 
			m_kD3DFD.Height = 14;
			m_kD3DFD.Width  = 7;
			m_kD3DFD.Weight = 500;
			m_kD3DFD.MipLevels = D3DX_DEFAULT;
			m_kD3DFD.Italic = false;
			m_kD3DFD.CharSet = DEFAULT_CHARSET;
			m_kD3DFD.OutputPrecision = OUT_DEFAULT_PRECIS;
			m_kD3DFD.Quality = DEFAULT_QUALITY;
			m_kD3DFD.PitchAndFamily = FIXED_PITCH | FF_MODERN;
			memset( m_kD3DFD.FaceName, 0, sizeof( m_kD3DFD.FaceName ) );

			D3DXCreateFontIndirect( m_rpGDev->GetDevice(), &m_kD3DFD, &m_pD3DFont );
		}
		void Print(int iX, int iY, const char* str)
		{
			DebugFontObjectRPtr rpFontObject = new DebugFontObject(iX,iY,str);
			m_kFontObjectList.push_back(rpFontObject);
		}
		void Render()
		{
			if(NULL == m_rpGDev || NULL == m_pD3DFont) return;
			for(auto ite = m_kFontObjectList.begin(); ite!=m_kFontObjectList.end(); ite++)
			{
				RECT R;
				SetRect( &R, 0, 0, 0, 0 );
				m_pD3DFont->DrawText( NULL, (*ite)->str.c_str(), -1, &R, DT_LEFT | DT_CALCRECT , 0xffffffff );
				OffsetRect( &R, (*ite)->iX * m_kD3DFD.Width, (*ite)->iY * m_kD3DFD.Height );
				m_pD3DFont->DrawText( NULL, (*ite)->str.c_str(), -1, &R, DT_LEFT , 0xffffffff );
			}
			m_kFontObjectList.clear();

		}
#define DebugPrint( x,y, str, ... ) \
	{ \
		TCHAR c[2048]; \
		_stprintf_s( c, str, __VA_ARGS__ );	\
		DebugFont::GetInstance().Print(x,y,c);	\
	}


	};
}

