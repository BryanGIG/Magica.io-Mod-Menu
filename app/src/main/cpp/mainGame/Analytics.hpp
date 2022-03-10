enum AdsPlacement {
    periodic,
    rewarded
};
enum AdsResult {
    watched,
    canceled
};
struct S_Analytics_API {
    uintptr_t SendVideoAdsEvent;
};

extern S_Analytics_API Analytics_API;

namespace Analytics {
    const char *getAssembly();

    const char *getNamespace();

    const char *getClass();

    void Initialize();
}