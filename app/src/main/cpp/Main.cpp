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
    auto FEATURE = env->GetMethodID(clax, "featureList", "([Ljava/lang/String;)V");
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
            OBFUSCATE("CheckBox_Anti Frosted"),
            OBFUSCATE("CheckBox_Anti Stunned"),
            OBFUSCATE("CheckBox_Anti Critical Damage"),
            OBFUSCATE("CheckBox_Anti Poison Damage"),
            OBFUSCATE("CheckBox_Anti Zone Damage"),
            OBFUSCATE("Category_Other Features"),
//            OBFUSCATE("CheckBox_True_No Ads"),
            OBFUSCATE("CheckBox_True_Always VIP"),
            OBFUSCATE("CheckBox_True_Unlimited Buy Item"),
    };
    int Total_Feature = (sizeof(features) / sizeof(features[0]));
    ret = env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                              env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));
    env->CallVoidMethod(ctx, FEATURE, ret);
}

HOOK_DEF(bool, get_VipActive, void *ins) {
    try {
        bool isVIP = std::any_cast<int>(Configs["Always VIP"]);
        if (isVIP)
            return true;
    } catch (const std::bad_any_cast &e) {}
    return orig_get_VipActive(ins);
}

HOOK_DEF(void, battleRoyalePoison, void *instance) {
    try {
        bool isZoneDamage = std::any_cast<int>(Configs["Anti Zone Damage"]);
        if (isZoneDamage) {
            return;
        }
    } catch (const std::bad_any_cast &e) {}
    orig_battleRoyalePoison(instance);
}

HOOK_DEF(bool, SpendMoney, void *ins, void *type, long number, void *s, void *item) {
    try {
        bool isUnlimitedBuyItem = std::any_cast<int>(Configs["Unlimited Buy Item"]);
        if (isUnlimitedBuyItem) {
            return orig_SpendMoney(ins, type, 0, s, item);;
        }
    } catch (const std::bad_any_cast &e) {}
    return orig_SpendMoney(ins, type, number, s, item);
}

HOOK_DEF(void, SpendMoney2, void *ins, void *price, void *s, void *item) {
    try {
        bool isUnlimitedBuyItem = std::any_cast<int>(Configs["Unlimited Buy Item"]);
        if (isUnlimitedBuyItem) {
            Il2Cpp::SetField(price, 0x14, 0);
        }
    } catch (const std::bad_any_cast &e) {}

    return orig_SpendMoney2(ins, price, s, item);
}

HOOK_DEF(bool, ApplyDamage, CCharacter *self, long damage, CCharacter *from, bool isCritical,
         bool poisonAttack, int damageSource) {
    if (self->get_isMine()) {
        try {
            bool isCriticalDamage = std::any_cast<int>(Configs["Anti Critical Damage"]);
            bool isPoisonDamage = std::any_cast<int>(Configs["Anti Poison Damage"]);

            return orig_ApplyDamage(self,
                                    damage, from,
                                    !isCriticalDamage && isCritical,
                                    !isPoisonDamage && poisonAttack, damageSource);

        } catch (const std::bad_any_cast &e) {}
    }
    return orig_ApplyDamage(self, damage, from, isCritical, poisonAttack, damageSource);
}

HOOK_DEF(void *, dirt_Build, void *ins, CCharacter *owner, float timer, float period) {
    if (owner->get_isMine()) {
        try {
            bool isAntiDirted = std::any_cast<int>(Configs["Anti Dirted"]);
            if (isAntiDirted) {
                return orig_dirt_Build(ins, owner, 0.1, 0.2);
            }
        } catch (const std::bad_any_cast &e) {}
    }
    return orig_dirt_Build(ins, owner, timer, period);
}

HOOK_DEF(void *, stun_Build, void *ins, CCharacter *owner, float timer, float period) {
    if (owner->get_isMine()) {
        try {
            bool isAntiStunned = std::any_cast<int>(Configs["Anti Stunned"]);
            if (isAntiStunned) {
                return orig_stun_Build(ins, owner, 0.1, 0.2);
            }
        } catch (const std::bad_any_cast &e) {}
    }
    return orig_stun_Build(ins, owner, timer, period);
}

HOOK_DEF(void *, frost_Build, void *ins, CCharacter *owner, float timer, float period) {
    if (owner->get_isMine()) {
        try {
            bool isAntiFrosted = std::any_cast<int>(Configs["Anti Frosted"]);
            if (isAntiFrosted) {
                return orig_frost_Build(ins, owner, 0.1, 0.2);
            }
        } catch (const std::bad_any_cast &e) {}
    }
    return orig_frost_Build(ins, owner, timer, period);
}

HOOK_DEF(void *, poison_Build, void *ins, CCharacter *owner, float timer, float period, int damage,
         CCharacter *attacker) {
    if (owner->get_isMine()) {
        try {
            bool isAntiPoisoned = std::any_cast<int>(Configs["Anti Poisoned"]);
            if (isAntiPoisoned) {
                return orig_poison_Build(ins, owner, 0.1, 0.2, 1, attacker);
            }
        } catch (const std::bad_any_cast &e) {}
    }
    return orig_poison_Build(ins, owner, timer, period, damage, attacker);
}

void *gameload(void *) {
    const char *libName = OBFUSCATE("libil2cpp.so");
    do {
        sleep(5);
    } while (!isLibraryLoaded(libName));
    sleep(5);

    Il2Cpp::Init(libName);
    Camera::Initialize();
    GameCore::Initialize();
    Character::Initialize();
    PlayerData::Initialize();
    PlayerBalance::Initialize();
    BaseCharStatus::Initialize();

    Hook(Character_API.battleRoyalePoison, battleRoyalePoison);
    Hook(Character_API.ApplyDamage, ApplyDamage);

    Hook(PlayerBalance_API.get_VipActive, get_VipActive);

    Hook(BaseCharStatus_API.dirt_Build, dirt_Build);
    Hook(BaseCharStatus_API.stun_Build, stun_Build);
    Hook(BaseCharStatus_API.frost_Build, frost_Build);
    Hook(BaseCharStatus_API.poison_Build, poison_Build);

    Hook(PlayerData_API.SpendMoney, SpendMoney);
    Hook(PlayerData_API.SpendMoney2, SpendMoney2);
    return nullptr;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_polarmods_ESPView_DrawOn(JNIEnv *env, jobject espView, jobject canvas) {
    ESP espOverlay = ESP(env, espView, canvas);
    if (espOverlay.isValid()) {
        screen = Vector2(espOverlay.getWidth(), espOverlay.getHeight());

//        try {
//            for (int u = 0; u < resp.PlayerCount; u++) {
//                if (!resp.Players[u].isLegit) continue;
//
//                auto enemyPos = resp.Players[u].Location;
//
//                auto drawLine = std::any_cast<bool>(Configs["Draw Player Line"]);
//                if (drawLine) {
//                    espOverlay.DrawLine(Colors::yellow(), 3, screen / 2, enemyPos);
//                }
//            }
//        } catch (const std::bad_any_cast &e) {}
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