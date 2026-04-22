/*
 * lpnu_dyld.c - Dynamic Linker Implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "lpnu_dyld.h"
#include "../mach_o.h"

struct LPNUImageInfo {
    char path[256];
    void* handle;
    uint64_t base;
    int state;
    char name[256];
};

static LPNUImageRef g_images[256];
static int g_image_count = 0;

const char* LPNUdyld_version(void) {
    return "LPNU dyld 1.0.0";
}

int LPNUdyld_load(const char* path, LPNUImageRef* image) {
    if (!path || !image) return -1;
    
    void* handle = dlopen(path, RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "LPNUdyld: failed to load %s: %s\n", path, dlerror());
        return -2;
    }
    
    LPNUImageRef img = (LPNUImageRef)calloc(1, sizeof(struct LPNUImageInfo));
    if (!img) {
        dlclose(handle);
        return -3;
    }
    
    strncpy(img->path, path, 255);
    strncpy(img->name, path, 255);
    img->handle = handle;
    img->base = (uint64_t)handle;
    img->state = LPNUImageStateReady;
    
    *image = img;
    return 0;
}

void LPNUdyld_unload(LPNUImageRef image) {
    if (!image) return;
    if (image->handle) dlclose(image->handle);
    free(image);
}

uint64_t LPNUdyld_image_base(LPNUImageRef image) {
    return image ? image->base : 0;
}

const char* LPNUdyld_image_name(LPNUImageRef image) {
    return image ? image->name : NULL;
}

int LPNUdyld_image_state(LPNUImageRef image) {
    return image ? image->state : LPNUImageStateNotLoaded;
}

void* LPNUdyld_get_symbol(LPNUImageRef image, const char* name) {
    if (!image || !name || !image->handle) return NULL;
    return dlsym(image->handle, name);
}

void* LPNUdyld_get_symbol_all(const char* name) {
    if (!name) return NULL;
    return dlsym(RTLD_DEFAULT, name);
}

int LPNUdyld_add_image(LPNUImageRef image) {
    if (!image || g_image_count >= 255) return -1;
    g_images[g_image_count++] = image;
    return 0;
}

void LPNUdyld_remove_image(LPNUImageRef image) {
    if (!image) return;
    for (int i = 0; i < g_image_count; i++) {
        if (g_images[i] == image) {
            for (int j = i; j < g_image_count - 1; j++) {
                g_images[j] = g_images[j + 1];
            }
            g_image_count--;
            break;
        }
    }
}

void** LPNUdyld_get_cache(void) {
    return (void**)RTLD_DEFAULT;
}

void** LPNUdyld_get_all_images(void) {
    return (void**)g_images;
}

void LPNUdyld_init(int argc, char** argv, char** envp) {
    (void)argc;
    (void)argv;
    (void)envp;
}

void LPNUdyld_run(int (*main)(int, char**, char**)) {
    if (main) {
        int argc = 0;
        char** argv = NULL;
        char** envp = NULL;
        exit(main(argc, argv, envp));
    }
}