#include "platform.h"
#include <windows.h>

// Windows-specific global variables
HHOOK g_hHook = NULL;
HANDLE g_hThread = NULL;
DWORD g_dwThreadId = 0;
HINSTANCE g_hModule = NULL;

// Windows low-level keyboard hook callback
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;
        if (g_isWinKeyDisabled && (p->vkCode == VK_LWIN || p->vkCode == VK_RWIN)) {
            return 1;
        }
        if (g_isAltTabDisabled && p->vkCode == VK_TAB && (GetKeyState(VK_MENU) & 0x8000)) {
            return 1;
        }
        if (g_isAltKeyDisabled && (p->vkCode == VK_LMENU || p->vkCode == VK_RMENU)) {
            return 1;
        }
        if (g_isF11KeyDisabled && p->vkCode == VK_F11) {
            return 1;
        }
    }
    return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

// Windows hook thread function
DWORD WINAPI HookThreadProc(LPVOID lpParameter) {
    g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, g_hModule, 0);
    if (g_hHook == NULL) return 1;

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (g_hHook) {
        UnhookWindowsHookEx(g_hHook);
        g_hHook = NULL;
    }
    return 0;
}

void EnsureHookThreadRunning(Napi::Env env) {
    if (g_hThread == NULL) {
        g_hThread = CreateThread(NULL, 0, HookThreadProc, NULL, 0, &g_dwThreadId);
        if (g_hThread == NULL) {
            Napi::TypeError::New(env, "Failed to create hook thread").ThrowAsJavaScriptException();
            return;
        }
        Sleep(100);
        if (g_hHook == NULL) {
            Napi::TypeError::New(env, "Failed to install keyboard hook").ThrowAsJavaScriptException();
        }
    }
}

void StopHookThreadIfNeeded() {
    if (g_hThread != NULL && !g_isWinKeyDisabled && !g_isAltTabDisabled && !g_isAltKeyDisabled && !g_isF11KeyDisabled) {
        PostThreadMessage(g_dwThreadId, WM_QUIT, 0, 0);
        WaitForSingleObject(g_hThread, 1000);
        CloseHandle(g_hThread);
        g_hThread = NULL;
        g_dwThreadId = 0;
        g_hHook = NULL;
    }
}

void PlatformInit(Napi::Env env, Napi::Object exports) {
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery((LPCVOID)PlatformInit, &mbi, sizeof(mbi));
    g_hModule = (HINSTANCE)mbi.AllocationBase;
}
