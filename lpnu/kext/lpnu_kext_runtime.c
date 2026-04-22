/*
 * lpnu_kext_runtime.c - KEXT Loader Runtime Implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/stat.h>

#include "lpnu_kext_runtime.h"

static int g_kext_runtime_enabled = LPNU_KEXT_DISABLED;
static LPnuKextInfo* g_loaded_kexts[256];
static int g_kext_count = 0;

const char* LPnuKextVersion(void) {
    return "LPNU KEXT Runtime 1.0.0";
}

int LPnuKextSetRuntime(int enabled) {
    if (enabled != LPNU_KEXT_DISABLED && enabled != LPNU_KEXT_ENABLED) {
        return -1;
    }
    g_kext_runtime_enabled = enabled;
    return 0;
}

int LPnuKextGetRuntime(void) {
    return g_kext_runtime_enabled;
}

int LPnuKextLoad(const char* path, LPnuKextInfo* info) {
    if (!path || !info) return -1;
    if (g_kext_runtime_enabled == LPNU_KEXT_DISABLED) {
        fprintf(stderr, "LPNU KEXT: runtime is disabled\n");
        return -2;
    }
    
    struct stat st;
    if (stat(path, &st) != 0) {
        fprintf(stderr, "LPNU KEXT: file not found: %s\n", path);
        return -3;
    }
    
    void* handle = dlopen(path, RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "LPNU KEXT: failed to load %s: %s\n", path, dlerror());
        return -4;
    }
    
    LPnuKextMain* kext_main = (LPnuKextMain*)dlsym(handle, "kext_main");
    if (!kext_main) {
        kext_main = (LPnuKextMain*)dlsym(handle, "_kext_main");
    }
    
    memset(info, 0, sizeof(LPnuKextInfo));
    strncpy(info->path, path, 511);
    strncpy(info->name, path, 255);
    char* basename = strrchr(info->name, '/');
    if (basename) memmove(info->name, basename + 1, strlen(basename));
    
    if (strstr(info->name, ".kext")) {
        char* ext = strstr(info->name, ".kext");
        *ext = '\0';
    }
    
    info->address = (uint64_t)handle;
    info->size = st.st_size;
    info->module = handle;
    
    if (kext_main && kext_main->start) {
        int ret = kext_main->start(NULL);
        if (ret != 0) {
            fprintf(stderr, "LPNU KEXT: start failed for %s\n", info->name);
            dlclose(handle);
            return -5;
        }
        info->start = (void*)kext_main->start;
        info->stop = (void*)kext_main->stop;
    }
    
    if (g_kext_count < 256) {
        g_loaded_kexts[g_kext_count++] = info;
    }
    
    return 0;
}

int LPnuKextUnload(LPnuKextInfo* info) {
    if (!info) return -1;
    
    if (info->stop) {
        ((int(*)(void*))info->stop(NULL);
    }
    
    if (info->module) {
        dlclose(info->module);
    }
    
    for (int i = 0; i < g_kext_count; i++) {
        if (g_loaded_kexts[i] == info) {
            for (int j = i; j < g_kext_count - 1; j++) {
                g_loaded_kexts[j] = g_loaded_kexts[j + 1];
            }
            g_kext_count--;
            break;
        }
    }
    
    return 0;
}

int LPnuKextGetInfo(const char* name, LPnuKextInfo* info) {
    if (!name || !info) return -1;
    
    for (int i = 0; i < g_kext_count; i++) {
        if (g_loaded_kexts[i] && strcmp(g_loaded_kexts[i]->name, name) == 0) {
            memcpy(info, g_loaded_kexts[i], sizeof(LPnuKextInfo));
            return 0;
        }
    }
    
    return -2;
}

int LPnuKextList(LPnuKextInfo** list, int* count) {
    if (!list || !count) return -1;
    
    *list = (LPnuKextInfo*)calloc(g_kext_count, sizeof(LPnuKextInfo));
    if (!*list) return -2;
    
    for (int i = 0; i < g_kext_count; i++) {
        if (g_loaded_kexts[i]) {
            memcpy(&(*list)[i], g_loaded_kexts[i], sizeof(LPnuKextInfo));
        }
    }
    
    *count = g_kext_count;
    return 0;
}

void LPnuKextFreeList(LPnuKextInfo* list) {
    if (list) free(list);
}

int LPnuKextLoadBootKexts(void) {
    if (g_kext_runtime_enabled == LPNU_KEXT_DISABLED) {
        return 0;
    }
    
    char* kext_paths[] = {
        "/System/Library/Extensions",
        "/Library/Extensions",
        NULL
    };
    
    int loaded = 0;
    
    for (int i = 0; kext_paths[i]; i++) {
        fprintf(stderr, "LPNU KEXT: checking %s\n", kext_paths[i]);
    }
    
    return loaded;
}