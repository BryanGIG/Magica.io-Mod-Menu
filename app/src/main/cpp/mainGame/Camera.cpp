
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
        setPointer(Camera_API.get_projectionMatrix, getAssembly(), getNamespace(), getClass(), "get_projectionMatrix", 0);
        setPointer(Camera_API.get_worldToCameraMatrix, getAssembly(), getNamespace(), getClass(), "get_worldToCameraMatrix", 0);
    }

    CCamera *get_main(){
        return (CCamera *)Camera_API.get_main(nullptr);
    }
}