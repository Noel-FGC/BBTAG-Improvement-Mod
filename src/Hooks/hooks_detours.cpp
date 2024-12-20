#include "hooks_detours.h"

#include "HookManager.h"

#include "Core/interfaces.h"
#include "Core/logger.h"
#include "D3D9EXWrapper/ID3D9Wrapper_Sprite.h"
#include "D3D9EXWrapper/ID3DXWrapper_Effect.h"
#include "D3D9EXWrapper/ID3D9EXWrapper.h"
#include "SteamApiWrapper/SteamMatchmakingWrapper.h"
#include "SteamApiWrapper/SteamNetworkingWrapper.h"

#include <detours.h>
#include <steam_api.h>

#pragma comment(lib, "detours.lib")
#pragma comment(lib, "steam_api.lib")


typedef HRESULT(__stdcall *Direct3DCreate9Ex_t)(UINT SDKVersion, IDirect3D9Ex**);
typedef HRESULT(APIENTRY *D3DXCreateEffect_t)(LPDIRECT3DDEVICE9, LPCVOID, UINT, CONST D3DXMACRO*, LPD3DXINCLUDE, DWORD, LPD3DXEFFECTPOOL, LPD3DXEFFECT*, LPD3DXBUFFER*);
typedef HRESULT(WINAPI *D3DXCreateSprite_t)(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE* ppSprite);
typedef bool (WINAPI *SteamAPI_Init_t)();
typedef HWND(__stdcall *CreateWindowExW_t)(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName,
	DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
typedef BOOL(WINAPI* SetWindowPos_t)(_In_ HWND hWnd, _In_opt_ HWND hWndInsertAfter, _In_ int X,
	_In_ int Y, _In_ int cx, _In_ int cy, _In_ UINT uFlags);

Direct3DCreate9Ex_t orig_Direct3DCreate9Ex;
D3DXCreateEffect_t orig_D3DXCreateEffect;
D3DXCreateSprite_t orig_D3DXCreateSprite;
SteamAPI_Init_t orig_SteamAPI_Init;
CreateWindowExW_t orig_CreateWindowExW;
SetWindowPos_t orig_SetWindowPos;

HRESULT __stdcall hook_Direct3DCreate9Ex(UINT sdkVers, IDirect3D9Ex** pD3DEx)
{
	LOG(1, "Direct3DCreate9EX pD3DEx: 0x%p\n", pD3DEx);
	HRESULT retval = orig_Direct3DCreate9Ex(sdkVers, pD3DEx); // real one

	Direct3D9ExWrapper *ret = new Direct3D9ExWrapper(&*pD3DEx);
	return retval;
}

HRESULT APIENTRY hook_D3DXCreateEffect(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataLen,
	CONST D3DXMACRO* pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXEFFECT* ppEffect,
	LPD3DXBUFFER* ppCompilationErrors)
{
	LOG(7, "D3DXCreateEffect\n");
	HRESULT hR = orig_D3DXCreateEffect(pDevice, pSrcData, SrcDataLen, pDefines, pInclude,
		Flags, pPool, ppEffect, ppCompilationErrors);
	if (SUCCEEDED(hR))
	{
		ID3DXEffectWrapper *ret = new ID3DXEffectWrapper(&ppEffect);
	}

	return hR;
}

HRESULT WINAPI hook_D3DXCreateSprite(LPDIRECT3DDEVICE9 pDevice, LPD3DXSPRITE* ppSprite)
{
	LOG(7, "D3DXCreateSprite\n");
	HRESULT hR = orig_D3DXCreateSprite(pDevice, ppSprite);
	if (SUCCEEDED(hR))
	{
		ID3DXSpriteWrapper *ret = new ID3DXSpriteWrapper(&ppSprite);
	}
	return hR;
}

DWORD SteamMatchmakingFuncJmpBackAddr = 0;
void __declspec(naked)GetSteamMatchmaking()
{
	LOG_ASM(2, "GetSteamMatchmaking\n");

	__asm
	{
		call dword ptr[eax + 28h]
		/////
		pushad
		add esi, 10h
		mov g_tempVals.ppSteamMatchmaking, esi
		popad
		/////
		mov[esi + 10h], eax
		jmp[SteamMatchmakingFuncJmpBackAddr]
	}
}

DWORD SteamNetworkingFuncJmpBackAddr = 0;
void __declspec(naked)GetSteamNetworking()
{
	LOG_ASM(2, "GetSteamNetworking\n");

	__asm
	{
		call dword ptr[eax + 40h]
		/////
		pushad
		add esi, 20h
		mov g_tempVals.ppSteamNetworking, esi
		popad
		/////
		mov[esi + 20h], eax
		jmp[SteamNetworkingFuncJmpBackAddr]
	}
}

DWORD SteamUserFuncJmpBackAddr = 0;
void __declspec(naked)GetSteamUser()
{
	LOG_ASM(2, "GetSteamUser\n");

	__asm
	{
		call dword ptr[eax + 14h]
		/////
		pushad
		add esi, 4h
		mov g_tempVals.ppSteamUser, esi
		popad
		/////
		mov[esi + 4h], eax
		jmp[SteamUserFuncJmpBackAddr]
	}
}

DWORD SteamFriendsFuncJmpBackAddr = 0;
void __declspec(naked)GetSteamFriends()
{
	LOG_ASM(2, "GetSteamFriends\n");

	__asm
	{
		call dword ptr[eax + 20h]
		/////
		pushad
		add esi, 8h
		mov g_tempVals.ppSteamFriends, esi
		popad
		/////
		mov[esi + 8h], eax
		jmp[SteamFriendsFuncJmpBackAddr]
	}
}

DWORD SteamUtilsFuncJmpBackAddr = 0;
void __declspec(naked)GetSteamUtils()
{

	LOG_ASM(2, "GetSteamUtils\n");
	
	__asm
	{
		call dword ptr[eax + 24h]
		/////
		pushad
		add esi, 0Ch
		mov g_tempVals.ppSteamUtils, esi
		popad
		/////
		mov[esi + 0Ch], eax
		jmp[SteamUtilsFuncJmpBackAddr]
	}
}

DWORD SteamUserStatsFuncJmpBackAddr = 0;
void __declspec(naked)GetSteamUserStats()
{
	
	LOG_ASM(2, "GetSteamUserStats\n");
	
	__asm
	{
		call dword ptr[eax + 34h]
		/////
		pushad
		add esi, 14h
		mov g_tempVals.ppSteamUserStats, esi
		popad
		/////
		mov[esi + 14h], eax
		jmp[SteamUserStatsFuncJmpBackAddr]
	}
}

bool WINAPI hook_SteamAPI_Init()
{
	LOG(1, "SteamAPI_Init\n");
	bool ret = orig_SteamAPI_Init();

	SteamMatchmakingFuncJmpBackAddr = HookManager::SetHook("SteamMatchmaking",
		"\xff\x50\x28\x89\x46\x10\x85\xc0", "xxxxxxxx", 6, GetSteamMatchmaking);

	SteamNetworkingFuncJmpBackAddr = HookManager::SetHook("SteamNetworking",
		"\xff\x50\x40\x89\x46\x20\x85\xc0", "xxxxxxxx", 6, GetSteamNetworking);

	SteamUserFuncJmpBackAddr = HookManager::SetHook("SteamUser",
		"\xff\x50\x14\x89\x46\x04", "xxxxxx", 6, GetSteamUser);

	SteamFriendsFuncJmpBackAddr = HookManager::SetHook("SteamFriends",
		"\xff\x50\x20\x89\x46\x08", "xxxxxx", 6, GetSteamFriends);

	SteamUtilsFuncJmpBackAddr = HookManager::SetHook("SteamUtils",
		"\xff\x50\x24\x89\x46\x0c", "xxxxxx", 6, GetSteamUtils);

	SteamUserStatsFuncJmpBackAddr = HookManager::SetHook("SteamUserStats",
		"\xff\x50\x34\x89\x46\x14", "xxxxxx", 6, GetSteamUserStats);

	return ret;
}

HWND WINAPI hook_CreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName,
	DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	LOG(2, "CreateWindowExW X:%d Y:%d nWidth:%d nHeight:%d ", X, Y, nWidth, nHeight);

	if (nWidth > 1280 && !g_gameProc.hWndGameWindow)
	{
		nWidth = Settings::settingsIni.windowwidth;
		nHeight = Settings::settingsIni.windowheight;
	}
	HWND hWnd = orig_CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y,
		nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

	LOG(2, "HWND: 0x%p\n", hWnd);
	if (SUCCEEDED(hWnd))
	{
		LOG(7, "\tSuccess: 0x%p\n", hWnd);
		if (nWidth > 0 && !g_gameProc.hWndGameWindow) // 2nd created window should be the correct one according to process hacker
		{
			LOG(2, "Correct window: 0x%p\n", hWnd);
			g_gameProc.hWndGameWindow = hWnd;

			//adjusting rendering resolution, due to the window borders
			if (Settings::settingsIni.displaymode == DISPLAYMODE_WINDOWED &&
				(Settings::settingsIni.windowwidth == Settings::settingsIni.renderwidth &&
					Settings::settingsIni.windowheight == Settings::settingsIni.renderheight))
			{
				RECT clientRect;
				GetClientRect(hWnd, &clientRect);

				//modifying with new values
				Settings::settingsIni.renderwidth = clientRect.right;
				Settings::settingsIni.renderheight = clientRect.bottom;
				Settings::savedSettings.newSourceRect.right = Settings::settingsIni.renderwidth;
				Settings::savedSettings.newSourceRect.bottom = Settings::settingsIni.renderheight;
				Settings::savedSettings.newViewport.Width = Settings::settingsIni.renderwidth;
				Settings::savedSettings.newViewport.Height = Settings::settingsIni.renderheight;

				LOG(2, "Renderwidth, Renderheight, newSourceRect, newViewport modified to: %d x %d\n",
					Settings::settingsIni.renderwidth, Settings::settingsIni.renderheight);
			}
		}
	}

	return hWnd;
}

BOOL WINAPI hook_SetWindowPos(_In_ HWND hWnd, _In_opt_ HWND hWndInsertAfter, _In_ int X,
	_In_ int Y, _In_ int cx, _In_ int cy, _In_ UINT uFlags)
{
	LOG(2, "hook_SetWindowPos: Hwnd: 0x%p, X: %d, Y: %d, Flags: 0x%p\n", hWnd, cx, cy, uFlags);
	if (g_gameProc.hWndGameWindow == hWnd && Settings::settingsIni.viewportoverride == VIEWPORT_OVERRIDE)
	{
		cx = Settings::settingsIni.windowwidth;
		cy = Settings::settingsIni.windowheight;

		if (Settings::settingsIni.displaymode == DISPLAYMODE_BORDERLESS)
		{
			SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
			uFlags = uFlags & (SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
		else if (Settings::settingsIni.displaymode == DISPLAYMODE_WINDOWED)
		{
			SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) | WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME);

			RECT rc;
			GetWindowRect(hWnd, &rc);
			X = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
			Y = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;
		}
		//else if (Settings::settingsIni.displaymode == DISPLAYMODE_FULLSCREEN)
		//{
		//	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW | WS_EX_TOPMOST);
		//	uFlags = SWP_NOZORDER | SWP_NOMOVE;
		//}
		LOG(2, "\t modifying to: X: %d, Y: %d, Flags: 0x%p\n", cx, cy, uFlags);
	}

	return orig_SetWindowPos(hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
}

bool placeHooks_detours()
{
	LOG(1, "placeHooks_detours\n");
	HMODULE hM_d3d9 = GetModuleHandleA("d3d9.dll");
	HMODULE hM_d3dx9_43 = GetModuleHandleA("d3dx9_43.dll");
	HMODULE hM_steam_api = GetModuleHandleA("steam_api.dll");
	HMODULE hM_user32 = GetModuleHandleA("user32.dll");

	PBYTE pDirect3DCreate9Ex = (PBYTE)GetProcAddress(hM_d3d9, "Direct3DCreate9Ex");
	PBYTE pD3DXCreateEffect = (PBYTE)GetProcAddress(hM_d3dx9_43, "D3DXCreateEffect");
	PBYTE pD3DXCreateSprite = (PBYTE)GetProcAddress(hM_d3dx9_43, "D3DXCreateSprite");
	PBYTE pSteamAPI_Init = (PBYTE)GetProcAddress(hM_steam_api, "SteamAPI_Init");
	PBYTE pCreateWindowExW = (PBYTE)GetProcAddress(hM_user32, "CreateWindowExW");
	PBYTE pSetWindowPos = (PBYTE)GetProcAddress(hM_user32, "SetWindowPos");
	PBYTE pGetFocus = (PBYTE)GetProcAddress(hM_user32, "GetFocus");

	if (!hookSucceeded((PBYTE)pDirect3DCreate9Ex, "Direct3DCreate9Ex"))
		return false;
	if (!hookSucceeded((PBYTE)pD3DXCreateEffect, "D3DXCreateEffect"))
		return false;
	if (!hookSucceeded((PBYTE)pD3DXCreateSprite, "D3DXCreateSprite"))
		return false;
	if (!hookSucceeded((PBYTE)pSteamAPI_Init, "SteamAPI_Init"))
		return false;
	if (!hookSucceeded((PBYTE)pCreateWindowExW, "CreateWindowExW"))
		return false;
	if (!hookSucceeded((PBYTE)pSetWindowPos, "pSetWindowPos"))
		return false;
	
	orig_Direct3DCreate9Ex = (Direct3DCreate9Ex_t)DetourFunction(pDirect3DCreate9Ex, (LPBYTE)hook_Direct3DCreate9Ex);
	//orig_D3DXCreateEffect = (D3DXCreateEffect_t)DetourFunction(pD3DXCreateEffect, (LPBYTE)hook_D3DXCreateEffect);
	orig_D3DXCreateSprite = (D3DXCreateSprite_t)DetourFunction(pD3DXCreateSprite, (LPBYTE)hook_D3DXCreateSprite);
	orig_SteamAPI_Init = (SteamAPI_Init_t)DetourFunction(pSteamAPI_Init, (LPBYTE)hook_SteamAPI_Init);
	orig_CreateWindowExW = (CreateWindowExW_t)DetourFunction(pCreateWindowExW, (LPBYTE)hook_CreateWindowExW);
	orig_SetWindowPos = (SetWindowPos_t)DetourFunction(pSetWindowPos, (LPBYTE)hook_SetWindowPos);

	return true;
}