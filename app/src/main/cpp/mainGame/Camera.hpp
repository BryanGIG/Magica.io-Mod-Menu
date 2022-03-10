#pragma once

enum MonoOrStereoscopicEye {
    Left,
    Right,
    Mono
};

struct S_Camera_API {
    void *(*get_main)(void *);

    void (*WorldToScreenPoint_Injected)(void *,Vector3 position, MonoOrStereoscopicEye, Vector3 *screenPosition);
};

extern S_Camera_API Camera_API;

class CCamera {
public:
    Vector3 WorldToScreenPoint(Vector3 pos) {
        Vector3 screenPos;
        Camera_API.WorldToScreenPoint_Injected(this, pos, Mono, &screenPos);
        return screenPos;
    }
};
namespace Camera {
    const char *getAssembly();

    const char *getNamespace();

    const char *getClass();

    void Initialize();

    CCamera *get_main();
}