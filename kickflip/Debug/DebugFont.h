#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

#include <vector>
#include "../SmartPointer.h"
#include "../GraphicDevice.h"

namespace kickflip
{
//#define DebugPrint( x,y, str, ... )

#define DebugPrint( x,y, str, ... ) \
	{ \
		TCHAR c[2048]; \
		_stprintf_s( c, str, __VA_ARGS__ );	\
		DebugFont::GetInstance().Print(x,y,c);	\
	}

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
			: m_pD3DFont(NULL)

		{}

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
		typedef std::vector<DebugFontObjectRPtr> FontObjectList;
		FontObjectList m_kFontObjectList;

	public:
		void Initialize();
		void Print(int iX, int iY, const char* str)
		{
			DebugFontObjectRPtr rpFontObject = new DebugFontObject(iX,iY,str);
			m_kFontObjectList.push_back(rpFontObject);
		}
		void Render();


	};
}

