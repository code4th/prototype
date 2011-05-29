#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

#include "kickflipSmartPointer.h"
#include "kickflipGraphicShader.h"

namespace kickflip
{
	SmartPtr(GraphicDevice);

	class GraphicDevice : public ReferenceObject
	{
	public:
		GraphicDevice(void)
			: m_pD3D(NULL)
			, m_pD3DDev(NULL)

		{
		}
		virtual ~GraphicDevice(void)
		{
			if(m_pD3D != NULL) m_pD3D->Release();
			if(m_pD3DDev != NULL) m_pD3DDev->Release();
		}

	private:
		LPDIRECT3D9 m_pD3D;
		LPDIRECT3DDEVICE9 m_pD3DDev;
		GraphicShader m_kShader;

	public:
		LPDIRECT3DDEVICE9 GetDevice()
		{
			return m_pD3DDev;
		}
		bool Initialize(const HWND hWnd, const int width, const int height);

		GraphicShader& GetShader(){ return m_kShader; }

	};
}

