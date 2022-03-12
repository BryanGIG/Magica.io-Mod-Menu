#pragma once

#ifndef OBFUSCATE_HEADER
#include <obfuscate.h>
#endif

#define getFieldOffset(image, namespaze, clas, method) Il2Cpp::GetFieldOffset(image, namespaze, clas, OBFUSCATE(method))
#define getMethodOffset(image, namespaze, clas, method, param) (uintptr_t) Il2Cpp::GetMethodOffset(image, namespaze, clas, OBFUSCATE(method), param)
#define getFieldStaticVal(image, namespaze, clas, method, out) Il2Cpp::GetStaticFieldValue(image,namespaze,clas,OBFUSCATE(method),out)
#define setPointer(func, image, namespaze, clas, method, param) func = Il2Cpp::GetMethod<decltype(func)>(image, namespaze, clas, OBFUSCATE(method),param)

#include "Unity/Unity.h"
#include "Unity/Matrix4x4.hpp"
#include "Camera.hpp"
#include "GameCore.hpp"
#include "Character.hpp"
#include "PlayerData.hpp"
#include "PlayerBalance.hpp"
#include "BaseCharStatus.hpp"
