#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <iostream>

#include "CHooks.h"
#include "CProcessFactory.h"

CProcessFactory ProcessFinder;
CHooks Hooker;

void FireHooks();
__declspec(naked) void UnlimitedAmmo();

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			FireHooks();
			break;
		default:
			std::cout << "Fail" << std::endl;
	}

	return 1;
}

void FireHooks()
{
	DWORD ammoAddress = Hooker.findSignature(".exe", "pattern..", "mask");
	ammoAddress += 0; //+ bytes ahead

	DWORD ammoJMPBack = ammoAddress;// + bytes

	Hooker.insertJMP((BYTE*)ammoAddress, (DWORD)UnlimitedAmmo, NULL); //NULL IS JUST A PLACEHOLDER, WILL NEED AN ACTUAL BYTE VALUE
}

__declspec(naked) void UnlimitedAmmo()
{

}