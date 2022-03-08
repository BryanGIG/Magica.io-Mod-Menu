#include <android/log.h>
#include "Vector3.hpp"
#include "Vector2.hpp"
#include "Quaternion.hpp"
#include "Rect.hpp"
#include "Color.hpp"
#include "Matrix4x4.hpp"
#include <cstdlib>
#include <uchar.h>
#include <codecvt>
#include "Internal.h"

float NormalizeAngle(float angle) {
    while (angle > 360)
        angle -= 360;
    while (angle < 0)
        angle += 360;
    return angle;
}

Vector3 NormalizeAngles(Vector3 angles) {
    angles.X = NormalizeAngle(angles.X);
    angles.Y = NormalizeAngle(angles.Y);
    angles.Z = NormalizeAngle(angles.Z);
    return angles;
}

Vector3 ToEulerRad(Quaternion q1) {
    float Rad2Deg = 360.0 / (M_PI * 2.0);

    float sqw = q1.W * q1.W;
    float sqx = q1.X * q1.X;
    float sqy = q1.Y * q1.Y;
    float sqz = q1.Z * q1.Z;
    float unit = sqx + sqy + sqz + sqw;
    float test = q1.X * q1.W - q1.Y * q1.Z;
    Vector3 v;

    if (test > 0.4995 * unit) {
        v.Y = 2.0 * atan2f(q1.Y, q1.X);
        v.X = M_PI / 2.0;
        v.Z = 0;
        return NormalizeAngles(v * Rad2Deg);
    }
    if (test < -0.4995 * unit) {
        v.Y = -2.0 * atan2f(q1.Y, q1.X);
        v.X = -M_PI / 2.0;
        v.Z = 0;
        return NormalizeAngles(v * Rad2Deg);
    }
    Quaternion q(q1.W, q1.Z, q1.X, q1.Y);
    v.Y = atan2f(2.0 * q.X * q.W + 2.0 * q.Y * q.Z, 1 - 2.0 * (q.Z * q.Z + q.W * q.W)); // yaw
    v.X = asinf(2.0 * (q.X * q.Z - q.W * q.Y)); // pitch
    v.Z = atan2f(2.0 * q.X * q.Y + 2.0 * q.Z * q.W, 1 - 2.0 * (q.Y * q.Y + q.Z * q.Z)); // roll
    return NormalizeAngles(v * Rad2Deg);
}

Quaternion GetRotationToLocation(Vector3 targetLocation, float y_bias, Vector3 myLoc) {
    return Quaternion::LookRotation((targetLocation + Vector3(0, y_bias, 0)) - myLoc,
                                    Vector3(0, 1, 0));
}

typedef unsigned short UTF16;
typedef wchar_t UTF32;
typedef char UTF8;

int is_surrogate(UTF16 uc) {
    return (uc - 0xd800u) < 2048u;
}

int is_high_surrogate(UTF16 uc) {
    return (uc & 0xfffffc00) == 0xd800;
}

int is_low_surrogate(UTF16 uc) {
    return (uc & 0xfffffc00) == 0xdc00;
}

UTF32 surrogate_to_utf32(UTF16 high, UTF16 low) {
    return (high << 10) + low - 0x35fdc00;
}

const char *utf16_to_utf8(const UTF16 *source, size_t len) {
    std::u16string s(source, source + len);
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.to_bytes(s).c_str();
}

const wchar_t *utf16_to_utf32(const UTF16 *source, size_t len) {
    auto output = new UTF32[len + 1];

    for (int i = 0; i < len; i++) {
        const UTF16 uc = source[i];
        if (!is_surrogate(uc)) {
            output[i] = uc;
        } else {
            if (is_high_surrogate(uc) && is_low_surrogate(source[i]))
                output[i] = surrogate_to_utf32(uc, source[i]);
            else
                output[i] = L'?';
        }
    }

    output[len] = L'\0';
    return output;
}

template<typename T>
struct Il2CppArray {
    Il2CppClass *klass;
    void *monitor;
    void *bounds;
    int max_length;
    T m_Items[65535];

    int getLength() {
        return max_length;
    }

    T getPointer() {
        return (T) m_Items;
    }
};

template<typename T>
using Array = Il2CppArray<T>;

struct Il2CppString {
    Il2CppClass *klass;
    void *monitor;
    int32_t length;
    uint16_t start_char;

    const char *CString() {
        return utf16_to_utf8(&this->start_char, this->length);
    }

    const wchar_t *WCString() {
        return utf16_to_utf32(&this->start_char, this->length);
    }

    int getLength() {
        return length;
    }
};

typedef Il2CppString String;

template<typename T>
struct Il2CppList {
    Il2CppClass *klass;
    void *unk1;
    Il2CppArray<T> *items;
    int size;
    int version;

    T getItems() {
        return items->getPointer();
    }

    int getSize() {
        return size;
    }

    int getVersion() {
        return version;
    }
};

template<typename T>
using List = Il2CppList<T>;

template<typename K, typename V>
struct Il2CppDictionary {
    Il2CppClass *klass;
    void *unk1;
    Il2CppArray<int **> *table;
    Il2CppArray<void **> *linkSlots;
    Il2CppArray<K> *keys;
    Il2CppArray<V> *values;
    int touchedSlots;
    int emptySlot;
    int size;

    K getKeys() {
        return keys->getPointer();
    }

    V getValues() {
        return values->getPointer();
    }

    int getNumKeys() {
        return keys->getLength();
    }

    int getNumValues() {
        return values->getLength();
    }

    int getSize() {
        return size;
    }
};

template<typename K, typename V>
using Dictionary = Il2CppDictionary<K, V>;
union intfloat {
    int i;
    float f;
};

/*
Get the real value of an ObscuredInt.
Parameters:
	- location: the location of the ObscuredInt
*/
int GetObscuredIntValue(uint64_t location) {
    int cryptoKey = *(int *) location;
    int obfuscatedValue = *(int *) (location + 0x4);

    return obfuscatedValue ^ cryptoKey;
}

int GetObscuredBoolValue(uint64_t location) {
    int cryptoKey = *(int *) (location + 0x8);
    int obfuscatedValue = *(int *) (location + 0xC);
    obfuscatedValue ^= cryptoKey;
    return obfuscatedValue;
}

/*
Set the real value of an ObscuredInt.
Parameters:
	- location: the location of the ObscuredInt
	- value: the value we're setting the ObscuredInt to
*/
void SetObscuredIntValue(uint64_t location, int value) {
    int cryptoKey = *(int *) location;

    *(int *) (location + 0x4) = value ^ cryptoKey;
}

/*
Get the real value of an ObscuredFloat.
Parameters:
	- location: the location of the ObscuredFloat
*/
float GetObscuredFloatValue(uint64_t location) {
    int cryptoKey = *(int *) location;
    int obfuscatedValue = *(int *) (location + 0x4);

    /* use this intfloat to set the integer representation of our parameter value, which will also set the float value */
    intfloat IF;
    IF.i = obfuscatedValue ^ cryptoKey;

    return IF.f;
}

/*
Set the real value of an ObscuredFloat.
Parameters:
	- location: the location of the ObscuredFloat
	- value: the value we're setting the ObscuredFloat to
*/
void SetObscuredFloatValue(uint64_t location, float value) {
    int cryptoKey = *(int *) location;

    /* use this intfloat to get the integer representation of our parameter value */
    intfloat IF;
    IF.f = value;

    /* use this intfloat to generate our hacked ObscuredFloat */
    intfloat IF2;
    IF2.i = IF.i ^ cryptoKey;

    *(float *) (location + 0x4) = IF2.f;
}
