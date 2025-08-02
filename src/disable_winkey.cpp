#include <napi.h>
#include <windows.h>
#include <iostream>

// 全局变量
HHOOK g_hHook = NULL;
HANDLE g_hThread = NULL;
DWORD g_dwThreadId = 0;
HINSTANCE g_hModule = NULL;

bool g_isWinKeyDisabled = false;
bool g_isAltTabDisabled = false;

// 底层键盘钩子回调函数
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;

        // 检查是否禁用Win键
        if (g_isWinKeyDisabled && (p->vkCode == VK_LWIN || p->vkCode == VK_RWIN)) {
            return 1; // 阻止消息
        }

        // 检查是否禁用Alt+Tab
        if (g_isAltTabDisabled && p->vkCode == VK_TAB && (p->flags & LLKHF_ALTDOWN)) {
            return 1; // 阻止消息
        }
    }
    return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

// 钩子线程函数
DWORD WINAPI HookThreadProc(LPVOID lpParameter) {
    g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, g_hModule, 0);
    if (g_hHook == NULL) {
        return 1;
    }

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

// 启动钩子线程（如果需要）
void EnsureHookThreadRunning(Napi::Env env) {
    if (g_hThread == NULL) {
        g_hThread = CreateThread(NULL, 0, HookThreadProc, NULL, 0, &g_dwThreadId);
        if (g_hThread == NULL) {
            Napi::TypeError::New(env, "Failed to create hook thread").ThrowAsJavaScriptException();
            return;
        }
        Sleep(100); // 等待钩子安装
        if (g_hHook == NULL) {
             Napi::TypeError::New(env, "Failed to install keyboard hook").ThrowAsJavaScriptException();
        }
    }
}

// 停止钩子线程（如果不再需要）
void StopHookThreadIfNeeded() {
    if (g_hThread != NULL && !g_isWinKeyDisabled && !g_isAltTabDisabled) {
        PostThreadMessage(g_dwThreadId, WM_QUIT, 0, 0);
        WaitForSingleObject(g_hThread, 1000);
        CloseHandle(g_hThread);
        g_hThread = NULL;
        g_dwThreadId = 0;
        g_hHook = NULL;
    }
}

Napi::Value DisableWinKey(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    EnsureHookThreadRunning(env);
    g_isWinKeyDisabled = true;
    return Napi::Boolean::New(env, true);
}

Napi::Value EnableWinKey(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    g_isWinKeyDisabled = false;
    StopHookThreadIfNeeded();
    return Napi::Boolean::New(env, true);
}

Napi::Value DisableAltTab(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    EnsureHookThreadRunning(env);
    g_isAltTabDisabled = true;
    return Napi::Boolean::New(env, true);
}

Napi::Value EnableAltTab(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    g_isAltTabDisabled = false;
    StopHookThreadIfNeeded();
    return Napi::Boolean::New(env, true);
}

Napi::Value IsWinKeyDisabled(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, g_isWinKeyDisabled);
}

Napi::Value IsAltTabDisabled(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, g_isAltTabDisabled);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery((LPCVOID)Init, &mbi, sizeof(mbi));
    g_hModule = (HINSTANCE)mbi.AllocationBase;

    exports.Set("disableWinKey", Napi::Function::New(env, DisableWinKey));
    exports.Set("enableWinKey", Napi::Function::New(env, EnableWinKey));
    exports.Set("isWinKeyDisabled", Napi::Function::New(env, IsWinKeyDisabled));
    exports.Set("disableAltTab", Napi::Function::New(env, DisableAltTab));
    exports.Set("enableAltTab", Napi::Function::New(env, EnableAltTab));
    exports.Set("isAltTabDisabled", Napi::Function::New(env, IsAltTabDisabled));
    return exports;
}

NODE_API_MODULE(disable_winkey, Init);