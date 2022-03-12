#pragma once

struct S_Character_API {
    uintptr_t addStatus;
    uintptr_t battleRoyalePoison;
    uintptr_t ApplyDamage;

    bool (*get_Spawned)(void *);

    bool (*get_isMine)(void *);
};

extern S_Character_API Character_API;

class CCharacter {
public:
    bool get_isMine() {
        return Character_API.get_isMine(this);
    }

    bool get_isSpawned() {
        return Character_API.get_Spawned(this);
    }
};

namespace Character {
    const char *getAssembly();

    const char *getNamespace();

    const char *getClass();

    void Initialize();
}