/*
 * lpnu_kext_runtime.h - KEXT (Kernel Extension) Loader Runtime
 * 
 * This provides XNU-compatible KEXT loading at runtime.
 * Disabled by default for safety.
 */

#ifndef LPNU_KEXT_RUNTIME_H
#define LPNU_KEXT_RUNTIME_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LPNU_KEXT_DISABLED 0
#define LPNU_KEXT_ENABLED 1

typedef struct {
    char name[256];
    char path[512];
    uint64_t address;
    uint64_t size;
    int version;
    int refs;
    void* module;
    void* start;
    void* stop;
} LPnuKextInfo;

typedef struct {
    int (*start)(void*);
    int (*stop)(void*);
} LPnuKextMain;

/* Runtime control */
int LPnuKextSetRuntime(int enabled);
int LPnuKextGetRuntime(void);

/* KEXT loading */
int LPnuKextLoad(const char* path, LPnuKextInfo* info);
int LPnuKextUnload(LPnuKextInfo* info);
int LPnuKextGetInfo(const char* name, LPnuKextInfo* info);

/* KEXT list */
int LPnuKextList(LPnuKextInfo** list, int* count);
void LPnuKextFreeList(LPnuKextInfo* list);

/* Boot KEXTs (loaded at init) */
int LPnuKextLoadBootKexts(void);

/* Info */
const char* LPnuKextVersion(void);

#ifdef __cplusplus
}
#endif

#endif