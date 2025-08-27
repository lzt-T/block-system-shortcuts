#include "platform.h"
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <Carbon/Carbon.h> // 添加这行来包含虚拟键码定义
#include <iostream>

// macOS-specific global variables
CFMachPortRef g_eventTap = NULL;
CFRunLoopSourceRef g_runLoopSource = NULL;
CFRunLoopRef g_runLoop = NULL;
pthread_t g_thread;

// macOS event tap callback
CGEventRef EventTapCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon)
{
    if (type == kCGEventKeyDown || type == kCGEventKeyUp || type == kCGEventFlagsChanged)
    {
        CGEventFlags flags = CGEventGetFlags(event);
        CGKeyCode keyCode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

        if (g_isWinKeyDisabled && (flags & kCGEventFlagMaskCommand) && (keyCode == 55 || keyCode == 54))
        {
            return NULL;
        }

        if (g_isAltTabDisabled && (flags & kCGEventFlagMaskCommand) && keyCode == kVK_Tab)
        {
            return NULL;
        }

        if (g_isAltKeyDisabled && (flags & kCGEventFlagMaskAlternate))
        {
            return NULL;
        }

        if (g_isF11KeyDisabled && keyCode == kVK_F11)
        {
            return NULL;
        }

        if (g_isCtrlKeyDisabled && (flags & kCGEventFlagMaskControl))
        {
            return NULL;
        }

        // F3键的全面拦截处理
        if (g_isF3KeyDisabled)
        {
            // 调试输出：显示键码和标志（仅在开发时使用）
            #ifdef DEBUG
            std::cout << "Key pressed: keyCode=" << keyCode << ", flags=" << flags << std::endl;
            #endif
            
            // 直接按F3键
            if (keyCode == kVK_F3)
            {
                return NULL;
            }
            
            // Fn+F3组合键（带有Function标志）
            if ((flags & kCGEventFlagMaskFunction) && keyCode == kVK_F3)
            {
                return NULL;
            }
            
            // Mission Control 功能键（可能的键码）
            if (keyCode == 160 || keyCode == 131 || keyCode == 179) // 常见的Mission Control键码
            {
                return NULL;
            }
        }
    }
    return event;
}

// Thread function to run the macOS event loop
void *RunLoopThread(void *arg)
{
    g_runLoop = CFRunLoopGetCurrent();

    g_eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault, kCGEventMaskForAllEvents, EventTapCallback, NULL);
    if (!g_eventTap)
    {
        std::cerr << "Failed to create event tap" << std::endl;
        return NULL;
    }

    g_runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, g_eventTap, 0);
    CFRunLoopAddSource(g_runLoop, g_runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(g_eventTap, true);

    CFRunLoopRun();

    CGEventTapEnable(g_eventTap, false);
    if (g_runLoopSource)
        CFRelease(g_runLoopSource);
    if (g_eventTap)
        CFRelease(g_eventTap);
    g_runLoop = NULL;
    g_eventTap = NULL;
    g_runLoopSource = NULL;

    return NULL;
}

void EnsureHookThreadRunning(Napi::Env env)
{
    if (!g_runLoop)
    {
        if (pthread_create(&g_thread, NULL, RunLoopThread, NULL) != 0)
        {
            Napi::TypeError::New(env, "Failed to create thread").ThrowAsJavaScriptException();
            return;
        }
        sleep(1);
        if (!g_eventTap)
        {
            Napi::TypeError::New(env, "Failed to install keyboard hook").ThrowAsJavaScriptException();
        }
    }
}

void StopHookThreadIfNeeded()
{
    if (g_runLoop && !g_isWinKeyDisabled && !g_isAltTabDisabled && !g_isAltKeyDisabled && !g_isF11KeyDisabled && !g_isCtrlKeyDisabled && !g_isF3KeyDisabled)
    {
        CFRunLoopStop(g_runLoop);
        pthread_join(g_thread, NULL);
    }
}

void PlatformInit(Napi::Env env, Napi::Object exports)
{
    // No-op for macOS
}
