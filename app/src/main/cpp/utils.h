
#if defined(__aarch64__)
#include "And64InlineHook//And64InlineHook.hpp"
#else
#include "Substrate/CydiaSubstrate.h"
#endif

void universalHook(void *orig_fcn, void* new_fcn, void **orig_fcn_ptr) {
#if defined(__aarch64__)
    A64HookFunction(orig_fcn, new_fcn, orig_fcn_ptr);
#else
    MSHookFunction(orig_fcn, new_fcn, orig_fcn_ptr);
#endif
}