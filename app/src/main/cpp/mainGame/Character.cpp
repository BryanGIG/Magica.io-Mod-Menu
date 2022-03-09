#include "Includes.hpp"

S_Character_API Character_API;

namespace Character {
    const char *getAssembly(){
        return OBFUSCATE("Assembly-CSharp.dll");
    }

    const char *getNamespace(){
        return OBFUSCATE("ThrowIO");
    }

    const char *getClass(){
        return OBFUSCATE("Character");
    }

    void Initialize(){
        //Field
        Character_API._isDirted = getFieldOffset(getAssembly(), getNamespace(), getClass(), "_isDirted");
        Character_API._isFrosted = getFieldOffset(getAssembly(), getNamespace(), getClass(), "_isFrosted");
        Character_API._isStuned = getFieldOffset(getAssembly(), getNamespace(), getClass(), "_isStuned");
        Character_API._isSpawned = getFieldOffset(getAssembly(), getNamespace(), getClass(), "<Spawned>k__BackingField");

        //Methods
        setPointer(Character_API.get_isLocalPlayer, getAssembly(), getNamespace(), getClass(), "get_isLocalPlayer", 0);
        Character_API.updateDelta = getMethodOffset(getAssembly(), getNamespace(), getClass(),  "updateDelta", 1);
    }
}