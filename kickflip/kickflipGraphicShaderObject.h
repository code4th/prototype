#pragma once
/*
	�V�F�[�_�I�u�W�F�N�g
*/

#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

#include "kickflipSmartPointer.h"

namespace kickflip
{
	SmartPtr(GraphicShaderObject);

	class GraphicShaderObject : public ReferenceObject
	{
	public:
		GraphicShaderObject(void)
		{
		}
		virtual ~GraphicShaderObject(void)
		{
		}

	private:
	public:

	};
}

