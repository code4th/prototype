#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <XInput.h>
#include <tchar.h>

#include "SmartPointer.h"
#include "InputDevice.h"
#include "Embedded.h"
#include "HashString.h"
#include "Debug/DebugFont.h"

#include <vector>
#include <map>
#include <iterator>

namespace kickflip
{

	SmartPtr(Action);
	class Action : public ReferenceObject
	{
	public:
		Action(hashString kHash)
			: m_fEnterTime(0.f)
			, m_kHash(kHash)
		{}
		virtual ~Action() {}
		virtual const hashString GetHash() { return m_kHash;};

		virtual void Enter()
		{
			m_fEnterTime = (float)Time::GetRealTimeSecond();
		}
		virtual bool Update()
		{
			return true;
		}
		virtual void Exit()
		{
		}
	protected:
		float m_fEnterTime;
		hashString m_kHash;

	};

	SmartPtr(ActionController);
	class ActionController :public ReferenceObject
	{
	public:
		typedef std::map<hash32, ActionRPtr> ActionMap;
		typedef std::vector<ActionRPtr> ActionList;
	public:
		ActionController(void)
			: m_rpDefaultAction(NULL)
			, m_rpCurrentAction(NULL)
		{
			m_kActionMap.clear();
			m_kActionQueue.clear();
		}
		virtual ~ActionController(void){}

		void Update()
		{
			if(NULL != m_rpCurrentAction)
			{
				bool bIsContinue = m_rpCurrentAction->Update();
				if(false == bIsContinue)
				{
					ChangeAction(GetNextAction());
				}
			}

		}
/*
		void Regist(int index, const ActionRPtr& rpAction)
		{
			m_kActionMap[index] = rpAction;
		}
*/
		void Regist(const ActionRPtr& rpAction)
		{
			m_kActionMap[rpAction->GetHash().hash] = rpAction;
		}
		virtual const ActionRPtr& GetDefaultAction()
		{
			return m_rpDefaultAction;
		}
		virtual void SetDefaultAction(const ActionRPtr& rpAction)
		{
			m_rpDefaultAction = rpAction;
		}

		bool ChangeAction(hashString index)
		{
			ActionMap::iterator ite = m_kActionMap.find(index.hash);
			if(m_kActionMap.end() == ite) return false;
			return ChangeAction(ite->second);
		}
		bool ChangeAction(const ActionRPtr& rpAction)
		{
			// 現在のを終わらせる
			if(NULL!= m_rpCurrentAction)
				m_rpCurrentAction->Exit();

			// カレントを差し替える
			m_rpCurrentAction = rpAction;

			// 現在のを終わらせる
			if(NULL!= m_rpCurrentAction)
				m_rpCurrentAction->Enter();

			return true;
		}
		const ActionRPtr& GetNextAction()
		{
			ActionList::iterator ite = m_kActionQueue.begin();
			if(m_kActionQueue.end() != ite)
			{
				// queueにあるものを返す
				const ActionRPtr& rpAction = *ite;
				m_kActionQueue.erase(ite);
				return rpAction;
			}else{
				return GetDefaultAction();
			}
		}

	private:
		ActionRPtr m_rpDefaultAction;
		ActionRPtr m_rpCurrentAction;
		ActionMap m_kActionMap;
		ActionList m_kActionQueue;


	};
}

