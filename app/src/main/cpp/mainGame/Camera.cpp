
#include "Includes.hpp"

S_Camera_API Camera_API;

namespace Camera {
    const char *getAssembly(){
        return "UnityEngine.CoreModule.dll";
    }

    const char *getNamespace(){
        return "UnityEngine";
    }

    const char *getClass(){
        return "Camera";
    }

    void Initialize(){
        setPointer(Camera_API.get_main, getAssembly(), getNamespace(), getClass(), "get_main", 0);
        setPointer(Camera_API.WorldToScreenPoint_Injected, getAssembly(), getNamespace(), getClass(), "WorldToScreenPoint_Injected", 3);
    }

    CCamera *get_main(){
        return (CCamera *)Camera_API.get_main(nullptr);
    }
}