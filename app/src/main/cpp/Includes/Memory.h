//
// Created by User on 23/08/2021.
//

#ifndef MOBILE_LEGENDS_MOD_MEMORY_H
#define MOBILE_LEGENDS_MOD_MEMORY_H

/*
     * Wrapper to dereference & get value of a multi level pointer
     * Make sure to use the correct data type!
     */
template<typename Type>
Type readMultiPtr(void *ptr, std::vector<int> offsets) {
    Type defaultVal = {};
    if (ptr == NULL)
        return defaultVal;

    uintptr_t finalPtr = reinterpret_cast<uintptr_t>(ptr);
    int offsetsSize = offsets.size();
    if (offsetsSize > 0) {
        for (int i = 0; finalPtr != 0 && i < offsetsSize; i++) {
            if (i == (offsetsSize - 1))
                return *reinterpret_cast<Type *>(finalPtr + offsets[i]);

            finalPtr = *reinterpret_cast<uintptr_t *>(finalPtr + offsets[i]);
        }
    }

    if (finalPtr == 0)
        return defaultVal;

    return *reinterpret_cast<Type *>(finalPtr);
}


/*
 * Wrapper to dereference & set value of a multi level pointer
 * Make sure to use the correct data type!, const objects won't work
 */
template<typename Type>
bool writeMultiPtr(void *ptr, std::vector<int> offsets, Type val) {
    if (ptr == NULL)
        return false;

    uintptr_t finalPtr = reinterpret_cast<uintptr_t>(ptr);
    int offsetsSize = offsets.size();
    if (offsetsSize > 0) {
        for (int i = 0; finalPtr != 0 && i < offsetsSize; i++) {
            if (i == (offsetsSize - 1)) {
                *reinterpret_cast<Type *>(finalPtr + offsets[i]) = val;
                return true;
            }

            finalPtr = *reinterpret_cast<uintptr_t *>(finalPtr + offsets[i]);
        }
    }

    if (finalPtr == 0)
        return false;

    *reinterpret_cast<Type *>(finalPtr) = val;
    return true;
}

/*
 * Wrapper to dereference & get value of a pointer
 * Make sure to use the correct data type!
 */
template<typename Type>
Type readPtr(void *ptr) {
    Type defaultVal = {};
    if (ptr == NULL)
        return defaultVal;

    return *reinterpret_cast<Type *>(ptr);
}

/*
 * Wrapper to dereference & set value of a pointer
 * Make sure to use the correct data type!, const objects won't work
 */
template<typename Type>
bool writePtr(void *ptr, Type val) {
    if (ptr == NULL)
        return false;

    *reinterpret_cast<Type *>(ptr) = val;
    return true;
}

#endif //MOBILE_LEGENDS_MOD_MEMORY_H
