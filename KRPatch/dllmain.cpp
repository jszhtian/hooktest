// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include "detours.h"

PVOID g_pOldCreateFontIndirectW = NULL;
const char cmptg1[] = "Windows title2 Set";
typedef int (WINAPI *PfuncCreateFontIndirectW)(LOGFONT *lplf);
int WINAPI NewCreateFontIndirectW(LOGFONT *lplf)
{

	//strcpy(lplf->lfFaceName, "����");
	wcscpy(lplf->lfFaceName, L"��Բ");
	MessageBoxW(0, L"API catch!", L"INFO", 0);
	//lplf->lfCharSet = GB2312_CHARSET;
	//lplf->lfCharSet = ANSI_CHARSET;
	//lplf->lfCharSet = SHIFTJIS_CHARSET;
	return ((PfuncCreateFontIndirectW)g_pOldCreateFontIndirectW)(lplf);
}

PVOID g_pOldSetWindowTextA = NULL;
typedef int (WINAPI *PfuncSetWindowTextA)(HWND hwnd, LPCTSTR lpString);
int WINAPI NewSetWindowTextA(HWND hwnd, LPCTSTR lpString)
{
	MessageBoxW(NULL, L"API Attach!", L"Info", 0);
	if (memcmp(lpString, cmptg1, strlen(cmptg1)))
	{
		strcpy((char*)(LPCTSTR)lpString, "����������");
	}
	return ((PfuncSetWindowTextA)g_pOldSetWindowTextA)(hwnd, lpString);
}



//��װHook 
void APIENTRY SetHook()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	g_pOldCreateFontIndirectW = DetourFindFunction("GDI32.dll","CreateFontIndirectW");
	DetourAttach(&g_pOldCreateFontIndirectW,NewCreateFontIndirectW);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	g_pOldSetWindowTextA = DetourFindFunction("USER32.dll", "SetWindowTextA");
	DetourAttach(&g_pOldSetWindowTextA, NewSetWindowTextA);
	DetourTransactionCommit();

}

__declspec(dllexport)void WINAPI Dummy()
{
}

static HMODULE s_hDll;
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//s_hDll = hModule;
		//DisableThreadLibraryCalls(hModule);
		SetHook();
		//MessageBoxW(0, L"Hooked!", L"Info", 0);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		//DropHook();
		break;
	}
	return TRUE;
}
