#include <list>
#include <vector>
#include <cstring>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Includes/obfuscate.h"
#include "Includes/Logger.h"
#include "Includes/Utils.h"
#include "Substrate/SubstrateHook.h"
#include "Substrate/CydiaSubstrate.h"
#include "Unity/API.h"
#include "Unity/Unity.h"
#include "Includes/esp.h"
#include "Includes/Gameclass.h"
#include "Includes/Tools.h"

#define targetLibName OBFUSCATE("liblogic.so")

struct PlayerData {
    bool isLegit = false;
    string heroName;
    string playerName;
    Vector2 Location;
    std::vector<int> CoolDown;
};

struct Response {
    int PlayerCount = -1;
    PlayerData Players[maxplayerCount];
} resp;

std::map<std::string, u_long> Configs;

Vector2 screen;
bool changeDrone = false;

void (*oOnUpdate)(void *);

void (*oSetParam)(void *, Vector3, Quaternion);
