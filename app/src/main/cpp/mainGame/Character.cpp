#include "Includes.hpp"

S_Character_API Character_API;

namespace Character {
    const char *getAssembly() {
        return OBFUSCATE("Assembly-CSharp.dll");
    }

    const char *getNamespace() {
        return OBFUSCATE("ThrowIO");
    }

    const char *getClass() {
        return OBFUSCATE("Character");
    }

    void Initialize() {
        Character_API.addStatus = getMethodOffset(getAssembly(), getNamespace(), getClass(), "addStatus", 1);
        Character_API.battleRoyalePoison = getMethodOffset(getAssembly(), getNamespace(), getClass(), "battleRoyalePoison", 0);
        Character_API.ApplyDamage = getMethodOffset(getAssembly(), getNamespace(), getClass(), "ApplyDamage", 5);
        setPointer(Character_API.get_Spawned, getAssembly(), getNamespace(), getClass(), "get_Spawned", 0);
        setPointer(Character_API.get_isMine, getAssembly(), getNamespace(), getClass(), "get_isMine", 0);
    }
}