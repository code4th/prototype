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
	���\�[�X�}�l�[�W
	�f�[�^�͂��ׂ�Resource���p�����Ă���
*/

namespace kickflip
{
	// ���\�[�X�͂������p������
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


	// Resource���Ǘ�����
	SmartPtr(ResourceManager);
	class ResourceManager :public ReferenceObject
	{
	public:
		ResourceManager(void)
			: m_rpThread(NULL)
		{
			m_kResourceMap.clear();
			m_kResourceList.clear();
		}

		virtual ~ResourceManager(void){}

	private:
		SmartPtr(BackGroundLoader);
		class BackGroundLoader :public ThreadFunction
		{
		public:
			BackGroundLoader(ResourceManager* pResourceManager)
				: ThreadFunction(true,0)
				, m_pResourceManager(pResourceManager)
			{}
			virtual ~BackGroundLoader()
			{}
			virtual unsigned int Execute(kickflip::Thread* pThread)
			{
				Lock();
				ResourceList& kResourceList = m_pResourceManager->m_kResourceList;
				if(false == kResourceList.empty())
				{
					auto ite = kResourceList.begin();
					Unlock();
					// ���[�h����
					if(true == (*ite)->Load())
					{
						(*ite)->m_eState = Resource::COMPLETE;
					}else{
						(*ite)->m_eState = Resource::UNKNOWN;
					}
					// ���[�h�㏈��
					(*ite)->CompleteLoad();
					Lock();
					kResourceList.erase(ite);
					Unlock();
				}else{
					Unlock();
				}

				Sleep(1);
				return 0;
			}
		private:
			ResourceManager* m_pResourceManager;
		};
		bool pushBackGroundLoadQueue(Resource* pResource)
		{
			WakeupBackGroundLoader();
			pResource->m_eState = Resource::LOADWAIT;
			if(NULL == m_rpThread)
			{
				return false;
			}
			m_rpThread->Lock();
			m_kResourceList.push_back(pResource);
			m_rpThread->Unlock();

			return true;
		}
	private:
		ThreadRPtr m_rpThread;

		typedef std::map<hash32, ResourceRPtr>	ResourceMap;
		typedef std::vector<Resource*>	ResourceList;

		ResourceMap		m_kResourceMap;
		ResourceList	m_kResourceList;
		

		Lock m_kLock;

	public:
		// ���ǂ݃V�X�e���̋N��
		bool WakeupBackGroundLoader()
		{
			if(NULL == m_rpThread)
			{
				m_rpThread = Thread::Create( new BackGroundLoader(this) );
				if ( NULL == m_rpThread ) return false;

				m_rpThread->Resume();
			}
			m_rpThread->Resume();

			return true;
		}
		// ���ǂ݃V�X�e���̈ꎞ��~
		void SleepBackGroundLoader()
		{
			if(NULL == m_rpThread) return;
			m_rpThread->Resume();
		}

		template<class T>
		RefPtr<T> LoadBackGround(const hashString& kFileName)
		{
			return Load<T>(kFileName, true);
		}

		template<class T>
		RefPtr<T> Load(const hashString& kFileName, bool bIsBackLoad = false)
		{
			m_kLock.Enter();
			auto ite = m_kResourceMap.find(kFileName.hash);

			if(m_kResourceMap.end() != ite)
			{
				// �����Ă�ΕԂ�
				m_kLock.Exit();
				return ite->second;
			}

			// �ǂݍ���
			T* pResource = new T(kFileName);
			if(NULL == pResource)
			{
				// ���s������NULL������
				m_kLock.Exit();
				return NULL;
			}

			// �ЂƂ܂��ǉ�
			m_kResourceMap[kFileName.hash] = pResource;

			if(false == bIsBackLoad)
			{
				// �����ǂݍ���
				if(false == pResource->Load())
				{
					delete pResource;
					m_kLock.Exit();
					return NULL;
				}else{
					pResource->m_eState = Resource::COMPLETE;
					pResource->CompleteLoad();
				}
			}else{
				// ���ǂ�
				pushBackGroundLoadQueue(pResource);
			}

			m_kLock.Exit();
			return pResource;
		}

	};
}

