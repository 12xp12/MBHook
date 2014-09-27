#include "CHooks.h"

CHooks::CHooks(){}

MODULEINFO CHooks::getModuleInfo(char *module)
{
	m_hModule = GetModuleHandle(module);
	GetModuleInformation(GetCurrentProcess(), m_hModule, &m_modInfo, sizeof(MODULEINFO));
	
	return m_modInfo;
}

DWORD CHooks::findSignature(char *module, char *pattern, char *mask)
{
	m_modInfo = getModuleInfo(module);

	DWORD base = (DWORD)m_modInfo.lpBaseOfDll;
	DWORD size = (DWORD)m_modInfo.SizeOfImage;

	DWORD patternSize = (DWORD)strlen(mask);

	for (DWORD i = 0; i < size - patternSize; i++)
	{
		bool isFound = true;
		for (DWORD j = 0; j < patternSize; j++)
		{
			isFound &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
		}

		if (isFound)
		{
			return base + i;
		}
	}
	
	return NULL;
}

void CHooks::insertJMP(BYTE *address, DWORD jumpDestination, DWORD length)
{
	VirtualProtect(address, length, PAGE_EXECUTE_READWRITE, &m_dwOriginal);

	//will need to revise this next line
	m_dwTargAddr = (DWORD)(jumpDestination - (DWORD)address) - length;

	//jmp opcode
	*address = 0xE9;

	*((DWORD*)(address + 0x1)) = m_dwTargAddr;

	for (DWORD x = 0x5; x < length; x++)
	{
		*(address + x) = 0x90;
	}

	VirtualProtect(address, length, m_dwOriginal, &m_dwBackup);
}

