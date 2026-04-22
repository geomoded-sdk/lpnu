/*
 * lpnu_dyld.h - Dynamic Linker (dyld) Compatibility for LPNU
 * 
 * dyld is Apple's dynamic linker. This implementation provides
 * Mach-O dynamic linking for LPNU.
 */

#ifndef LPNU_DYLD_H
#define LPNU_DYLD_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LPNUImageInfo* LPNUImageRef;
typedef struct LPNUSectionInfo* LPNUSectionRef;
typedef struct LPNUSymbolInfo* LPNUSymbolRef;

typedef struct {
    const char* name;
    uint64_t addr;
    uint64_t size;
} LPNUImage;

/* Load commands */
#define LPNU_LC_SEGMENT 0x1
#define LPNU_LC_SYMTAB 0x2
#define LPNU_LC_DYSYMTAB 0xb
#define LPNU_LC_LOAD_DYLIB 0xc
#define LPNU_LC_ID_DYLIB 0xd
#define LPNU_LC_LOAD_WEAK_DYLIB 0x80000018
#define LPNU_LC_REEXPORT_DYLIB 0x8000001c
#define LPNU_LC_RPATH (0x8000002c | 0x80000000)

/* Image states */
#define LPNUImageStateNotLoaded 0
#define LPNUImageStateMapped 1
#define LPNUImageStateLinked 2
#define LPNUImageStateInitializing 3
#define LPNUImageStateReady 4

/* API */
int LPNUdyld_load(const char* path, LPNUImageRef* image);
void LPNUdyld_unload(LPNUImageRef image);

uint64_t LPNUdyld_image_base(LPNUImageRef image);
const char* LPNUdyld_image_name(LPNUImageRef image);
int LPNUdyld_image_state(LPNUImageRef image);

void* LPNUdyld_get_symbol(LPNUImageRef image, const char* name);
void* LPNUdyld_get_symbol_all(const char* name);

int LPNUdyld_add_image(LPNUImageRef image);
void LPNUdyld_remove_image(LPNUImageRef image);

void** LPNUdyld_get_cache(void);
void** LPNUdyld_get_all_images(void);

/* Program initialization */
void LPNUdyld_init(int argc, char** argv, char** envp);
void LPNUdyld_run(int (*main)(int, char**, char**));

/* Info */
const char* LPNUdyld_version(void);

#ifdef __cplusplus
}
#endif

#endif