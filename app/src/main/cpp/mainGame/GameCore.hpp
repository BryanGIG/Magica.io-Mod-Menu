#pragma once

//struct S_GameCore_API {
//    uintptr_t m_ShowPlayers;
//};
//extern S_GameCore_API GameCore_API;
class CCharacter;
//class CGameCore {
//public:
//};

namespace GameCore {
    const char *getAssembly();

    const char *getNamespace();

    const char *getClass();

    void Initialize();

    CCharacter *getMe();

    monoList<CCharacter *> *getListPlayer();
}