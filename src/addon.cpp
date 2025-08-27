#include "platform.h"

// Define global state variables
bool g_isWinKeyDisabled = false;
bool g_isAltTabDisabled = false;
bool g_isAltKeyDisabled = false;
bool g_isF11KeyDisabled = false;
bool g_isCtrlKeyDisabled = false;
bool g_isF3KeyDisabled = false;

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

Napi::Value DisableAltKey(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    EnsureHookThreadRunning(env);
    g_isAltKeyDisabled = true;
    return Napi::Boolean::New(env, true);
}

Napi::Value EnableAltKey(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    g_isAltKeyDisabled = false;
    StopHookThreadIfNeeded();
    return Napi::Boolean::New(env, true);
}

Napi::Value IsAltKeyDisabled(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, g_isAltKeyDisabled);
}

Napi::Value DisableF11Key(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    EnsureHookThreadRunning(env);
    g_isF11KeyDisabled = true;
    return Napi::Boolean::New(env, true);
}

Napi::Value EnableF11Key(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    g_isF11KeyDisabled = false;
    StopHookThreadIfNeeded();
    return Napi::Boolean::New(env, true);
}

Napi::Value IsF11KeyDisabled(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, g_isF11KeyDisabled);
}

Napi::Value DisableCtrlKey(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    EnsureHookThreadRunning(env);
    g_isCtrlKeyDisabled = true;
    return Napi::Boolean::New(env, true);
}

Napi::Value EnableCtrlKey(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    g_isCtrlKeyDisabled = false;
    StopHookThreadIfNeeded();
    return Napi::Boolean::New(env, true);
}

Napi::Value IsCtrlKeyDisabled(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, g_isCtrlKeyDisabled);
}

Napi::Value DisableF3Key(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    EnsureHookThreadRunning(env);
    g_isF3KeyDisabled = true;
    return Napi::Boolean::New(env, true);
}

Napi::Value EnableF3Key(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    g_isF3KeyDisabled = false;
    StopHookThreadIfNeeded();
    return Napi::Boolean::New(env, true);
}

Napi::Value IsF3KeyDisabled(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, g_isF3KeyDisabled);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    PlatformInit(env, exports);

    exports.Set("disableSuperKey", Napi::Function::New(env, DisableSuperKey));
    exports.Set("enableSuperKey", Napi::Function::New(env, EnableSuperKey));
    exports.Set("isSuperKeyDisabled", Napi::Function::New(env, IsSuperKeyDisabled));
    exports.Set("disableAppSwitch", Napi::Function::New(env, DisableAppSwitch));
    exports.Set("enableAppSwitch", Napi::Function::New(env, EnableAppSwitch));
    exports.Set("isAppSwitchDisabled", Napi::Function::New(env, IsAppSwitchDisabled));
    exports.Set("disableAltKey", Napi::Function::New(env, DisableAltKey));
    exports.Set("enableAltKey", Napi::Function::New(env, EnableAltKey));
    exports.Set("isAltKeyDisabled", Napi::Function::New(env, IsAltKeyDisabled));
    exports.Set("disableF11Key", Napi::Function::New(env, DisableF11Key));
    exports.Set("enableF11Key", Napi::Function::New(env, EnableF11Key));
    exports.Set("isF11KeyDisabled", Napi::Function::New(env, IsF11KeyDisabled));
    exports.Set("disableCtrlKey", Napi::Function::New(env, DisableCtrlKey));
    exports.Set("enableCtrlKey", Napi::Function::New(env, EnableCtrlKey));
    exports.Set("isCtrlKeyDisabled", Napi::Function::New(env, IsCtrlKeyDisabled));
    exports.Set("disableF3Key", Napi::Function::New(env, DisableF3Key));
    exports.Set("enableF3Key", Napi::Function::New(env, EnableF3Key));
    exports.Set("isF3KeyDisabled", Napi::Function::New(env, IsF3KeyDisabled));
    return exports;
}

NODE_API_MODULE(disable_winkey, Init)
