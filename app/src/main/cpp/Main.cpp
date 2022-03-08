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
    env->CallVoidMethod(titleObj, setText, env->NewStringUTF(OBFUSCATE("Mobile Legends Mod")));
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
            OBFUSCATE("Category_Feature List"),
            OBFUSCATE("CheckBox_Draw Player Line"),
            OBFUSCATE("CheckBox_Draw Player Name"),
            OBFUSCATE("CheckBox_Draw Player CoolDown"),
            OBFUSCATE("CheckBox_Draw Player Box"),
            OBFUSCATE("CheckBox_Show Distance"),
            OBFUSCATE("SeekBar_Camera Height_0_20"),
    };
    int Total_Feature = (sizeof(features) / sizeof(features[0]));
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));
    env->CallVoidMethod(ctx, FEATURE, ret);
}


void SetCameraParam(void *ins, Vector3 offsetPt, Quaternion qua) {
    auto droneVal = Configs["Camera Height"];
    Vector3 baruNich = offsetPt;

    if (droneVal > 0) {
        if (offsetPt.X <= 0.0)
            baruNich.X = (float) (offsetPt.X + (float) droneVal * -0.234);
        else
            baruNich.X = (float) (offsetPt.X + (float) droneVal * 0.234);
        baruNich.Y = (float) (offsetPt.Y + (float) droneVal * -0.307);
        if (offsetPt.Z <= 0.0)
            baruNich.Z = (float) (offsetPt.Z + (float) droneVal * -0.235);
        else
            baruNich.Z = (float) (offsetPt.Z + (float) droneVal * 0.235);
    }
    oSetParam(ins, baruNich, qua);
}

bool WorldToScreen(Matrix4x4 _viewMatrix, Vector3 enemyPos, Vector2 &out) {
    auto viewMatrix = _viewMatrix.MultiplyPoint(enemyPos);
    if (viewMatrix != Vector3::Zero()) {
        auto screenPos = Vector3(
                viewMatrix.X + 1.0f,
                viewMatrix.Y + 1.0f,
                viewMatrix.Z + 1.0f) / 2.0f;
        out = Vector2(screenPos.X * screen.X, screen.Y - (screenPos.Y * screen.Y));
        return true;
    }
    return false;
}

int parseCamp() {
    int res;
    getFieldStaticVal("Assembly-CSharp.dll", "", "LogicBattleManager", "m_SelfCampType", &res);
    return res;
}

void *GameManager;

std::vector<int> getCoolDown(void *logic) {
    std::vector<int> hasilAkhir = {-1, -1, -1};

    auto skillComp = *(void **) ((uintptr_t) logic + LogicFighter::m_SkillComp);
    if (skillComp == NULL) return hasilAkhir;

    auto cdComp = *(void **) ((uintptr_t) skillComp + LogicFighter::m_CoolDownComp);
    if (cdComp == NULL) return hasilAkhir;

    auto cdDict = *(Dictionary<int, void **> **) ((uintptr_t) cdComp +
                                                  LogicFighter::m_DicCoolInfo);
    if (cdDict == NULL) return hasilAkhir;
    for (int i = 0; i < cdDict->getSize(); ++i) {
        auto cdData = cdDict->getValues()[i];
        if (cdData == NULL) continue;
        auto spelID = *(int *) ((uintptr_t) cdData + CoolDownData::iSpellID);
        if (spelID < 20000) {
            auto spelID2 = spelID % 100;
            if (spelID2 == 10) {
                hasilAkhir[0] = CoolDownData::GetCoolTime(cdData);
            }
            if (spelID2 == 20) {
                hasilAkhir[1] = CoolDownData::GetCoolTime(cdData);
            }
            if (spelID2 == 30) {
                hasilAkhir[2] = CoolDownData::GetCoolTime(cdData);
            }
            if (spelID2 == 40) {
                // Why push back? because skill 4 isn't always available all hero
                hasilAkhir.push_back(CoolDownData::GetCoolTime(cdData));
            }
        }
    }
    return hasilAkhir;
}

void doLoopPlayers() {
    if (GameManager == NULL) {
        Il2Cpp::GetMethodOffset("Assembly-CSharp.dll", "", "BattleManager", "Instance");
        getFieldStaticVal("Assembly-CSharp.dll", "", "BattleManager", "Instance", &GameManager);
        return;
    }
    auto photonplayers = *(List<void **> **) ((uintptr_t) GameManager +
                                              BattleManager::m_ShowPlayers);
    if (photonplayers == NULL) {
        GameManager = NULL;
        return;
    }
    auto main_cam = Camera::get_main(NULL);
    if (main_cam == NULL) return;

    resp.PlayerCount = photonplayers->getSize();

    for (int u = 0; u < resp.PlayerCount; u++) {
        resp.Players[u].isLegit = false;
        auto showEntity = photonplayers->getItems()[u];
        if (showEntity == NULL) continue;

        auto logic = *(void **) ((uintptr_t) showEntity + ShowEntity::logicFighter);
        if (logic == NULL) continue;
        auto localLogic = LogicBattleManager::GetPlayerRealSelf(LogicBattleManager::getInstance());
        if (localLogic == NULL)continue;
        if (!LogicFighter::IsEnemy(logic, parseCamp())) continue;
        if (LogicFighter::GetHPEmpty(logic)) continue;
        auto enmPos = LogicFighter::get_Position(logic);
        auto enmName = *(String **) ((uintptr_t) showEntity + ShowEntity::m_RoleName);

        Vector2 playerLoc;
        auto vMatrix =
                Camera::get_projectionMatrix(main_cam) * Camera::get_worldToCameraMatrix(main_cam);
        if (!WorldToScreen(vMatrix, enmPos, playerLoc)) continue;
        resp.Players[u].Location = playerLoc;
        resp.Players[u].playerName = Il2Cpp::GetString(enmName);
        resp.Players[u].CoolDown = getCoolDown(logic);
        resp.Players[u].isLegit = true;
    }
}

void OnUpdate(void *instance) {
    oOnUpdate(instance);
    doLoopPlayers();

    if (changeDrone) {
        ShowBattleCamera::ResetCameraParm(NULL);
        changeDrone = false;
    }
}

void *gameload(void *) {
    do {
        sleep(10);
    } while (!isLibraryLoaded(targetLibName));

    Il2Cpp::Init(); //init the il2cpp base

    SmoothFollow::init();
    BattleManager::init();
    SystemData::init();
    ShowEntity::init();
    LogicFighter::init();
    LogicBattleManager::init();
    ShowBattleCamera::init();
    CoolDownData::init();
    Camera::init();
    CData_Hero::init();

    TOOLS::HookFunc((void *) ShowEntity::OnUpdate, (void *) OnUpdate, (void **) &oOnUpdate);
    TOOLS::HookFunc((void *) SmoothFollow::SetCameraParam, (void *) SetCameraParam,
                    (void **) &oSetParam);
    return NULL;
}

template<typename Container>
auto its_and_idx(Container &&container) {
    using std::begin, std::end;
    return std::tuple{begin(container), end(container), 0};
}

extern "C"
JNIEXPORT void JNICALL
Java_com_polarmods_ESPView_DrawOn(JNIEnv *env, jobject espView, jobject canvas) {
    ESP espOverlay = ESP(env, espView, canvas);
    if (espOverlay.isValid()) {
        screen = Vector2(espOverlay.getWidth(), espOverlay.getHeight());
        auto From = screen / 2;
        auto sizeBox = 120;

        for (int u = 0; u < resp.PlayerCount; u++) {
            if (!resp.Players[u].isLegit) continue;

            auto enemyPos = resp.Players[u].Location;
            auto droneVal = Configs["Camera Height"];
            Rect PlayerRect(enemyPos.X - ((float) sizeBox / 2), enemyPos.Y - sizeBox, sizeBox,
                            sizeBox + 13.0f);

            if (Configs["Draw Player Line"]) {
                espOverlay.DrawLine(Colors::yellow(), 3, From, enemyPos);
            }
            if (Configs["Draw Player Box"]) {
                espOverlay.DrawBox(Colors::white(), 2, PlayerRect);
            }
            if (Configs["Draw Player Name"]) {
                auto heroName = resp.Players[u].playerName;
                string dist = "[ " + heroName + " ]";

                espOverlay.DrawText(Colors::black(), dist.c_str(),
                                    Vector2(PlayerRect.m_XMin + (PlayerRect.m_Width / 2),
                                            PlayerRect.m_YMin + PlayerRect.m_Height + 16),
                                    droneVal + 15.0f);
            }
            if (Configs["Show Distance"]) {

            }
            if (Configs["Draw Player CoolDown"]) {
                auto heroCD = resp.Players[u].CoolDown;
                ostringstream ost;
                for (auto[it, end, i] = its_and_idx(heroCD); it != end; ++it, ++i) {
                    auto cooldownTime = (*it / 1000) % 60;
                    if (*it == -1)
                        ost << "Skill " << i + 1 << " : Locked | ";
                    else if (cooldownTime > 0)
                        ost << "Skill " << i + 1 << " : " << cooldownTime << " | ";
                    else
                        ost << "Skill " << i + 1 << " : Ready" << " | ";
                }

                espOverlay.DrawText(Colors::white(), ost.str().c_str(),
                                    Vector2(PlayerRect.m_XMin,
                                            PlayerRect.m_YMin + PlayerRect.m_Height + 16),
                                    droneVal + 15.0f);
            }
        }
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_polarmods_BaseView_Changes(JNIEnv *env, jobject, jstring feature, jint value) {
    auto features = env->GetStringUTFChars(feature, NULL);
    Configs[(const char *) features] = (u_long) value;
    env->ReleaseStringUTFChars(feature, features);
}

[[maybe_unused]]
__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, gameload, NULL);
}