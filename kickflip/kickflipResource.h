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
			UNKNOWN = -1,	// 状態未定義
			LOADWAIT = 0,	// ロード待ち
			LOADING,		// 読み込み中
			LOADFINISH,		// 読み込み完了
			COMPLETE,		// 準備完了
		};
	private:
		friend class ResourceManager;
		friend class BackGroundLoader;
	protected:
		friend class ResourceManager;
		Resource(const hashString& kFileName)
			: m_kFileName(kFileName)
			, m_bIsCallFinish(true)
			, m_eState(UNKNOWN)
		{}
		virtual ~Resource(){}

		virtual bool  Load() = 0;		// ロード処理
		virtual bool  Finish(){ return true;}		// ロード完了

		void Complete() { m_eState = COMPLETE; }

	public:
		bool IsLoadFinish() { return LOADFINISH == m_eState; }
		bool IsComplete() { return COMPLETE == m_eState; }
		const State GetState() { return m_eState; }
	private:
		void  callFinish()
		{
		 	if(false == m_bIsCallFinish) return;
			if( true == Finish()) Complete();
		}


	protected:
		hashString m_kFileName;
		bool m_bIsCallFinish;
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
		void Suspend() { m_rpThread->Suspend(); }
		void Resume(){ m_rpThread->Resume(); }
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
					ResourceList::iterator ite = m_kResuestQueue.begin();
					Resource* pResource = *(ite);
					Unlock();
					// ロード処理
					if(true == pResource->Load())
					{
						pResource->m_eState = Resource::LOADFINISH;
						// ロード後処理
						pResource->callFinish();
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
			ResourceMap::iterator ite = m_kResourceMap.find(kFileName.hash);

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
					pResource->m_eState = Resource::UNKNOWN;
				}else{
					pResource->m_eState = Resource::LOADFINISH;
					pResource->callFinish();
				}
			}else{
				// 裏読み
				pushBackGroundLoadQueue(pResource);
			}

			return pResource;
		}

	};
}

