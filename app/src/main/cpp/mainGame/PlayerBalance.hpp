struct S_PlayerBalance_API {
    uintptr_t get_VipActive;
    uintptr_t get_NoAds;
    uintptr_t get_NoAds2;
};

extern S_PlayerBalance_API PlayerBalance_API;

class CPlayerBalance {
public:
};

namespace PlayerBalance {
    const char *getAssembly();

    const char *getNamespace();

    const char *getClass();

    void Initialize();
}
