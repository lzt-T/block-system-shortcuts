#include "platform.h"
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <iostream>

// macOS-specific global variables
CFMachPortRef g_eventTap = NULL;
CFRunLoopSourceRef g_runLoopSource = NULL;
CFRunLoopRef g_runLoop = NULL;
pthread_t g_thread;

// macOS event tap callback
CGEventRef EventTapCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    if (type == kCGEventKeyDown || type == kCGEventKeyUp || type == kCGEventFlagsChanged) {
        CGEventFlags flags = CGEventGetFlags(event);
        CGKeyCode keyCode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

        if (g_isWinKeyDisabled && (flags & kCGEventFlagMaskCommand) && (keyCode == 55 || keyCode == 54)) {
            return NULL;
        }
        
        if (g_isAltTabDisabled && (flags & kCGEventFlagMaskCommand) && keyCode == kVK_Tab) {
            return NULL;
        }
    }
    return event;
}

// Thread function to run the macOS event loop
void* RunLoopThread(void* arg) {
    g_runLoop = CFRunLoopGetCurrent();
    
    g_eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault, kCGEventMaskForAllEvents, EventTapCallback, NULL);
    if (!g_eventTap) {
        std::cerr << "Failed to create event tap" << std::endl;
        return NULL;
    }

    g_runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, g_eventTap, 0);
    CFRunLoopAddSource(g_runLoop, g_runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(g_eventTap, true);
    
    CFRunLoopRun();
    
    CGEventTapEnable(g_eventTap, false);
    if(g_runLoopSource) CFRelease(g_runLoopSource);
    if(g_eventTap) CFRelease(g_eventTap);
    g_runLoop = NULL;
    g_eventTap = NULL;
    g_runLoopSource = NULL;

    return NULL;
}

void EnsureHookThreadRunning(Napi::Env env) {
    if (!g_runLoop) {
        if (pthread_create(&g_thread, NULL, RunLoopThread, NULL) != 0) {
            Napi::TypeError::New(env, "Failed to create thread").ThrowAsJavaScriptException();
            return;
        }
        sleep(1);
        if(!g_eventTap){
             Napi::TypeError::New(env, "Failed to install keyboard hook").ThrowAsJavaScriptException();
        }
    }
}

void StopHookThreadIfNeeded() {
    if (g_runLoop && !g_isWinKeyDisabled && !g_isAltTabDisabled) {
        CFRunLoopStop(g_runLoop);
        pthread_join(g_thread, NULL);
    }
}

void PlatformInit(Napi::Env env, Napi::Object exports) {
    // No-op for macOS
}
