#include <napi.h>
#include <windows.h>
#include <iostream>

// 全局变量
HHOOK g_hHook = NULL;
bool g_isDisabled = false;
HANDLE g_hThread = NULL;
DWORD g_dwThreadId = 0;
HINSTANCE g_hModule = NULL;

// 底层键盘钩子回调函数
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;
        // 检查是否为Windows键 (左或右)
        if (p->vkCode == VK_LWIN || p->vkCode == VK_RWIN) {
            // 阻止消息继续传播
            return 1;
        }
    }
    return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

// 钩子线程函数
DWORD WINAPI HookThreadProc(LPVOID lpParameter) {
    // 为这个线程安装一个全局的底层键盘钩子
    g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, g_hModule, 0);
    if (g_hHook == NULL) {
        return 1; // 安装失败
    }

    // 运行消息循环以接收钩子消息
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 卸载钩子
    if (g_hHook) {
        UnhookWindowsHookEx(g_hHook);
        g_hHook = NULL;
    }

    return 0;
}

// 禁用Windows键
Napi::Value DisableWinKey(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (g_isDisabled) {
        return Napi::Boolean::New(env, true);
    }

    // 创建一个新线程来管理钩子和消息循环
    g_hThread = CreateThread(NULL, 0, HookThreadProc, NULL, 0, &g_dwThreadId);

    if (g_hThread == NULL) {
        Napi::TypeError::New(env, "Failed to create hook thread").ThrowAsJavaScriptException();
        return env.Null();
    }

    // 等待一小段时间以确保钩子已安装
    // 注意：这是一个简化的同步方法，更健壮的实现应使用事件对象
    Sleep(100);

    if (g_hHook == NULL) {
        WaitForSingleObject(g_hThread, 1000);
        CloseHandle(g_hThread);
        g_hThread = NULL;
        Napi::TypeError::New(env, "Failed to install keyboard hook").ThrowAsJavaScriptException();
        return env.Null();
    }

    g_isDisabled = true;
    return Napi::Boolean::New(env, true);
}

// 启用Windows键
Napi::Value EnableWinKey(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (!g_isDisabled) {
        return Napi::Boolean::New(env, true);
    }

    if (g_hThread != NULL) {
        // 向钩子线程发送退出消息
        PostThreadMessage(g_dwThreadId, WM_QUIT, 0, 0);
        // 等待线程结束
        WaitForSingleObject(g_hThread, 1000);
        CloseHandle(g_hThread);
        g_hThread = NULL;
        g_dwThreadId = 0;
    }
    
    g_hHook = NULL;
    g_isDisabled = false;
    return Napi::Boolean::New(env, true);
}

// 检查Windows键是否被禁用
Napi::Value IsWinKeyDisabled(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, g_isDisabled);
}

// 模块初始化
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    // 在加载时获取模块句柄
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery((LPCVOID)Init, &mbi, sizeof(mbi));
    g_hModule = (HINSTANCE)mbi.AllocationBase;

    exports.Set(Napi::String::New(env, "disableWinKey"), Napi::Function::New(env, DisableWinKey));
    exports.Set(Napi::String::New(env, "enableWinKey"), Napi::Function::New(env, EnableWinKey));
    exports.Set(Napi::String::New(env, "isWinKeyDisabled"), Napi::Function::New(env, IsWinKeyDisabled));
    return exports;
}

NODE_API_MODULE(disable_winkey, Init)