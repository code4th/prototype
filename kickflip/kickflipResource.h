#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <XInput.h>
#include <tchar.h>

#include <vector>
#include <map>
#include "kickflipSmartPointer.h"
#include "kickflipThread.h"
#include "kickflipEmbedded.h"
#include "kickflipHashString.h"
#include "kickflipDebugFont.h"

/*
	リソースマネージ
	データはすべてResourceを継承しておけ
*/

namespace kickflip
{
	// リソースはこいつを継承する
	SmartPtr(Resource);
	class Resource :public ReferenceObject
	{
	public:
		enum State{
			UNKNOWN = -1,
			INIT = 0,
			LOADING,
			COMPLETE,
		};
	private:
		friend class ResourceManager;
	public:
		Resource()
			: m_eState(UNKNOWN)
		{}
		virtual ~Resource(){}

		virtual bool  Load(const char* pFileName){return true;}

		bool IsComplete() { return COMPLETE == m_eState; }
		const State GetState() { return m_eState; }

	private:
		State m_eState;
	};


	// Resourceを管理する
	SmartPtr(ResourceManager);
	class ResourceManager :public ReferenceObject
	{
	public:
		ResourceManager(void)
		{}

		virtual ~ResourceManager(void){}

	private:
		SmartPtr(BackGroundLoader);
		class BackGroundLoader :public ThreadFunction
		{
		public:
			BackGroundLoader()
				: ThreadFunction(true,0)
			{}
			virtual ~BackGroundLoader()
			{}
			virtual unsigned int Execute(kickflip::Thread* pThread)
			{
				Sleep(0);

				return 0;
			}
			Lock m_kLock;
		private:
		};

	private:
		ThreadRPtr m_rpThread;
		BackGroundLoader* m_pInputStabilizer;

		typedef std::map<hash32, ResourceRPtr>	ResourceMap;

		ResourceMap		m_kResourceMap;

		Lock m_kLock;

	public:
		template<class T>
		RefPtr<T> Load(const hashString kFileName)
		{
			m_kLock.Enter();
			auto ite = m_kResourceMap.find(kFileName.hash);
			m_kLock.Exit();

			if(m_kResourceMap.end() != ite)
			{
				// 持ってれば返す
				return ite->second;
			}

			// 読み込む
//			RefPtr<T> rpResource = T::load(*this, kFileName);
			RefPtr<T> rpResource = new T;
			if(NULL == rpResource)
			{
				// 失敗したらNULLかえす
				return NULL;
			}
			if(true == rpResource->Load(kFileName.str))
			{
				m_kLock.Enter();
				m_kResourceMap[kFileName.hash] = rpResource;
				m_kLock.Exit();
			}

			return rpResource;
		}

	};
}

