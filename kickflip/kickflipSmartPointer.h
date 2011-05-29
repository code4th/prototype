/*!
smartPointer.h
スマートポインタ
*/

// ----------------------------------------------------------------------------
#pragma once
// ----------------------------------------------------------------------------
// Include
// ----------------------------------------------------------------------------
#include <assert.h>
namespace kickflip
{
	// ----------------------------------------------------------------------------
	// Define
	// ----------------------------------------------------------------------------
#define SmartPtr(classname) \
	class classname; \
	typedef RefPtr<classname> classname##RPtr; \
	typedef WeakPtr<classname> classname##WPtr

	//!	NULLマクロ定義
#ifndef NULL
#ifdef __cplusplus
#define NULL	0
#else	//__cplusplus
#define NULL	((void *)0)
#endif	//__cplusplus
#endif	//NULL
	// ----------------------------------------------------------------------------
	// Class
	// ----------------------------------------------------------------------------

	// ----------------------------------------------------------------------------
	/*
	リファレンスポインタ
	RefPtr<test> rpTest = new test;
	rpTest->x = xxx;
	{
	RefPtr<test> rpTest2 = rpTest;
	// rpTest消えない
	}
	rpTest = NULL;	// 参照なくなって消える
	*/
	template <typename T> 
	class RefPtr
	{
	protected:
		template<typename O> friend class RefPtr;
		template<typename T> friend class WeakPtr;
	protected:
		T* m_pObject;

	public:
		inline RefPtr(T* pObject = (T*) 0)
		{
			m_pObject = pObject;
			if (m_pObject)
				m_pObject->AddRef();
		}
		inline RefPtr(const RefPtr<T>& ptr)
		{
			m_pObject = ptr.m_pObject;
			if (m_pObject)
				m_pObject->AddRef();
		}
		template<class SRC>
		inline RefPtr(const RefPtr<SRC>& ptr)
		{
			m_pObject = ptr.m_pObject;
			if (m_pObject)
				m_pObject->AddRef();
		}

		inline RefPtr(const WeakPtr<T>& ptr)
		{
			if(NULL == ptr.m_pRefObject){
				m_pObject = NULL;
			}else{
				m_pObject = (T*)ptr.m_pRefObject->m_pRealObject;
			}
			if (m_pObject)
				m_pObject->AddRef();
		}

		inline virtual ~RefPtr()
		{
			if (m_pObject)
				m_pObject->DelRef();
		}

		inline operator T*() const 
		{
			return m_pObject;
		}
		inline T* operator->() const
		{
			return m_pObject;
		}
		inline T& operator*() const
		{
			return *m_pObject;
		}
		inline RefPtr<T>& operator=(const RefPtr<T>& ptr)
		{
			if (m_pObject != ptr.m_pObject)
			{
				if (m_pObject)
					m_pObject->DelRef();
				m_pObject = ptr.m_pObject;
				if (m_pObject)
					m_pObject->AddRef();
			}
			return *this;
		}
		// comparisons

		inline bool operator==(T* pObject) const
		{
			return (m_pObject == pObject);
		}

		inline bool operator!=(T* pObject) const
		{
			return (m_pObject != pObject);
		}

		inline bool operator==(const RefPtr& ptr) const
		{
			return (m_pObject == ptr.m_pObject);
		}
		inline bool operator!=(const RefPtr& ptr) const
		{
			return (m_pObject != ptr.m_pObject);
		}
	};
	// ----------------------------------------------------------------------------
	/*
	ウィークポインタ
	WaekPtr<test> wpTest = rpTest;	// rpTest:リファレンスポインタ
	if(NULL != wpTest) wpTest->x = xxx;
	rpTest = NULL;	// 参照なくなって消えた場合は以下がNULLになって通らない
	if(NULL != wpTest) wpTest->x = xxx;
	*/
	struct ReferenceObject;
	struct RefReferenceObject;

	template <typename T> 
	class WeakPtr
	{
	protected:
		template<typename O> friend class WeakPtr;
		template<typename T> friend class RefPtr;
	protected:
		RefReferenceObject* m_pRefObject;
	public:
		inline WeakPtr(T* pObject = (T*) 0)
		{
			if( NULL == pObject )
			{
				m_pRefObject = NULL;
			}
			else
			{
				m_pRefObject = pObject->GetRefMe();
				m_pRefObject->AddRef();
			}

		}

		inline WeakPtr(const WeakPtr<T>& ptr)
		{
			m_pRefObject = ptr.m_pRefObject;
			if(m_pRefObject)
				m_pRefObject->AddRef();
		}
		template<class SRC>
		inline WeakPtr(const WeakPtr<SRC>& ptr)
		{
			m_pRefObject = ptr.m_pRefObject;
			if(m_pRefObject)
				m_pRefObject->AddRef();
		}
		inline WeakPtr(const RefPtr<T>& ptr)
		{
			if( NULL == ptr.m_pObject )
			{
				m_pRefObject = NULL;
			}
			else
			{
				m_pRefObject = ptr.m_pObject->GetRefMe();
				m_pRefObject->AddRef();
			}


		}

		inline virtual ~WeakPtr()
		{
			if (m_pRefObject)
				m_pRefObject->DelRef();
		}

		inline operator T*() const 
		{
			return (NULL == m_pRefObject) ? NULL : (T*)(m_pRefObject->m_pRealObject);
		}
		inline T* operator->() const
		{
			assert(NULL!=m_pRefObject);
			return (T*)(m_pRefObject->m_pRealObject);
		}
		inline T& operator*() const
		{
			assert(NULL!=m_pRefObject);
			return *((T*)m_pRefObject->m_pRealObject);
		}
		inline WeakPtr<T>& operator=(const WeakPtr<T>& ptr)
		{
			// 既にいるWeakPtrがもっているm_pRefObjectがReferenceObjectRefを持っていないことはない
			if ( m_pRefObject != ptr.m_pRefObject )
			{
				if(m_pRefObject)
					m_pRefObject->DelRef();
				m_pRefObject = ptr.m_pRefObject;
				if(m_pRefObject)
					m_pRefObject->AddRef();
			}
			return *this;
		}
		// comparisons
		inline bool operator==(T* pObject) const
		{
			if(NULL == pObject)
			{
				if(NULL == m_pRefObject || NULL == m_pRefObject->m_pRealObject) return true;
				return false;
			}

			if(NULL == m_pRefObject || NULL == m_pRefObject->m_pRealObject) return false;
			return( m_pRefObject->m_pRealObject == pObject);

		}
		inline bool operator!=(T* pObject) const
		{
			if(NULL == pObject)
			{
				if(NULL == m_pRefObject || NULL == m_pRefObject->m_pRealObject) return false;
				return true;
			}

			if(NULL == m_pRefObject || NULL == m_pRefObject->m_pRealObject) return true;
			return( m_pRefObject->m_pRealObject != pObject);
		}
		inline bool operator==(const WeakPtr& ptr) const
		{
			if(NULL == m_pRefObject) return false;
			if(NULL == ptr.m_pRefObject) return false;
			return( m_pRefObject->m_pRealObject == ptr.m_pRefObject->m_pRealObject);
		}
		inline bool operator!=(const WeakPtr& ptr) const
		{
			if(NULL == m_pRefObject) return true;
			if(NULL == ptr.m_pRefObject) return true;
			return( m_pRefObject->m_pRealObject != ptr.m_pRefObject->m_pRealObject);
		}

	};

	// ----------------------------------------------------------------------------
	/*
	ウィークポインタ用参照オブジェクト参照オブジェクト
	*/
	struct RefReferenceObject
	{
	private:
		friend struct ReferenceObject;
		template<typename A> friend class WeakPtr;
		template<typename B> friend class RefPtr;
		ReferenceObject* m_pRealObject;
		//	mutable unsigned int m_uiWeakCount;
		mutable volatile int m_uiWeakCount;
	public:
		inline RefReferenceObject(ReferenceObject* pObject = NULL)
			: m_pRealObject(pObject)
			, m_uiWeakCount(0)
		{
		}

		virtual ~RefReferenceObject();

	private:
		inline void AddRef() const
		{
			//		InterlockedExchangeAdd(&m_uiWeakCount,1);
			m_uiWeakCount++;
		}
		inline void DelRef() const
		{
			//		InterlockedExchangeAdd(&m_uiWeakCount,-1);
			m_uiWeakCount--;
			if(m_uiWeakCount==0) delete this;
		}

	};
	// ----------------------------------------------------------------------------
	/*
	スマートポインタコンテナ
	実際のオブジェクトを保持しているオブジェクト
	*/
	struct ReferenceObject
	{
	private:
		friend struct RefReferenceObject;
		template<typename A> friend class WeakPtr;
		template<typename B> friend class RefPtr;
	private:
		mutable volatile int m_uiRefCount;
		//	mutable unsigned int m_uiRefCount;				// 権利を持っている人の数
		RefReferenceObject* m_pRefMe;			// 自分を参照するオブジェクト
	public:
		ReferenceObject()
			: m_uiRefCount(0)
			, m_pRefMe(NULL)
		{
		}

		virtual ~ReferenceObject();
		inline unsigned int GetRefCount() const { return m_uiRefCount; }

	private:
		inline void AddRef() const 
		{
			//		InterlockedExchangeAdd(&m_uiRefCount,1);
			m_uiRefCount++;
		}
		inline void DelRef() const
		{
			//		InterlockedExchangeAdd(&m_uiRefCount,-1);
			m_uiRefCount--;
			if(m_uiRefCount==0) delete this;
		}

		inline RefReferenceObject* GetRefMe()
		{
			// コンテナにまだ参照オブジェクトがなければ作る
			if(NULL == m_pRefMe)
			{
				m_pRefMe = new RefReferenceObject(this);
			}
			return m_pRefMe;
		}
		inline void ClearRefMe()
		{
			m_pRefMe = NULL;
		}

	};
}
