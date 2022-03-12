struct S_Template_API {

};
extern S_Template_API Template_API;

class CTemplate {
public:
};

namespace Template {
    const char *getAssembly();
    const char *getNamespace();
    const char *getClass();
    void Initialize();
}
