

#include "DebugFont.h"
#include "../Framework.h"

namespace kickflip
{
	void DebugFont::Initialize()
	{
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
		D3DXCreateFontIndirect( Framework::Get().GetGraphicDevice()->GetDevice(), &m_kD3DFD, &m_pD3DFont );
	}
	void DebugFont::Render()
	{
//		m_kFontObjectList.clear();
		if(NULL == m_pD3DFont) return;
		for(FontObjectList::iterator ite = m_kFontObjectList.begin(); ite!=m_kFontObjectList.end(); ite++)
		{
			RECT R;
			SetRect( &R, 0, 0, 0, 0 );
			m_pD3DFont->DrawText( NULL, (*ite)->str.c_str(), -1, &R, DT_LEFT | DT_CALCRECT , 0xffffffff );
			OffsetRect( &R, (*ite)->iX * m_kD3DFD.Width, (*ite)->iY * m_kD3DFD.Height );
			m_pD3DFont->DrawText( NULL, (*ite)->str.c_str(), -1, &R, DT_LEFT , 0xffffffff );
		}
		m_kFontObjectList.clear();

	}

}