//
// Created by BryanGIG on 13/04/2021.
//

#ifndef ANDROID_MOD_MENU_GAMECLASS_H
#define ANDROID_MOD_MENU_GAMECLASS_H

#define getFieldOffset(image, namespaze, clas, method) Il2Cpp::GetFieldOffset(OBFUSCATE(image), OBFUSCATE(namespaze), OBFUSCATE(clas), OBFUSCATE(method))
#define getMethodOffset(image, namespaze, clas, method, param) (uintptr_t) Il2Cpp::GetMethodOffset(OBFUSCATE(image), OBFUSCATE(namespaze), OBFUSCATE(clas), OBFUSCATE(method), param)
#define getFieldStaticVal(image, namespaze, clas, method, out)Il2Cpp::GetStaticFieldValue(OBFUSCATE(image),OBFUSCATE(namespaze),OBFUSCATE(clas),OBFUSCATE(method),out)
#define setPointer(pointer, image, namespaze, clas, method, param) Il2Cpp::GetMethod<pointer>(OBFUSCATE(image), OBFUSCATE(namespaze), OBFUSCATE(clas), OBFUSCATE(method),param)

using func_void = void *(*)(void *);
using func_bool = bool (*)(void *);
using func_bool2 = bool (*)(void *, int);
using func_int = void *(*)(void *, int);
using func_uint = uint(*)(void *);
using func_List = List<void **> *(*)(void *);
using func_vector3 = Vector3(*)(void *);
using func_vector32 = Vector3(*)(void *, int);
using func_Matrix = Matrix4x4(*)(void *);

namespace SystemData {
    uintptr_t strName;

    List<void **> *(*GetBattlePlayerInfo)(void *);

    void init() {
        strName = getFieldOffset("Assembly-CSharp.dll", "", "SystemData.RoomData",
                                 "strName");
        SystemData::GetBattlePlayerInfo = setPointer(func_List, "Assembly-CSharp.dll", "",
                                                     "SystemData",
                                                     "GetBattlePlayerInfo", 0);
    }
}
namespace CData_Hero {
    uintptr_t m_SortIcon;

    void *(*GetInstance)(void *);//static
    void *(*GetValue_ByID)(void *, int ID);

    void init() {

        m_SortIcon = getFieldOffset("Assembly-CSharp.dll", "", "CData_Hero_Element",
                                    "m_SortIcon");
        CData_Hero::GetInstance = setPointer(func_void, "Assembly-CSharp.dll", "",
                                             "CData_HeroCostume", "GetInstance",
                                             0);
        CData_Hero::GetValue_ByID = setPointer(func_int, "Assembly-CSharp.dll", "",
                                               "CData_HeroCostume",
                                               "GetValue_ByID", 1);
    }
}

namespace BattleManager {
    uintptr_t m_ShowPlayers;

    void init() {
        m_ShowPlayers = getFieldOffset("Assembly-CSharp.dll", "", "BattleManager", "m_ShowPlayers");
    }
}
namespace SmoothFollow {
    uintptr_t SetCameraParam;
    void init() {
        SetCameraParam =getMethodOffset("Assembly-CSharp.dll", "", "SmoothFollow",
                        "SetCameraParam", 2);
    }
}
namespace ShowBattleCamera {
    void *(*ResetCameraParm)(void *);

    void init() {
        ShowBattleCamera::ResetCameraParm = setPointer(func_void, "Assembly-CSharp.dll", "",
                                                       "ShowBattleCamera",
                                                       "ResetCameraParm", 0);
    }
}
namespace CoolDownData {
    uintptr_t iSpellID;

    uint (*GetCoolTime)(void *);

    void init() {
        iSpellID = getFieldOffset("Assembly-CSharp.dll", "Battle", "CoolDownData",
                                  "iSpellID");
        GetCoolTime = setPointer(func_uint, "Assembly-CSharp.dll", "Battle", "CoolDownData",
                                 "GetCoolTime", 0);
    }
}
namespace LogicFighter {

    uintptr_t m_SkillComp;
    uintptr_t m_CoolDownComp;
    uintptr_t m_DicCoolInfo;

    bool (*IsEnemy)(void *, int);

    bool (*GetHPEmpty)(void *);

    Vector3 (*get_Position)(void *);

    void init() {

        m_SkillComp = getFieldOffset("Assembly-CSharp.dll", "Battle", "LogicFighter",
                                     "m_SkillComp");

        m_CoolDownComp = getFieldOffset("Assembly-CSharp.dll", "Battle", "LogicSkillComp",
                                        "m_CoolDownComp");

        m_DicCoolInfo = getFieldOffset("Assembly-CSharp.dll", "Battle", "CoolDownComp",
                                       "m_DicCoolInfo");

        LogicFighter::IsEnemy = setPointer(func_bool2, "Assembly-CSharp.dll", "Battle",
                                           "LogicFighter",
                                           "IsEnemy", 1);

        LogicFighter::GetHPEmpty = setPointer(func_bool, "Assembly-CSharp.dll", "Battle",
                                              "LogicFighter",
                                              "GetHPEmpty", 0);

        LogicFighter::get_Position = setPointer(func_vector3, "Assembly-CSharp.dll", "Battle",
                                                "LogicFighter",
                                                "get_Position", 0);
    }
}

namespace ShowEntity {
    uintptr_t OnUpdate;
    uintptr_t m_RoleName;
    uintptr_t logicFighter;

    Vector3 (*get_DefaultScale)(void *);

    Vector3 (*GetPositionByPartPos)(void *, int es);

    void init() {
        m_RoleName = getFieldOffset("Assembly-CSharp.dll", "", "ShowEntity", "m_RoleName");
        logicFighter = getFieldOffset("Assembly-CSharp.dll", "", "ShowEntity",
                                      "logicFighter");
        OnUpdate = getMethodOffset("Assembly-CSharp.dll", "", "ShowEntity", "OnUpdate", 0);
        get_DefaultScale = setPointer(func_vector3, "Assembly-CSharp.dll", "", "ShowEntity",
                                      "get_DefaultScale", 0);
        ShowEntity::GetPositionByPartPos = setPointer(func_vector32, "Assembly-CSharp.dll", "",
                                                      "ShowEntity", "GetPositionByPartPos", 1);
    }
}

namespace LogicBattleManager {
    void *(*GetPlayerRealSelf)(void *);

    void *getInstance() {
        void *BLogicInstance;
        getFieldStaticVal("Assembly-CSharp.dll", "", "LogicBattleManager", "Instance",
                          &BLogicInstance);
        return BLogicInstance;
    }

    void init() {
        LogicBattleManager::GetPlayerRealSelf = setPointer(func_void, "Assembly-CSharp.dll", "",
                                                           "LogicBattleManager",
                                                           "GetPlayerRealSelf", 0);
    }
}
namespace Camera {

    void *(*get_main)(void *);

    Matrix4x4 (*get_projectionMatrix)(void *);

    Matrix4x4 (*get_worldToCameraMatrix)(void *);

    void init() {
        Camera::get_main = setPointer(func_void, "UnityEngine.CoreModule.dll", "UnityEngine",
                                      "Camera", "get_main", 0);

        Camera::get_projectionMatrix = setPointer(func_Matrix, "UnityEngine.CoreModule.dll",
                                                  "UnityEngine",
                                                  "Camera", "get_projectionMatrix", 0);

        Camera::get_worldToCameraMatrix = setPointer(func_Matrix, "UnityEngine.CoreModule.dll",
                                                     "UnityEngine",
                                                     "Camera", "get_worldToCameraMatrix", 0);

    }
}
#endif //ANDROID_MOD_MENU_GAMECLASS_H
