#include "CProcessFactory.h"

CProcessFactory::CProcessFactory()
{
	mPEntry.dwSize = sizeof(PROCESSENTRY32);
	mTEntry.dwSize = sizeof(THREADENTRY32);
	mMEntry.dwSize = sizeof(MODULEENTRY32);
}

DWORD CProcessFactory::getProcessID(const char *name, PROCESSENTRY32 *entry)
{
	HANDLE snapProc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	Process32First(snapProc, &mPEntry);

	while (Process32Next(snapProc, &mPEntry))
	{
		if (!_stricmp(mPEntry.szExeFile, name))
		{
			memcpy((void*)entry, (void*)&mPEntry, sizeof(PROCESSENTRY32));
			CloseHandle(snapProc);
			return mPEntry.th32ProcessID;
		}
	}
	CloseHandle(snapProc);
	return 0;
}

DWORD CProcessFactory::getThreadfromProc(DWORD process)
{
	HANDLE snapProc = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	Thread32First(snapProc, &mTEntry);

	while (Thread32Next(snapProc, &mTEntry))
	{
		if (mTEntry.th32OwnerProcessID == process)
		{
			CloseHandle(snapProc);
			return mTEntry.th32ThreadID;
		}
	}
}

DWORD CProcessFactory::getModulePointer(LPSTR module, DWORD procID)
{
	HANDLE snapProc = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procID);
	bool isModuleRunning = Module32First(snapProc, &mMEntry);

	while (isModuleRunning)
	{
		if (!_stricmp(mMEntry.szModule, module))
		{
			CloseHandle(snapProc);
			return (DWORD)mMEntry.modBaseAddr;
		}
		isModuleRunning = Module32Next(snapProc, &mMEntry);
	}
	CloseHandle(snapProc);
	return 0x0;
}

void CProcessFactory::setPrivs()
{
	mHndlProc = GetCurrentProcess();

	OpenProcessToken(mHndlProc, TOKEN_ADJUST_PRIVILEGES, &mToken);
	LookupPrivilegeValue(0, "seDebugPrivilege", &luid);

	mPrivs.PrivilegeCount = 1;
	mPrivs.Privileges[0].Luid = luid;
	mPrivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(mToken, false, &mPrivs, 0, 0, 0);

	CloseHandle(mToken);
	CloseHandle(mHndlProc);
}
