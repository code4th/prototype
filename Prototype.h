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
			: ThreadFunction(true,1)
			, name(name_)
			, x(x_)
			, y(y_)
		{}
		virtual ~Test()
		{
		}
		virtual unsigned int Execute(kickflip::Thread* pThread)
		{
			//			DebugTrace("%s",name.c_str());
			return 0;
		}
		std::string name;
		int x,y;
	};

	SmartPtr(MeshObject);
	class MeshObject : public kickflip::Resource
	{
	public:
		MeshObject(){}
		virtual ~MeshObject()
		{
		}

		virtual bool  Load(const char* kFileName)
		{
//			D3DXLoadMeshFromX( _T("media/wall_with_pillars.x"), D3DXMESH_MANAGED, GetGraphicDevice(), NULL, &pMatBuf, NULL, &dwMatNum, &pMesh );
			D3DXLoadMeshFromX( kFileName, D3DXMESH_MANAGED, NULL, NULL, &pMatBuf, NULL, &dwMatNum, &pMesh );
			pMatAry = (D3DXMATERIAL*)pMatBuf->GetBufferPointer();
			return true;
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
	MeshObjectRPtr m_rpMeshObject;

};

