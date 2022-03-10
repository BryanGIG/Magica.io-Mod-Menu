#include "Main.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_polarmods_FloatingMod_Init(JNIEnv *env, jobject, jobject ctx) {
    auto clax = env->GetObjectClass(ctx);
    auto clazz = env->FindClass("android/widget/TextView");
    auto setText = env->GetMethodID(clazz, "setText", "(Ljava/lang/CharSequence;)V");
    auto TITLE = env->GetFieldID(clax, "title", "Landroid/widget/TextView;");
    auto HEADER = env->GetFieldID(clax, "heading", "Landroid/widget/TextView;");
    auto ICON = env->GetMethodID(clax, "changeIcon", "(Ljava/lang/String;)V");
    auto FEATURE = env->GetMethodID(clax, "addFeature", "([Ljava/lang/String;)V");
    auto toHtml = env->GetMethodID(clax, "toHTML", "(Ljava/lang/String;)Ljava/lang/CharSequence;");
    auto titleObj = env->GetObjectField(ctx, TITLE);
    auto headerObj = env->GetObjectField(ctx, HEADER);
    env->CallVoidMethod(titleObj, setText, env->NewStringUTF(OBFUSCATE("Magica.io Mod")));
    auto HTML = env->CallObjectMethod(ctx, toHtml, env->NewStringUTF(OBFUSCATE(
                                                                             "<b><marquee><p style=\"font-size:30\">"
                                                                             "<p style=\"color:green;\">Modded by BryanGIG</p> | "
                                                                             "<p style=\"color:cyan;\">www.polarmods.com</p> | "
                                                                             "<p style=\"color:green;\">Report Any Bugs There</p>"
                                                                             "</marquee></b>")));
    env->CallVoidMethod(headerObj, setText, HTML);
    env->CallVoidMethod(ctx, ICON, env->NewStringUTF(LogoPolarMods));
    // Note:
    // Do not change or translate the first text unless you know what you are doing
    // Assigning feature numbers is optional. Without it, it will automatically count for you, starting from 0
    // Assigned feature numbers can be like any numbers 1,3,200,10... instead in order 0,1,2,3,4,5...
    // ButtonLink, Category, RichTextView and RichWebView is not counted. They can't have feature number assigned
    // Toggle, ButtonOnOff and Checkbox can be switched on by default, if you add True_. Example: CheckBox_True_The Check Box
    // To learn HTML, go to this page: https://www.w3schools.com/

    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("Category_>>> www.polarmods.com <<<"),
            OBFUSCATE("Category_GamePlay Features"),
            OBFUSCATE("CheckBox_Anti Dirted"),
            OBFUSCATE("CheckBox_Anti Frozen"),
            OBFUSCATE("CheckBox_Anti Stunned"),
            OBFUSCATE("InputValue_Set Crystal Amount_0_500"),
            OBFUSCATE("InputText_Set Player Name"),
    };
    int Total_Feature = (sizeof(features) / sizeof(features[0]));
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));
    env->CallVoidMethod(ctx, FEATURE, ret);
}

HOOK_DEF(void, SendVideoAdsEvent, AdsPlacement placement, AdsResult result, void *str_object_id){
    orig_SendVideoAdsEvent(placement, watched, str_object_id);
}

HOOK_DEF(void, updateDelta, CCharacter *instance, float deltaTime) {
    orig_updateDelta(instance, deltaTime);
    if (instance->get_isLocalPlayer() && instance->get_isSpawned()) {
        try {
            bool isDirted = std::any_cast<bool>(Configs["Anti Dirted"]);
            bool isFrozen = std::any_cast<bool>(Configs["Anti Frozen"]);
            bool isStunned = std::any_cast<bool>(Configs["Anti Stunned"]);

            if (isDirted && instance->get_isDirted()) {
                LOGI("Lmao you got dirted");
                instance->set_isDirted(false);
            }
            if (isFrozen && instance->get_isFrosted()) {
                LOGI("Lmao you got frozen");
                instance->set_isFrosted(false);
            }
            if (isStunned && instance->get_isStuned()) {
                LOGI("Lmao you got stunned");
                instance->set_isStuned(false);
            }
        } catch (const std::bad_any_cast &e) {}
    }
}

void *gameload(void *) {
    const char *libName = OBFUSCATE("libil2cpp.so");
    do {
        sleep(10);
    } while (!isLibraryLoaded(libName));

    Il2Cpp::Init(libName);
    Camera::Initialize();
    GameCore::Initialize();
    Character::Initialize();
    Analytics::Initialize();

    Tools::HookFunc((void *) Character_API.updateDelta, (void *) new_updateDelta,(void **) &orig_updateDelta);
    Tools::HookFunc((void *) Analytics_API.SendVideoAdsEvent, (void *) new_SendVideoAdsEvent,(void **) &orig_SendVideoAdsEvent);
    return nullptr;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_polarmods_ESPView_DrawOn(JNIEnv *env, jobject espView, jobject canvas) {
    ESP espOverlay = ESP(env, espView, canvas);
    if (espOverlay.isValid()) {
        screen = Vector2(espOverlay.getWidth(), espOverlay.getHeight());

        try {
            for (int u = 0; u < resp.PlayerCount; u++) {
                if (!resp.Players[u].isLegit) continue;

                auto enemyPos = resp.Players[u].Location;

                auto drawLine = std::any_cast<bool>(Configs["Draw Player Line"]);
                if (drawLine) {
                    espOverlay.DrawLine(Colors::yellow(), 3, screen / 2, enemyPos);
                }
            }
        } catch (const std::bad_any_cast &e) {}
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_polarmods_BaseView_Changes(JNIEnv *env, jobject, jstring feature, jint value,
                                    jstring value2) {
    const char *features = env->GetStringUTFChars(feature, nullptr);
    if (value2 != nullptr) {
        const char *values2 = env->GetStringUTFChars(value2, nullptr);
        Configs[features] = values2;
        env->ReleaseStringUTFChars(value2, values2);
    } else {
        Configs[features] = value;
    }
    env->ReleaseStringUTFChars(feature, features);
}

[[maybe_unused]]
__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, nullptr, gameload, nullptr);
}