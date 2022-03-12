
#include "Includes.hpp"
#include <Utils.h>

S_PlayerData_API PlayerData_API;

namespace PlayerData {
    const char *getAssembly(){
        return OBFUSCATE("Assembly-CSharp.dll");
    }
    const char *getNamespace(){
        return OBFUSCATE("ThrowIO");
    }

    const char *getClass() {
        return OBFUSCATE("PlayerData");
    }

    void Initialize(){
        PlayerData_API.HasSkin = getMethodOffset(getAssembly(), getNamespace(), getClass(), "HasSkin", 1);
        PlayerData_API.HasAllSkins = getMethodOffset(getAssembly(), getNamespace(), getClass(), "HasAllSkins", 0);
        PlayerData_API.HasWeapon = getMethodOffset(getAssembly(), getNamespace(), getClass(), "HasWeapon", 1);
        PlayerData_API.CallOnChanged = getMethodOffset(getAssembly(), getNamespace(), getClass(), "CallOnChanged", 1);
        PlayerData_API.SpendMoney = getMethodOffset(getAssembly(), getNamespace(), getClass(), "SpendMoney", 4);
        PlayerData_API.SpendMoney2 = getMethodOffset(getAssembly(), getNamespace(), getClass(), "SpendMoney", 3);
        PlayerData_API.CheckNotAvailableSkins = getMethodOffset(getAssembly(), getNamespace(), getClass(), "CheckNotAvailableSkins", 0);
    }
}