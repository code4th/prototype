
#ifndef __NC_PLT_EX_STACKTRACE_h__
#define __NC_PLT_EX_STACKTRACE_h__

#include <windows.h>

//
// �X�^�b�N�g���[�X�̏o�͂�񋟂��܂��B
//
class StackTrace
{

private:
	bool LoadModules(HANDLE hProcess, DWORD dwProcessId);

public:
	StackTrace(void);
	~StackTrace(void);
	void Show();

};

#endif // __NC_PLT_EX_STACKTRACE_h__