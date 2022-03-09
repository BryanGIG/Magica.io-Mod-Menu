//
// Created by aimar on 12/28/2019.
//
#include <stdio.h>
#include <android/log.h>
#include <dlfcn.h>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

#ifndef API_H
#define API_H


namespace Il2Cpp {

    int Init(const char *lib);

    int Init(void *handle);

    void *GetImageByName(const char *image);

    void *GetClassType(const char *image, const char *namespaze, const char *clazz);

    void *CreateArray(const char *image, const char *namespaze, const char *clazz, size_t length);

    void *GetMethodOffset(const char *image, const char *namespaze, const char *clazz, const char *name, int argsCount = 0);
    void *GetMethodOffset(const char *image, const char *namespaze, const char *clazz, const char *name, char **args, int argsCount = 0);
    const char *GetMethodName(void* method);

    size_t GetFieldOffset(const char *image, const char *namespaze, const char *clazz, const char *name);

    void GetStaticFieldValue(const char *image, const char *namespaze, const char *clazz, const char *name, void *output);
    void SetStaticFieldValue(const char *image, const char *namespaze, const char *clazz, const char *name, void* value);

    void *CreateClassInstance(const char *image, const char *namespaze, const char *clazz);

    bool IsAssembliesLoaded();

    void *CreateString(const char *str);

    template <typename T>
    static inline T GetMethod(uint64_t realOffset) {
        return (T)((uint8_t*)realOffset);
    }

    template <typename T>
    static inline T GetMethod(const char *image, const char *namespaze, const char *clazz, const char *name, int argsCount) {
        auto realOffset = Il2Cpp::GetMethodOffset(image,namespaze,clazz,name,argsCount);
        return (T)((uint8_t*)realOffset);
    }

    template <typename T>
    static inline T GetField(void* obj, std::ptrdiff_t Offset) {
        return *(T*)((std::uint8_t*)obj + Offset);
    }

    template <typename T>
    static inline void SetField(void* obj, std::ptrdiff_t Offset, T val)
    {
        *(T*)((std::uint8_t*)obj + Offset) = val;
    }

    string GetString(void *string);

    uintptr_t GetFieldOffset(void *clazz, const char *name);
};


#endif //API_H
