#pragma once

#include "kickflip/kickflipframework.h"
#include "kickflip/kickflipSmartPointer.h"
#include "kickflip/kickflipThread.h"
#include "kickflip/kickflipAction.h"

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
	ID3DXBuffer* pMatBuf;
	ID3DXMesh* pMesh;
	ID3DXEffect* pEffect;

	D3DXMATRIX View, Proj;
	D3DXMATERIAL* pMatAry;
	DWORD dwMatNum;
	float f;

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
	typedef kickflip::RefPtr<Test> TestRPtr;
	typedef kickflip::WeakPtr<Test> TestWPtr;

	std::vector<kickflip::ThreadRPtr> m_rpThreadList;

	kickflip::ThreadRPtr m_rpThread;
	kickflip::ActionControllerRPtr m_rpActionController;

};

