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
			LOADWAIT = 0,
			LOADING,
			COMPLETE,
		};
	private:
		friend class ResourceManager;
		friend class BackGroundLoader;
	protected:
		friend class ResourceManager;
		Resource(const hashString& kFileName)
			: m_kFileName(kFileName)
			, m_eState(UNKNOWN)
		{}
		virtual ~Resource(){}

		virtual bool  Load() = 0;
		virtual void  CompleteLoad(){}

	public:
		bool IsComplete() { return COMPLETE == m_eState; }
		const State GetState() { return m_eState; }

	protected:
		hashString m_kFileName;
	private:
		State m_eState;
	};


	// Resourceを管理する
	SmartPtr(ResourceManager);
	class ResourceManager :public ReferenceObject
	{
	public:
		ResourceManager(void)
			: m_rpThread(NULL)
		{
			m_kResourceMap.clear();
		}

		virtual ~ResourceManager(void){}

	private:
		SmartPtr(BackGroundLoader);
		class BackGroundLoader :public ThreadFunction
		{
		public:
			BackGroundLoader(ResourceManager* pResourceManager)
				: m_pResourceManager(pResourceManager)
			{
				m_kResuestQueue.clear();
			}
			virtual ~BackGroundLoader()
			{}
			virtual unsigned int Execute(kickflip::Thread* pThread)
			{
				Lock();
				if(false == m_kResuestQueue.empty())
				{
					auto ite = m_kResuestQueue.begin();
					Resource* pResource = *(ite);
					Unlock();
					// ロード処理
					if(true == pResource->Load())
					{
						pResource->m_eState = Resource::COMPLETE;
						// ロード後処理
						pResource->CompleteLoad();
					}else{
						pResource->m_eState = Resource::UNKNOWN;
					}
					Lock();
					m_kResuestQueue.erase( m_kResuestQueue.begin());
					Unlock();
				}else{
//					pThread->Suspend();
					StopContinue();
					Unlock();
				}
				return 0;
			}
		public:
			void AddQueue(Resource* pResource)
			{
				Lock();
				m_kResuestQueue.push_back(pResource);
				Unlock();
			}
		private:
			ResourceManager* m_pResourceManager;
			typedef std::vector<ResourceRPtr>	ResourceList;
			ResourceList	m_kResuestQueue;
		};
		bool pushBackGroundLoadQueue(Resource* pResource)
		{
			if(false == WakeupBackGroundLoader())
			{
				pResource->m_eState = Resource::UNKNOWN;
				return false;
			}
			pResource->m_eState = Resource::LOADWAIT;
			BackGroundLoaderRPtr rpBackGroundLoader = m_rpThread->GetThreadFunction();
			rpBackGroundLoader->AddQueue(pResource);

			return true;
		}
	private:
		ThreadRPtr m_rpThread;

		typedef std::map<hash32, ResourceRPtr>	ResourceMap;

		ResourceMap		m_kResourceMap;
		

//		Lock m_kLock;

	public:
		// 裏読みシステムの起動
		bool WakeupBackGroundLoader()
		{
			if(NULL == m_rpThread)
			{
				m_rpThread = Thread::Create( new BackGroundLoader(this), true, 0 );
				if ( NULL == m_rpThread ) return false;
				m_rpThread->Resume();
			}else{
				m_rpThread->SignalStart();
			}


			return true;
		}

		template<class T>
		RefPtr<T> LoadBackGround(const hashString& kFileName)
		{
			return Load<T>(kFileName, true);
		}

		template<class T>
		RefPtr<T> Load(const hashString& kFileName, bool bIsBackLoad = false)
		{
			auto ite = m_kResourceMap.find(kFileName.hash);

			if(m_kResourceMap.end() != ite)
			{
				// 持ってれば返す
				ResourceRPtr rpResource = ite->second;
				return rpResource;
			}

			// 読み込む
			T* pResource = new T(kFileName);
			if(NULL == pResource)
			{
				// 失敗したらNULLかえす
				return NULL;
			}

			// ひとまず追加
			m_kResourceMap[kFileName.hash] = pResource;

			if(false == bIsBackLoad)
			{
				// 即時読み込み
				if(false == pResource->Load())
				{
					return NULL;
				}else{
					pResource->m_eState = Resource::COMPLETE;
					pResource->CompleteLoad();
				}
			}else{
				// 裏読み
				pushBackGroundLoadQueue(pResource);
			}

			return pResource;
		}

	};
}

