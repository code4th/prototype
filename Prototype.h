#pragma once

#include "kickflip/kickflipframework.h"
#include "kickflip/kickflipSmartPointer.h"
#include "kickflip/kickflipThread.h"
#include "kickflip/kickflipAction.h"
#include "kickflip/kickflipResource.h"
#include "kickflip/kickflipHashString.h"

#include <vector>

class Prototype :
	public kickflip::Framework
{
public:
	Prototype(void){}
	virtual ~Prototype(void){}

	virtual void ExecOnceBeforeUpdate();
	virtual void UpdateFrame();


public:
	ID3DXEffect* pEffect;

	D3DXMATRIX View, Proj;
	float f;

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
		{}
		virtual ~MeshObject()
		{
		}

		virtual bool Load()
		{

			HRESULT hRes = D3DXLoadMeshFromX( m_kFileName.str.c_str(), D3DXMESH_MANAGED, Framework::Get()->GetGraphicDevice(), NULL, &pMatBuf, NULL, &dwMatNum, &pMesh );
			Sleep(1000);
			if(D3D_OK == hRes)
			{
				pMatAry = (D3DXMATERIAL*)pMatBuf->GetBufferPointer();
				return true;
			}else{

				return false;
			}
		}

	public:
		ID3DXBuffer* pMatBuf;
		ID3DXMesh* pMesh;
		D3DXMATERIAL* pMatAry;
		DWORD dwMatNum;
	};

	std::vector<kickflip::ThreadRPtr> m_rpThreadList;

	kickflip::ThreadRPtr m_rpThread;
	kickflip::ActionControllerRPtr m_rpActionController;
	kickflip::ResourceManagerRPtr m_rpResouceManager;
	std::vector<MeshObjectRPtr> m_kMeshObjectList;

};

