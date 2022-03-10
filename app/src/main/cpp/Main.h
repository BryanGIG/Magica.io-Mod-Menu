#include <list>
#include <vector>
#include <cstring>
#include <pthread.h>
#include <jni.h>
#include <map>
#include <fstream>
#include <sstream>
#include <any>
#include <obfuscate.h>
#include <Logger.h>
#include <Utils.h>
#include "mainGame/Unity/API.h"
#include "mainGame/Includes.hpp"
#include <Esp.h>
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

std::map<std::string, std::any> Configs;

Vector2 screen;
