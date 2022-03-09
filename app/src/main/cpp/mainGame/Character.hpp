#pragma once

struct S_Character_API {
    uintptr_t _isDirted;
    uintptr_t _isFrosted;
    uintptr_t _isStuned;
    uintptr_t _isSpawned;
    uintptr_t updateDelta;
    bool (*get_isLocalPlayer)(void *);
};

extern S_Character_API Character_API;

class CCharacter {
public:
    bool get_isLocalPlayer() {
        return Character_API.get_isLocalPlayer(this);
    }

    bool get_isDirted() {
        return Il2Cpp::GetField<bool>(this, Character_API._isDirted);
    }

    void set_isDirted(bool value) {
         Il2Cpp::SetField(this, Character_API._isDirted, value);
    }

    bool get_isFrosted() {
        return Il2Cpp::GetField<bool>(this, Character_API._isFrosted);
    }

    void set_isFrosted(bool value) {
        Il2Cpp::SetField(this, Character_API._isFrosted, value);
    }

    bool get_isStuned() {
        return Il2Cpp::GetField<bool>(this, Character_API._isStuned);
    }

    void set_isStuned(bool value) {
        Il2Cpp::SetField(this, Character_API._isStuned, value);
    }

    bool get_isSpawned() {
        return Il2Cpp::GetField<bool>(this, Character_API._isSpawned);
    }

    void set_isSpawned(bool value) {
        Il2Cpp::SetField(this, Character_API._isSpawned, value);
    }

};

namespace Character {
    const char *getAssembly();

    const char *getNamespace();

    const char *getClass();

    void Initialize();
}