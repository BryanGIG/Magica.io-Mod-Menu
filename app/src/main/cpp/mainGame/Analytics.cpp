
#include "Includes.hpp"

S_Analytics_API Analytics_API;

namespace Analytics {
    const char *getAssembly(){
        return OBFUSCATE("Assembly-CSharp.dll");
    }
    const char *getNamespace(){
        return OBFUSCATE("ThrowIO");
    }

    const char *getClass() {
        return OBFUSCATE("Analytics");
    }

    void Initialize(){
        Analytics_API.SendVideoAdsEvent = getMethodOffset(getAssembly(), getNamespace(), getClass(), "SendVideoAdsEvent", 3);
    }

}