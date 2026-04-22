/*
 * LPNU libkern - Implementation
 * 
 * Based on XNU libkern but compatible with Linux.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lpnu_libkern.h"

const char* LPNULibkernGetVersion(void) {
    return "LPNU libkern 1.0.0 (XNU compatible)";
}

/* OSObject reference counting */
void LPNUOSObjectRetain(LPNUOSObject* obj) {
    if (obj) obj->refCount++;
}

void LPNUOSObjectRelease(LPNUOSObject* obj) {
    if (obj) {
        obj->refCount--;
        if (obj->refCount == 0) {
            LPNUOSObjectDestroy(obj);
        }
    }
}

uint32_t LPNUOSObjectGetRetainCount(LPNUOSObject* obj) {
    return obj ? obj->refCount : 0;
}

int LPNUOSObjectCreate(LPNUOSObject** obj, const char* className) {
    if (!obj) return -1;
    
    LPNUOSObject* o = (LPNUOSObject*)calloc(1, sizeof(LPNUOSObject));
    if (!o) return -2;
    
    o->className = className;
    o->refCount = 1;
    *obj = o;
    return 0;
}

void LPNUOSObjectDestroy(LPNUOSObject* obj) {
    if (obj) free(obj);
}

bool LPNUOSObjectIsEqual(LPNUOSObject* obj1, LPNUOSObject* obj2) {
    if (!obj1 || !obj2) return false;
    return obj1 == obj2;
}

LPNUOSObject* LPNUOSObjectCopy(LPNUOSObject* obj) {
    if (!obj) return NULL;
    /* Simple copy - subclasses should override */
    return obj;
}

const char* LPNUOSObjectGetClassName(LPNUOSObject* obj) {
    return obj ? obj->className : NULL;
}

/* OSString implementation */
int LPNUOSStringCreate(LPNUOSString** str, const char* cstring) {
    if (!str || !cstring) return -1;
    
    LPNUOSString* s = (LPNUOSString*)calloc(1, sizeof(LPNUOSString));
    if (!s) return -2;
    
    s->className = "OSString";
    s->refCount = 1;
    s->length = strlen(cstring);
    s->string = (char*)malloc(s->length + 1);
    if (!s->string) {
        free(s);
        return -2;
    }
    strcpy(s->string, cstring);
    *str = s;
    return 0;
}

void LPNUOSStringDestroy(LPNUOSString* str) {
    if (str) {
        if (str->string) free(str->string);
        free(str);
    }
}

const char* LPNUOSStringGetCString(LPNUOSString* str) {
    return str ? str->string : NULL;
}

uint32_t LPNUOSStringGetLength(LPNUOSString* str) {
    return str ? str->length : 0;
}

bool LPNUOSStringIsEqualTo(LPNUOSString* str, const char* cstring) {
    if (!str || !cstring) return false;
    return strcmp(str->string, cstring) == 0;
}

/* OSData implementation */
int LPNUOSDataCreate(LPNUOSData** data, const void* bytes, uint32_t length) {
    if (!data || !bytes || length == 0) return -1;
    
    LPNUOSData* d = (LPNUOSData*)calloc(1, sizeof(LPNUOSData));
    if (!d) return -2;
    
    d->className = "OSData";
    d->refCount = 1;
    d->length = length;
    d->data = malloc(length);
    if (!d->data) {
        free(d);
        return -2;
    }
    memcpy(d->data, bytes, length);
    *data = d;
    return 0;
}

void LPNUOSDataDestroy(LPNUOSData* data) {
    if (data) {
        if (data->data) free(data->data);
        free(data);
    }
}

void* LPNUOSDataGetBytes(LPNUOSData* data) {
    return data ? data->data : NULL;
}

uint32_t LPNUOSDataGetLength(LPNUOSData* data) {
    return data ? data->length : 0;
}

bool LPNUOSDataIsEqualTo(LPNUOSData* data, const void* bytes, uint32_t length) {
    if (!data || !bytes) return false;
    return data->length == length && memcmp(data->data, bytes, length) == 0;
}

/* OSDictionary implementation */
int LPNUOSDictionaryCreate(LPNUOSDictionary** dict) {
    if (!dict) return -1;
    
    LPNUOSDictionary* d = (LPNUOSDictionary*)calloc(1, sizeof(LPNUOSDictionary));
    if (!d) return -2;
    
    d->className = "OSDictionary";
    d->refCount = 1;
    d->capacity = 16;
    d->keys = (char**)calloc(d->capacity, sizeof(char*));
    d->values = (void**)calloc(d->capacity, sizeof(void*));
    if (!d->keys || !d->values) {
        if (d->keys) free(d->keys);
        if (d->values) free(d->values);
        free(d);
        return -2;
    }
    *dict = d;
    return 0;
}

void LPNUOSDictionaryDestroy(LPNUOSDictionary* dict) {
    if (!dict) return;
    if (dict->keys) free(dict->keys);
    if (dict->values) free(dict->values);
    free(dict);
}

int LPNUOSDictionarySetObject(LPNUOSDictionary* dict, const char* key, void* object) {
    if (!dict || !key) return -1;
    
    for (uint32_t i = 0; i < dict->count; i++) {
        if (dict->keys[i] && strcmp(dict->keys[i], key) == 0) {
            dict->values[i] = object;
            return 0;
        }
    }
    
    if (dict->count >= dict->capacity) {
        uint32_t newCap = dict->capacity * 2;
        char** newKeys = (char**)realloc(dict->keys, newCap * sizeof(char*));
        void** newValues = (void**)realloc(dict->values, newCap * sizeof(void*));
        if (!newKeys || !newValues) return -2;
        dict->keys = newKeys;
        dict->values = newValues;
        dict->capacity = newCap;
    }
    
    dict->keys[dict->count] = strdup(key);
    dict->values[dict->count] = object;
    dict->count++;
    return 0;
}

void* LPNUOSDictionaryGetObject(LPNUOSDictionary* dict, const char* key) {
    if (!dict || !key) return NULL;
    
    for (uint32_t i = 0; i < dict->count; i++) {
        if (dict->keys[i] && strcmp(dict->keys[i], key) == 0) {
            return dict->values[i];
        }
    }
    return NULL;
}

bool LPNUOSDictionaryRemoveObject(LPNUOSDictionary* dict, const char* key) {
    if (!dict || !key) return false;
    
    for (uint32_t i = 0; i < dict->count; i++) {
        if (dict->keys[i] && strcmp(dict->keys[i], key) == 0) {
            for (uint32_t j = i; j < dict->count - 1; j++) {
                dict->keys[j] = dict->keys[j + 1];
                dict->values[j] = dict->values[j + 1];
            }
            dict->count--;
            return true;
        }
    }
    return false;
}

uint32_t LPNUOSDictionaryGetCount(LPNUOSDictionary* dict) {
    return dict ? dict->count : 0;
}

void** LPNUOSDictionaryCopyKeys(LPNUOSDictionary* dict) {
    if (!dict) return NULL;
    void** keys = (void**)dict->keys;
    return keys;
}

/* OSSet implementation */
int LPNUOSSetCreate(LPNUOSSet** set) {
    if (!set) return -1;
    
    LPNUOSSet* s = (LPNUOSSet*)calloc(1, sizeof(LPNUOSSet));
    if (!s) return -2;
    
    s->className = "OSSet";
    s->refCount = 1;
    s->capacity = 16;
    s->objects = (void**)calloc(s->capacity, sizeof(void*));
    if (!s->objects) {
        free(s);
        return -2;
    }
    *set = s;
    return 0;
}

void LPNUOSSetDestroy(LPNUOSSet* set) {
    if (set) {
        if (set->objects) free(set->objects);
        free(set);
    }
}

int LPNUOSSetAddObject(LPNUOSSet* set, void* object) {
    if (!set || !object) return -1;
    
    for (uint32_t i = 0; i < set->count; i++) {
        if (set->objects[i] == object) return 0;
    }
    
    if (set->count >= set->capacity) {
        uint32_t newCap = set->capacity * 2;
        void** newObjects = (void**)realloc(set->objects, newCap * sizeof(void*));
        if (!newObjects) return -2;
        set->objects = newObjects;
        set->capacity = newCap;
    }
    
    set->objects[set->count++] = object;
    return 0;
}

bool LPNUOSSetContainsObject(LPNUOSSet* set, void* object) {
    if (!set || !object) return false;
    
    for (uint32_t i = 0; i < set->count; i++) {
        if (set->objects[i] == object) return true;
    }
    return false;
}

bool LPNUOSSetRemoveObject(LPNUOSSet* set, void* object) {
    if (!set || !object) return false;
    
    for (uint32_t i = 0; i < set->count; i++) {
        if (set->objects[i] == object) {
            for (uint32_t j = i; j < set->count - 1; j++) {
                set->objects[j] = set->objects[j + 1];
            }
            set->count--;
            return true;
        }
    }
    return false;
}

uint32_t LPNUOSSetGetCount(LPNUOSSet* set) {
    return set ? set->count : 0;
}

void** LPNUOSSetCopyObjects(LPNUOSSet* set) {
    if (!set) return NULL;
    return set->objects;
}

/* OSNumber implementation */
int LPNUOSNumberCreate(LPNUOSNumber** num, uint64_t value) {
    if (!num) return -1;
    
    LPNUOSNumber* n = (LPNUOSNumber*)calloc(1, sizeof(LPNUOSNumber));
    if (!n) return -2;
    
    n->className = "OSNumber";
    n->refCount = 1;
    n->value = value;
    *num = n;
    return 0;
}

void LPNUOSNumberDestroy(LPNUOSNumber* num) {
    if (num) free(num);
}

uint64_t LPNUOSNumberGetValue(LPNUOSNumber* num) {
    return num ? num->value : 0;
}

int LPNUOSNumberSetValue(LPNUOSNumber* num, uint64_t value) {
    if (!num) return -1;
    num->value = value;
    return 0;
}

/* OSSymbol implementation */
int LPNUOSSymbolCreate(LPNUOSSymbol** sym, const char* symbol) {
    if (!sym || !symbol) return -1;
    
    LPNUOSSymbol* s = (LPNUOSSymbol*)calloc(1, sizeof(LPNUOSSymbol));
    if (!s) return -2;
    
    s->className = "OSSymbol";
    s->refCount = 1;
    s->length = strlen(symbol);
    s->symbol = strdup(symbol);
    if (!s->symbol) {
        free(s);
        return -2;
    }
    *sym = s;
    return 0;
}

void LPNUOSSymbolDestroy(LPNUOSSymbol* sym) {
    if (sym) {
        if (sym->symbol) free(sym->symbol);
        free(sym);
    }
}

const char* LPNUOSSymbolGetCString(LPNUOSSymbol* sym) {
    return sym ? sym->symbol : NULL;
}

bool LPNUOSSymbolIsEqualTo(LPNUOSSymbol* sym, const char* cstring) {
    if (!sym || !cstring) return false;
    return strcmp(sym->symbol, cstring) == 0;
}

/* OSSerialize implementation */
int LPNUOSSerializeCreate(LPNUOSSerialize** ser) {
    if (!ser) return -1;
    
    LPNUOSSerialize* s = (LPNUOSSerialize*)calloc(1, sizeof(LPNUOSSerialize));
    if (!s) return -2;
    
    s->className = "OSSerialize";
    s->bufferSize = 256;
    s->currentSize = 0;
    s->buffer = (char*)malloc(s->bufferSize);
    if (!s->buffer) {
        free(s);
        return -2;
    }
    s->buffer[0] = '\0';
    *ser = s;
    return 0;
}

void LPNUOSSerializeDestroy(LPNUOSSerialize* ser) {
    if (ser) {
        if (ser->buffer) free(ser->buffer);
        free(ser);
    }
}

int LPNUOSSerializeAddString(LPNUOSSerialize* ser, const char* string) {
    if (!ser || !string) return -1;
    
    uint32_t len = strlen(string);
    if (ser->currentSize + len + 1 >= ser->bufferSize) {
        uint32_t newSize = ser->bufferSize * 2 + len;
        char* newBuffer = (char*)realloc(ser->buffer, newSize);
        if (!newBuffer) return -2;
        ser->buffer = newBuffer;
        ser->bufferSize = newSize;
    }
    
    strcat(ser->buffer, string);
    ser->currentSize += len + 1;
    return 0;
}

int LPNUOSSerializeAddNumber(LPNUOSSerialize* ser, uint64_t value) {
    if (!ser) return -1;
    
    char numStr[32];
    snprintf(numStr, sizeof(numStr), "%llu", (unsigned long long)value);
    return LPNUOSSerializeAddString(ser, numStr);
}

int LPNUOSSerializeAddData(LPNUOSSerialize* ser, const void* data, uint32_t length) {
    (void)ser;
    (void)data;
    (void)length;
    return 0;
}

char* LPNUOSSerializeGetString(LPNUOSSerialize* ser) {
    return ser ? ser->buffer : NULL;
}

uint32_t LPNUOSSerializeGetLength(LPNUOSSerialize* ser) {
    return ser ? ser->currentSize : 0;
}

/* Atomic operations */
int LPNUOSIncrementAtomic(int* addr) {
    if (!addr) return 0;
    return ++(*addr);
}

int LPNUOSDecrementAtomic(int* addr) {
    if (!addr) return 0;
    return --(*addr);
}

int LPNUOSCompareAndSwap(int oldValue, int newValue, int* addr) {
    if (!addr) return 0;
    if (*addr == oldValue) {
        *addr = newValue;
        return 1;
    }
    return 0;
}

void* LPNUOSCompareAndSwapPointer(void* oldValue, void* newValue, void** addr) {
    if (!addr) return NULL;
    void* orig = *addr;
    if (orig == oldValue) {
        *addr = newValue;
    }
    return orig;
}