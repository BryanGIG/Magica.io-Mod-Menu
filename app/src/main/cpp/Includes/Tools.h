#include <sys/mman.h>
#include <dobby.h>

namespace Tools {
    #define _SYS_PAGE_SIZE_ (sysconf(_SC_PAGE_SIZE))
    #define _PAGE_START_OF_(x)    ((uintptr_t)x & ~(uintptr_t)(_SYS_PAGE_SIZE_ - 1))
    #define _PAGE_END_OF_(x, len) (_PAGE_START_OF_((uintptr_t)x + len - 1))
    #define _PAGE_LEN_OF_(x, len) (_PAGE_END_OF_(x, len) - _PAGE_START_OF_(x) + _SYS_PAGE_SIZE_)
    #define _PAGE_OFFSET_OF_(x)   ((uintptr_t)x - _PAGE_START_OF_(x))

    #define _PROT_RWX_ (PROT_READ | PROT_WRITE | PROT_EXEC)
    #define _PROT_RX_  (PROT_READ | PROT_EXEC)

    bool SetProtRWX(void *addr) {
        uintptr_t pageStart = _PAGE_START_OF_(addr);
        uintptr_t pageLen = _PAGE_LEN_OF_(addr, 4);
        return (mprotect(reinterpret_cast<void *>(pageStart), pageLen, _PROT_RWX_) != -1);
    }

    void HookFunc(void *address, void *replace, void **backup) {
        if (SetProtRWX(address)) {
            DobbyHook(address, replace, backup);
        }
    }
}