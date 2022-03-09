#include <list>
#include <vector>
#include <cstring>
#include <pthread.h>
#include <jni.h>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <obfuscate.h>
#include <Logger.h>
#include <Utils.h>
#include "Unity/API.h"
#include "Unity/Unity.h"
#include <Esp.h>
#include <Gameclass.h>
#include <Tools.h>

#define HOOK_DEF(ret, func, ...) \
  ret (*orig_##func)(__VA_ARGS__); \
  ret new_##func(__VA_ARGS__)

struct PlayerData {
    bool isLegit = false;
    Vector2 Location;
};

struct Response {
    int PlayerCount = -1;
    PlayerData Players[maxplayerCount];
} resp;

std::map<std::string, u_long> Configs;

Vector2 screen;
