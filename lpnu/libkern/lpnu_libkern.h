/*
 * LPNU libkern - OSObject Implementation
 * 
 * Base classes for LPNU kernel.
 * Based on XNU libkern but compatible with Linux.
 */

#ifndef _LPNU_LIBKERN_H
#define _LPNU_LIBKERN_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* OSObject base */
typedef struct LPNUOSObject {
    const char* className;
    uint32_t refCount;
    void* userData;
} LPNUOSObject;

typedef LPNUOSObject* LPNUOSObjectPtr;

/* OSString */
typedef struct LPNUOSString {
    const char* className;
    uint32_t refCount;
    char* string;
    uint32_t length;
} LPNUOSString;

/* OSData */
typedef struct LPNUOSData {
    const char* className;
    uint32_t refCount;
    void* data;
    uint32_t length;
} LPNUOSData;

/* OSDictionary */
typedef struct LPNUOSDictionary {
    const char* className;
    uint32_t refCount;
    char** keys;
    void** values;
    uint32_t count;
    uint32_t capacity;
} LPNUOSDictionary;

/* OSSet */
typedef struct LPNUOSSet {
    const char* className;
    uint32_t refCount;
    void** objects;
    uint32_t count;
    uint32_t capacity;
} LPNUOSSet;

/* OSNumber */
typedef struct LPNUOSNumber {
    const char* className;
    uint32_t refCount;
    uint64_t value;
} LPNUOSNumber;

/* OSSymbol */
typedef struct LPNUOSSymbol {
    const char* className;
    uint32_t refCount;
    char* symbol;
    uint32_t length;
} LPNUOSSymbol;

/* OSSerialize */
typedef struct LPNUOSSerialize {
    const char* className;
    char* buffer;
    uint32_t bufferSize;
    uint32_t currentSize;
} LPNUOSSerialize;

/* Reference counting */
void LPNUOSObjectRetain(LPNUOSObject* obj);
void LPNUOSObjectRelease(LPNUOSObject* obj);
uint32_t LPNUOSObjectGetRetainCount(LPNUOSObject* obj);

/* OSObject functions */
int LPNUOSObjectCreate(LPNUOSObject** obj, const char* className);
void LPNUOSObjectDestroy(LPNUOSObject* obj);
bool LPNUOSObjectIsEqual(LPNUOSObject* obj1, LPNUOSObject* obj2);
LPNUOSObject* LPNUOSObjectCopy(LPNUOSObject* obj);
const char* LPNUOSObjectGetClassName(LPNUOSObject* obj);

/* OSString functions */
int LPNUOSStringCreate(LPNUOSString** str, const char* cstring);
void LPNUOSStringDestroy(LPNUOSString* str);
const char* LPNUOSStringGetCString(LPNUOSString* str);
uint32_t LPNUOSStringGetLength(LPNUOSString* str);
bool LPNUOSStringIsEqualTo(LPNUOSString* str, const char* cstring);

/* OSData functions */
int LPNUOSDataCreate(LPNUOSData** data, const void* bytes, uint32_t length);
void LPNUOSDataDestroy(LPNUOSData* data);
void* LPNUOSDataGetBytes(LPNUOSData* data);
uint32_t LPNUOSDataGetLength(LPNUOSData* data);
bool LPNUOSDataIsEqualTo(LPNUOSData* data, const void* bytes, uint32_t length);

/* OSDictionary functions */
int LPNUOSDictionaryCreate(LPNUOSDictionary** dict);
void LPNUOSDictionaryDestroy(LPNUOSDictionary* dict);
int LPNUOSDictionarySetObject(LPNUOSDictionary* dict, const char* key, void* object);
void* LPNUOSDictionaryGetObject(LPNUOSDictionary* dict, const char* key);
bool LPNUOSDictionaryRemoveObject(LPNUOSDictionary* dict, const char* key);
uint32_t LPNUOSDictionaryGetCount(LPNUOSDictionary* dict);
void** LPNUOSDictionaryCopyKeys(LPNUOSDictionary* dict);

/* OSSet functions */
int LPNUOSSetCreate(LPNUOSSet** set);
void LPNUOSSetDestroy(LPNUOSSet* set);
int LPNUOSSetAddObject(LPNUOSSet* set, void* object);
bool LPNUOSSetContainsObject(LPNUOSSet* set, void* object);
bool LPNUOSSetRemoveObject(LPNUOSSet* set, void* object);
uint32_t LPNUOSSetGetCount(LPNUOSSet* set);
void** LPNUOSSetCopyObjects(LPNUOSSet* set);

/* OSNumber functions */
int LPNUOSNumberCreate(LPNUOSNumber** num, uint64_t value);
void LPNUOSNumberDestroy(LPNUOSNumber* num);
uint64_t LPNUOSNumberGetValue(LPNUOSNumber* num);
int LPNUOSNumberSetValue(LPNUOSNumber* num, uint64_t value);

/* OSSymbol functions */
int LPNUOSSymbolCreate(LPNUOSSymbol** sym, const char* symbol);
void LPNUOSSymbolDestroy(LPNUOSSymbol* sym);
const char* LPNUOSSymbolGetCString(LPNUOSSymbol* sym);
bool LPNUOSSymbolIsEqualTo(LPNUOSSymbol* sym, const char* cstring);

/* OSSerialize functions */
int LPNUOSSerializeCreate(LPNUOSSerialize** ser);
void LPNUOSSerializeDestroy(LPNUOSSerialize* ser);
int LPNUOSSerializeAddString(LPNUOSSerialize* ser, const char* string);
int LPNUOSSerializeAddNumber(LPNUOSSerialize* ser, uint64_t value);
int LPNUOSSerializeAddData(LPNUOSSerialize* ser, const void* data, uint32_t length);
char* LPNUOSSerializeGetString(LPNUOSSerialize* ser);
uint32_t LPNUOSSerializeGetLength(LPNUOSSerialize* ser);

/* Atomic operations */
int LPNUOSIncrementAtomic(int* addr);
int LPNUOSDecrementAtomic(int* addr);
int LPNUOSCompareAndSwap(int oldValue, int newValue, int* addr);
void* LPNUOSCompareAndSwapPointer(void* oldValue, void* newValue, void** addr);

/* OSArray */
typedef struct LPNUOSArray {
    const char* className;
    uint32_t refCount;
    void** objects;
    uint32_t count;
    uint32_t capacity;
} LPNUOSArray;

/* OSIterator */
typedef struct LPNUOSIterator {
    const char* className;
    void** objects;
    uint32_t index;
    uint32_t count;
} LPNUOSIterator;

/* OSOrderedSet */
typedef struct LPNUOSOrderedSet {
    const char* className;
    uint32_t refCount;
    void** objects;
    uint32_t count;
    uint32_t capacity;
    int (*compare)(void*, void*);
} LPNUOSOrderedSet;

/* libkern utilities */
void* LPNUOSMalloc(uint64_t size);
void LPNUOSFree(void* ptr, uint64_t size);
void* LPNUOSRealloc(void* ptr, uint64_t oldSize, uint64_t newSize);
void* LPNUOSMemcpy(void* dest, const void* src, uint64_t n);
void* LPNUOSMemset(void* s, int c, uint64_t n);
int LPNUOSMemcmp(const void* s1, const void* s2, uint64_t n);

/* OSArray functions */
int LPNUOSArrayCreate(LPNUOSArray** array, uint32_t capacity);
void LPNUOSArrayDestroy(LPNUOSArray* array);
int LPNUOSArrayAddObject(LPNUOSArray* array, void* object);
int LPNUOSArrayRemoveObject(LPNUOSArray* array, void* object);
void* LPNUOSArrayGetObject(LPNUOSArray* array, uint32_t index);
uint32_t LPNUOSArrayGetCount(LPNUOSArray* array);
int LPNUOSArraySetObject(LPNUOSArray* array, uint32_t index, void* object);

/* OSIterator functions */
int LPNUOSIteratorCreate(LPNUOSIterator** iter, void** objects, uint32_t count);
void LPNUOSIteratorDestroy(LPNUOSIterator* iter);
void* LPNUOSIteratorNext(LPNUOSIterator* iter);
void* LPNUOSIteratorReset(LPNUOSIterator* iter);
bool LPNUOSIteratorIsValid(LPNUOSIterator* iter);

/* OSOrderedSet functions */
int LPNUOSOrderedSetCreate(LPNUOSOrderedSet** set, int (*compare)(void*, void*));
void LPNUOSOrderedSetDestroy(LPNUOSOrderedSet* set);
int LPNUOSOrderedSetAddObject(LPNUOSOrderedSet* set, void* object);
void* LPNUOSOrderedSetGetObject(LPNUOSOrderedSet* set, uint32_t index);
bool LPNUOSOrderedSetContainsObject(LPNUOSOrderedSet* set, void* object);

/* OSTimer */
typedef void (*LPNUOSTimerCallback)(void* ref);
typedef struct LPNUOSTimer {
    const char* className;
    uint32_t refCount;
    uint64_t interval;
    bool armed;
    LPNUOSTimerCallback callback;
    void* ref;
} LPNUOSTimer;

int LPNUOSTimerCreate(LPNUOSTimer** timer, uint64_t interval, LPNUOSTimerCallback callback, void* ref);
void LPNUOSTimerDestroy(LPNUOSTimer* timer);
int LPNUOSTimerArm(LPNUOSTimer* timer);
int LPNUOSTimerDisarm(LPNUOSTimer* timer);
int LPNUOSTimerFire(LPNUOSTimer* timer);

/* OSLock */
typedef struct LPNUOSLock {
    const char* className;
    uint32_t refCount;
    void* lock;
} LPNUOSLock;

int LPNUOSLockCreate(LPNUOSLock** lock);
void LPNUOSLockDestroy(LPNUOSLock* lock);
int LPNUOSLockAcquire(LPNUOSLock* lock);
int LPNUOSLockRelease(LPNUOSLock* lock);
bool LPNUOSLockTryAcquire(LPNUOSLock* lock);

/* Debug */
void LPNUOSDebugLog(const char* format, ...);
void LPNUOSPanic(const char* reason);

/* Version */
const char* LPNULibkernGetVersion(void);

#ifdef __cplusplus
}
#endif

#endif