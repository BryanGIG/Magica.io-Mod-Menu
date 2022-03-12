#include "Includes.hpp"

S_PlayerBalance_API PlayerBalance_API;


namespace PlayerBalance {
    const char *getAssembly() {
        return OBFUSCATE("Assembly-CSharp.dll");
    }

    const char *getNamespace() {
        return OBFUSCATE("ThrowIO");
    }

    const char *getClass() {
        return OBFUSCATE("PlayerBalance");
    }

    void Initialize() {
        PlayerBalance_API.get_VipActive = getMethodOffset(getAssembly(), getNamespace(), getClass(), "get_VipActive", 0);
        PlayerBalance_API.get_NoAds = getMethodOffset(getAssembly(), getNamespace(), getClass(), "get_NoAds", 0);
        PlayerBalance_API.get_NoAds2 = getMethodOffset(getAssembly(), getNamespace(), getClass(), "get_NoAds2", 0);
    }
}