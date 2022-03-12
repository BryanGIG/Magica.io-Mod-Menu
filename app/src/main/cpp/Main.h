#include <list>
#include <vector>
#include <cstring>
#include <pthread.h>
#include <jni.h>
#include <map>
#include <fstream>
#include <any>
#include <obfuscate.h>
#include <Logger.h>
#include <Utils.h>
#include "mainGame/Unity/API.h"
#include "mainGame/Includes.hpp"
#include <Esp.h>
#if !defined(__LP64__)

#include "Substrate/CydiaSubstrate.h"

#define Hook(addr, func) MSHookFunction((void*)addr, (void*)new_func, (void**)&orig_func)
#else

#include "And64InlineHook/And64InlineHook.hpp"

#define Hook(addr, func) A64HookFunction((void*)addr, (void*)new_##func, (void**)&orig_##func)
#endif

#define HOOK_DEF(ret, func, ...) \
  ret (*orig_##func)(__VA_ARGS__); \
  ret new_##func(__VA_ARGS__)

std::map<std::string, std::any> Configs;

Vector2 screen;
