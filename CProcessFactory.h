#include <Windows.h>
#include <TlHelp32.h>

#ifndef CPROCESSFACTORY_H
#define CPROCESSFACTORY_H

class CProcessFactory
{
private:
	HANDLE mHndlProc;
	HANDLE mToken;
	HWND mHwnd;

	TOKEN_PRIVILEGES mPrivs;
	LUID luid;

	PROCESSENTRY32 mPEntry;
	THREADENTRY32 mTEntry;
	MODULEENTRY32 mMEntry;
public:
	CProcessFactory();

	PROCESSENTRY32 mGameProc;

	DWORD getProcessID(const char *name, PROCESSENTRY32 *entry);
	DWORD getThreadfromProc(DWORD process);
	DWORD getModulePointer(LPSTR module, DWORD procID);
	void  setPrivs();
};

extern CProcessFactory ProcessFinder;

#endif