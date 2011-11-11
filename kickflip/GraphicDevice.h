#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

#include "SmartPointer.h"
#include "GraphicShader.h"
#include "Thread.h"

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
		GraphicShaderRPtr m_rpShader;
		Lock m_kLock;

	public:
		bool Initialize(const HWND hWnd, const int width, const int height);

		GraphicShaderRPtr& GetShader(){ return m_rpShader; }
		LPDIRECT3DDEVICE9 GetDevice() { return m_pD3DDev; }

		void Lock() { m_kLock.Enter(); }
		void Unlock() { m_kLock.Exit(); }

	};
}

