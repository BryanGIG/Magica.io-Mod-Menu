#include <jni.h>
#include <pthread.h>
#include "KittyMemory/MemoryPatch.h"
#include "utils.h"
#include "Vector3.hpp"
#include <android/log.h>

void *mainThread(void *) {
    ProcMap il2cppMap;
    do {
        il2cppMap = KittyMemory::getLibraryMap("libil2cpp.so");
        sleep(1);
    } while(!il2cppMap.isValid());
    sleep(1);


    return nullptr;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_polarmods_NativeFunctions_init(JNIEnv *, jobject ) {
    pthread_t ptid;
    pthread_create(&ptid, nullptr, mainThread, nullptr);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_polarmods_NativeFunctions_changeSwitch(JNIEnv *, jobject , jint id) {
    int i = reinterpret_cast<int>(id);
    if (i == 0) {

    }
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_polarmods_NativeFunctions_getFeatures(JNIEnv *env, jobject ) {
    const char *features[] = {"Switch_AntiCheat Bypass"};
    int totalFeatures = (sizeof features / sizeof features[0]);
    jobjectArray stringArray = env->NewObjectArray(totalFeatures,env->FindClass("java/lang/String"), env->NewStringUTF(""));

    for (int i = 0; i < totalFeatures; i++) {
        env->SetObjectArrayElement(stringArray, i, env->NewStringUTF(features[i]));
    }
    return stringArray;
}