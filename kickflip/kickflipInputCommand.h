#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <XInput.h>
#include <tchar.h>

#include <vector>
#include <map>
#include "kickflipSmartPointer.h"
#include "kickflipInputDevice.h"
#include "kickflipEmbedded.h"
#include "kickflipDebugFont.h"

namespace kickflip
{
	SmartPtr(InputCommand);
	class InputCommand :public ReferenceObject
	{
	public:
		InputCommand(void){}

		virtual ~InputCommand(void){}

	public:
		SmartPtr(Command);
		typedef std::map< unsigned int, Command > CommandMap;
		class Command : public ReferenceObject
		{
		public:
			unsigned int m_uiMixTime;
			unsigned int m_uiMaxTime;
			std::string m_kName;

			CommandMap m_kCommandMap;
		};
	private:
		CommandMap m_kCommandMap;
	public:
		static CommandRPtr Make(int iMinTime, unsigned int iKey, int iMaxTime, const char* pName)
		{
			CommandRPtr rpCommand = new Command;
			rpCommand->m_uiMixTime = iMinTime;
			rpCommand->m_uiMaxTime = iMaxTime;
			return rpCommand;
		}

	};
#if 0
	// �R�}���h�쐬
	State kPS = InputCommand::Make(1,GamePad::A,10,"���p���`");
	State kPL = InputCommand::Make(11,GamePad::A,-1,"��p���`");
	State kKS = InputCommand::Make(1,GamePad::B,10,"���L�b�N");
	State kKL = InputCommand::Make(11,GamePad::B,-1,"��L�b�N");
	State kNS = InputCommand::Make(1,GamePad::NONE,10,"���j���[�g����");
	State kNL = InputCommand::Make(11,GamePad::NONE,30,"��j���[�g����");

	// PK
	State kPK = MakeCommand(kPS+kNS+kKL,"PK");

	// PPK
	State kPPK = MakeCommand(kPS+kNS+kPS+kNS+kKL,"PPK");
	State kPPK = MakeCommand(kPS+kNS+kPK,"PPK");

#endif
}

