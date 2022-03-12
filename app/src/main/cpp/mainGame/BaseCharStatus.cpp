#include "Includes.hpp"

S_BaseCharStatus_API BaseCharStatus_API;


namespace BaseCharStatus {
    const char *getAssembly() {
        return OBFUSCATE("Assembly-CSharp.dll");
    }

    const char *getNamespace() {
        return OBFUSCATE("ThrowIO");
    }

    void Initialize() {
        setPointer(BaseCharStatus_API.dirt_Build, getAssembly(), getNamespace(), OBFUSCATE("CharStatusDirt"), "build", 3);
        setPointer(BaseCharStatus_API.frost_Build, getAssembly(), getNamespace(), OBFUSCATE("CharStatusFrost"), "build", 3);
        setPointer(BaseCharStatus_API.stun_Build, getAssembly(), getNamespace(), OBFUSCATE("CharStatusStun"), "build", 3);
        setPointer(BaseCharStatus_API.poison_Build, getAssembly(), getNamespace(), OBFUSCATE("CharStatusPoison"), "build", 5);
    }
}