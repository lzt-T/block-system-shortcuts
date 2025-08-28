#include "platform.h"

// Define global state variables
bool g_isWinKeyDisabled = false;
bool g_isAltTabDisabled = false;
bool g_isAltKeyDisabled = false;
bool g_isF11KeyDisabled = false;
bool g_isCtrlKeyDisabled = false;
bool g_isF3KeyDisabled = false;

// Function to disable all keys
Napi::Value disableAll(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    EnsureHookThreadRunning(env);
    g_isWinKeyDisabled = true;
    g_isAltTabDisabled = true;
    g_isAltKeyDisabled = true;
    g_isF11KeyDisabled = true;
    g_isCtrlKeyDisabled = true;
    g_isF3KeyDisabled = true;
    return Napi::Boolean::New(env, true);
}

// Function to enable all keys
Napi::Value enableAll(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    g_isWinKeyDisabled = false;
    g_isAltTabDisabled = false;
    g_isAltKeyDisabled = false;
    g_isF3KeyDisabled = false;
    g_isCtrlKeyDisabled = false;
    g_isF11KeyDisabled = false;
    StopHookThreadIfNeeded();
    return Napi::Boolean::New(env, true);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    PlatformInit(env, exports);

    exports.Set("disableAll", Napi::Function::New(env, disableAll));
    exports.Set("enableAll", Napi::Function::New(env, enableAll));
    return exports;
}

NODE_API_MODULE(disable_winkey, Init)
