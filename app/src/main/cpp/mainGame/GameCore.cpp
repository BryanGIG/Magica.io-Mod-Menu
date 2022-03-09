
#include "Includes.hpp"

//S_GameCore_API GameCore_API;

namespace GameCore {
    const char *getAssembly() {
        return "Assembly-CSharp.dll";
    }

    const char *getNamespace() {
        return "ThrowIO";
    }

    const char *getClass() {
        return "GameCore";
    }

    void Initialize(){
//        GameCore_API.m_ShowPlayers = getFieldOffset(getAssembly(), getNamespace(), getClass(), "m_ShowPlayers");
    }


    CCharacter *getMe() {
        CCharacter *me = nullptr;
        getFieldStaticVal(getAssembly(), getNamespace(), getClass(), "Me", &me);
        return me;
    }

    monoList<CCharacter *> *getListPlayer() {
        monoList<CCharacter *> *players = nullptr;
        getFieldStaticVal(getAssembly(), getNamespace(), getClass(), "PlayersInRoom", &players);
        return players;
    }

}