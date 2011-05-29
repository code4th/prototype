/*!
	smartPointer.h
	スマートポインタ
*/

// ----------------------------------------------------------------------------
// Include
// ----------------------------------------------------------------------------
#include "kickflipSmartPointer.h"
namespace kickflip
{
	RefReferenceObject::~RefReferenceObject()
	{
		if( NULL != m_pRealObject )
			m_pRealObject->ClearRefMe();
	}
	ReferenceObject::~ReferenceObject()
	{
		if(NULL != m_pRefMe)
		{
			m_pRefMe->m_pRealObject = NULL;
		}
	}
}
