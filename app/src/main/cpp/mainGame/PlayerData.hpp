struct S_PlayerData_API {
    uintptr_t HasSkin;
    uintptr_t HasAllSkins;
    uintptr_t HasWeapon;
    uintptr_t CallOnChanged;
    uintptr_t SpendMoney;
    uintptr_t SpendMoney2;
    uintptr_t CheckNotAvailableSkins;
    void (*GiveAllWeapons)(void *);
};

extern S_PlayerData_API PlayerData_API;

namespace PlayerData {
    const char *getAssembly();

    const char *getNamespace();

    const char *getClass();

    void Initialize();
}