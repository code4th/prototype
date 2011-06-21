#pragma once

#include "kickflip/kickflipframework.h"
#include "kickflip/kickflipSmartPointer.h"
#include "kickflip/kickflipThread.h"
#include "kickflip/kickflipAction.h"
#include "kickflip/kickflipResource.h"
#include "kickflip/kickflipHashString.h"
#include <cstdio>
#include <vector>

class Prototype :
	public kickflip::Framework
{
public:
	Prototype(void){}
	virtual ~Prototype(void){}

	virtual void InitSetting();

	virtual void ExecOnceBeforeUpdate();
	virtual void UpdateFrame();
	virtual void BeforePresent();
	virtual void AfterPresent();


public:
	ID3DXEffect* pEffect;

	D3DXMATRIX View, Proj;
	float f,l;

	SmartPtr(Test);
	class Test : public kickflip::ThreadFunction
	{
	public:
		Test(char* name_, int x_, int y_)
			: name(name_)
			, x(x_)
			, y(y_)
		{}
		virtual ~Test()
		{
		}
		virtual unsigned int Execute(kickflip::Thread* pThread)
		{
			DebugTrace("%s",name.c_str());
			return 0;
		}
		std::string name;
		int x,y;
	};

	SmartPtr(MeshObject);
	class MeshObject : public kickflip::Resource
	{
	public:
		MeshObject(const kickflip::hashString& kFileName)
			: Resource(kFileName)
			, pMatBuf(NULL)
			, pMesh(NULL)
			, pMatAry(NULL)
			, dwMatNum(0)
			, pBuf(NULL)
		{
//			m_bIsCallFinish = false;
		
		}
		virtual ~MeshObject()
		{
			if(NULL != pBuf) delete pBuf;
		}
		void Draw()
		{
			if(false == IsComplete()) return;

			for(DWORD i=0; i<dwMatNum; i++)
			{
				Framework::Get().GetGraphicDevice()->GetDevice()->SetMaterial( &(pMatAry[i].MatD3D) );
				pMesh->DrawSubset(i);
			}
		}
		virtual bool Load()
		{
			Sleep(200);
			std::FILE* fp = NULL;
			errno_t error = fopen_s(&fp, m_kFileName.str.c_str(),"rb");
			if( 0 == error )
			{
				fseek( fp, 0L, SEEK_END );
				iBufSize = ftell( fp );
				fseek( fp, 0L, SEEK_SET );
				pBuf = std::malloc(iBufSize);
				size_t readNum = std::fread(pBuf,iBufSize,1,fp);
				std::fclose(fp);

				return true;

			}

			return false;

		}
		virtual bool Finish()
		{
			if(NULL == pBuf) return false;
			Framework::Get().GetGraphicDevice()->Lock();
			HRESULT hRes = D3DXLoadMeshFromXInMemory(pBuf,iBufSize,D3DXMESH_MANAGED, Framework::Get().GetGraphicDevice()->GetDevice(), NULL, &pMatBuf, NULL, &dwMatNum, &pMesh);
			Framework::Get().GetGraphicDevice()->Unlock();
			free(pBuf);pBuf=NULL;
			if(D3D_OK != hRes)
			{
					return false;
			}

			pMatAry = (D3DXMATERIAL*)pMatBuf->GetBufferPointer();
			Complete();
			return true;
		}

	public:
		ID3DXBuffer* pMatBuf;
		ID3DXMesh* pMesh;
		D3DXMATERIAL* pMatAry;
		DWORD dwMatNum;
		void* pBuf;
		size_t iBufSize;
	};

	std::vector<kickflip::ThreadRPtr> m_rpThreadList;

	kickflip::ThreadRPtr m_rpThread;
	kickflip::ActionControllerRPtr m_rpActionController;
	kickflip::ResourceManagerRPtr m_rpResouceManager;
	std::vector<MeshObjectRPtr> m_kMeshObjectList;

	LPDIRECT3DTEXTURE9       texture0;
	LPDIRECT3DSURFACE9       surface0;
	LPDIRECT3DSURFACE9       backbuffer;
	LPDIRECT3DTEXTURE9       rayMap;


};

