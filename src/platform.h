#ifndef PLATFORM_H
#define PLATFORM_H

#include <napi.h>

// Declarations for platform-specific functions that must be implemented
// in win_impl.cpp and mac_impl.cpp.
void EnsureHookThreadRunning(Napi::Env env);
void StopHookThreadIfNeeded();
void PlatformInit(Napi::Env env, Napi::Object exports);

// Declarations for global state variables defined in addon.cpp
extern bool g_isWinKeyDisabled;
extern bool g_isAltTabDisabled;

#endif // PLATFORM_H
