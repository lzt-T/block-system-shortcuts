#include "platform.h"

// Define global state variables
bool g_isWinKeyDisabled = false;
bool g_isAltTabDisabled = false;

// --- Common N-API Functions ---

Napi::Value DisableSuperKey(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    EnsureHookThreadRunning(env);
    g_isWinKeyDisabled = true;
    return Napi::Boolean::New(env, true);
}

Napi::Value EnableSuperKey(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    g_isWinKeyDisabled = false;
    StopHookThreadIfNeeded();
    return Napi::Boolean::New(env, true);
}

Napi::Value IsSuperKeyDisabled(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, g_isWinKeyDisabled);
}

Napi::Value DisableAppSwitch(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    EnsureHookThreadRunning(env);
    g_isAltTabDisabled = true;
    return Napi::Boolean::New(env, true);
}

Napi::Value EnableAppSwitch(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    g_isAltTabDisabled = false;
    StopHookThreadIfNeeded();
    return Napi::Boolean::New(env, true);
}

Napi::Value IsAppSwitchDisabled(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, g_isAltTabDisabled);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    PlatformInit(env, exports);

    exports.Set("disableSuperKey", Napi::Function::New(env, DisableSuperKey));
    exports.Set("enableSuperKey", Napi::Function::New(env, EnableSuperKey));
    exports.Set("isSuperKeyDisabled", Napi::Function::New(env, IsSuperKeyDisabled));
    exports.Set("disableAppSwitch", Napi::Function::New(env, DisableAppSwitch));
    exports.Set("enableAppSwitch", Napi::Function::New(env, EnableAppSwitch));
    exports.Set("isAppSwitchDisabled", Napi::Function::New(env, IsAppSwitchDisabled));
    return exports;
}

NODE_API_MODULE(disable_winkey, Init)
