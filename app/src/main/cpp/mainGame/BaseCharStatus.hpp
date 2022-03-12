struct S_BaseCharStatus_API {
    uintptr_t dirt_Build;
    uintptr_t stun_Build;
    uintptr_t poison_Build;
    uintptr_t frost_Build;
};

extern S_BaseCharStatus_API BaseCharStatus_API;

namespace BaseCharStatus {
    const char *getAssembly();

    const char *getNamespace();

    void Initialize();
}
