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
			, pMesh(NULL)
			, ppTextureAry(NULL)
			, pMatAry(NULL)
			, dwMatNum(0)
			, pBuf(NULL)
		{
//			m_bIsCallFinish = false;
		
		}
		virtual ~MeshObject()
		{
			if(NULL != pBuf) delete pBuf;
			if(NULL != ppTextureAry) delete ppTextureAry;
			if(NULL != pMatAry) delete pMatAry;
		}
		void Draw()
		{
			if(false == IsComplete()) return;

			for(DWORD i=0; i<dwMatNum; i++)
			{
				Framework::Get().GetGraphicDevice()->GetDevice()->SetMaterial( &pMatAry[i] );
				Framework::Get().GetGraphicDevice()->GetDevice()->SetTexture( 0, ppTextureAry[i] );
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

			LPDIRECT3DDEVICE9 d3dev = Framework::Get().GetGraphicDevice()->GetDevice();

			Framework::Get().GetGraphicDevice()->Lock();
			LPD3DXBUFFER pMatBuf;  // D3DBufferオブジェクトへのポインタ

			HRESULT hRes = D3DXLoadMeshFromXInMemory(pBuf,iBufSize,D3DXMESH_MANAGED, d3dev, NULL, &pMatBuf, NULL, &dwMatNum, &pMesh);

			// D3DXMATERIAL構造体の配列へのポインタを取得
			D3DXMATERIAL* d3dxmatrs = 
				(D3DXMATERIAL*)pMatBuf->GetBufferPointer(); 

			// LPDIRECT3DTEXTURE9型の配列を確保
			ppTextureAry = 
				(LPDIRECT3DTEXTURE9*)std::malloc(sizeof(LPDIRECT3DTEXTURE9) * dwMatNum);
			// D3DMATERIAL9構造体の配列を確保
			pMatAry = (D3DMATERIAL9*)std::malloc(sizeof(D3DMATERIAL9) * dwMatNum);
			for (int i = 0; i < (int)dwMatNum; i++)
			{
				pMatAry[i] = d3dxmatrs[i].MatD3D; // 材質の情報をコピー

				// D3DXCreateTextureFromFile関数が失敗した場合
				std::string fileNameTexture("media/");
				fileNameTexture+=d3dxmatrs[i].pTextureFilename;
				if (D3DXCreateTextureFromFile(
					d3dev, 
					fileNameTexture.c_str(), 
					&ppTextureAry[i] 
				) != S_OK ) // テクスチャ画像のファイル名からテクスチャオブジェクトを作成
					ppTextureAry[i] = NULL; // D3DXCreateTextureFromFile関数が失敗したらテクスチャは無効

			}
			Framework::Get().GetGraphicDevice()->Unlock();
			free(pBuf);pBuf=NULL;

			pMatBuf->Release(); // D3DXBUFFERオブジェクトを解放
			Complete();
			return true;
		}

	public:
		ID3DXMesh* pMesh;
		D3DMATERIAL9* pMatAry;
		LPDIRECT3DTEXTURE9 *ppTextureAry;
		DWORD dwMatNum;
		void* pBuf;
		size_t iBufSize;
	};

	std::vector<kickflip::ThreadRPtr> m_rpThreadList;

	kickflip::ThreadRPtr m_rpThread;
	kickflip::ActionControllerRPtr m_rpActionController;
	kickflip::ResourceManagerRPtr m_rpResouceManager;
	std::vector<MeshObjectRPtr> m_kMeshObjectList;

	LPDIRECT3DSURFACE9       backbuffer;

	LPDIRECT3DTEXTURE9       texColor;
	LPDIRECT3DSURFACE9       surColor;
	LPDIRECT3DTEXTURE9       texNormalDepth;
	LPDIRECT3DSURFACE9       surNormalDepth;

	LPDIRECT3DTEXTURE9       rayMap;

	int m_iFlag;

};

