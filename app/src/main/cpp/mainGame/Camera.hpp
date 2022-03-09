#pragma once

struct S_Camera_API{
    void *(*get_main)(void *);
    Matrix4x4 (*get_projectionMatrix)(void *);
    Matrix4x4 (*get_worldToCameraMatrix)(void *);
};

extern S_Camera_API Camera_API;

class CCamera {
public:
    Matrix4x4 getProjectionMatrix(){
        return Camera_API.get_projectionMatrix(this);
    }
    Matrix4x4 getWorldToCameraMatrix() {
        return Camera_API.get_worldToCameraMatrix(this);
    }
};

namespace Camera{
    const char *getAssembly();

    const char *getNamespace();

    const char *getClass();

    void Initialize();

    CCamera *get_main();
}