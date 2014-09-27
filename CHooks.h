#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

#ifndef CHooks_H
#define CHooks_H

class CHooks
{
	private:
		MODULEINFO m_modInfo;
		HMODULE m_hModule;

		DWORD m_dwOriginal;
		DWORD m_dwBackup;
		DWORD m_dwTargAddr;
	public:
		CHooks();

		MODULEINFO getModuleInfo(char *module);
		DWORD findSignature(char *module, char *pattern, char *mask);
		void insertJMP(BYTE *address, DWORD jumpDestination, DWORD length);
};

#endif