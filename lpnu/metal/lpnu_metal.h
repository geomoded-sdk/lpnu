/*
 * lpnu_metal.h - Metal API Compatibility Layer for LPNU
 * 
 * Metal is Apple's graphics API. This layer provides compatibility
 * by translating Metal calls to Vulkan/OpenGL on Linux.
 */

#ifndef LPNU_METAL_H
#define LPNU_METAL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef int32_t IOReturn;

#ifdef __cplusplus
extern "C" {
#endif

/* Metal device types */
typedef struct LPnuMetalDevice* LPnuMetalDevice_t;
typedef struct LPnuMetalBuffer* LPnuMetalBuffer_t;
typedef struct LPnuMetalTexture* LPnuMetalTexture_t;
typedef struct LPnuMetalSampler* LPnuMetalSampler_t;
typedef struct LPnuMetalRenderPipeline* LPnuMetalRenderPipeline_t;
typedef struct LPnuMetalCommandQueue* LPnuMetalCommandQueue_t;
typedef struct LPnuMetalCommandBuffer* LPnuMetalCommandBuffer_t;
typedef struct LPnuMetalLibrary* LPnuMetalLibrary_t;
typedef struct LPnuMetalFunction* LPnuMetalFunction_t;
typedef struct LPnuMetalComputePipeline* LPnuMetalComputePipeline_t;
typedef struct LPnuMetalHeap* LPnuMetalHeap_t;
typedef struct LPnuMetalFence* LPnuMetalFence_t;

/* Pixel formats */
#define LPnuMTLPixelFormatInvalid 0
#define LPnuMTLPixelFormatRGBA8Unorm 70
#define LPnuMTLPixelFormatBGRA8Unorm 80
#define LPnuMTLPixelFormatRGBA16Float 83
#define LPnuMTLPixelFormatRGBA32Float 111
#define LPnuMTLPixelFormatR8Unorm 10
#define LPnuMTLPixelFormatR8Unorm_lit 11

/* Texture types */
enum {
    LPnuMTLTextureType1D = 0,
    LPnuMTLTextureType2D = 1,
    LPnuMTLTextureType3D = 2,
    LPnuMTLTextureTypeCube = 3,
    LPnuMTLTextureType1DArray = 4,
    LPnuMTLTextureType2DArray = 5,
    LPnuMTLTextureTypeCubeArray = 6
};

/* Buffer storage modes */
#define LPnuMTLBufferStorageModeShared (1 << 0)
#define LPnuMTLBufferStorageModeManaged (1 << 1)
#define LPnuMTLBufferStorageModePrivate (1 << 2)

/* Return codes */
#define kIOReturnSuccess 0
#define kIOReturnError -1

/* Core functions - already implemented */
int LPnuMetalCreateDevice(LPnuMetalDevice_t* device);
void LPnuMetalReleaseDevice(LPnuMetalDevice_t device);
const char* LPnuMetalGetDriverName(void);
const char* LPnuMetalGetDriverVersion(void);

/* Buffer functions */
int LPnuMetalBufferCreate(LPnuMetalDevice_t device, size_t length, uint32_t options, LPnuMetalBuffer_t* buffer);
void LPnuMetalBufferDestroy(LPnuMetalBuffer_t buffer);
void* LPnuMetalBufferGetContents(LPnuMetalBuffer_t buffer);
size_t LPnuMetalBufferGetLength(LPnuMetalBuffer_t buffer);

/* Texture functions */
int LPnuMetalTextureCreate(LPnuMetalDevice_t device, uint32_t pixelFormat, uint32_t width, uint32_t height, uint32_t mipLevels, LPnuMetalTexture_t* texture);
void LPnuMetalTextureDestroy(LPnuMetalTexture_t texture);

/* Sampler functions */
int LPnuMetalSamplerCreate(LPnuMetalDevice_t device, LPnuMetalSampler_t* sampler);
void LPnuMetalSamplerDestroy(LPnuMetalSampler_t sampler);

/* Command queue/buffer functions */
int LPnuMetalCommandQueueCreate(LPnuMetalDevice_t device, LPnuMetalCommandQueue_t* queue);
void LPnuMetalCommandQueueDestroy(LPnuMetalCommandQueue_t queue);
int LPnuMetalCommandBufferCreate(LPnuMetalCommandQueue_t queue, LPnuMetalCommandBuffer_t* buffer);
void LPnuMetalCommandBufferDestroy(LPnuMetalCommandBuffer_t buffer);
void LPnuMetalCommandBufferCommit(LPnuMetalCommandBuffer_t buffer);
void LPnuMetalCommandBufferWait(LPnuMetalCommandBuffer_t buffer);

/* Library/function functions */
int LPnuMetalLibraryCreateFromSource(LPnuMetalDevice_t device, const char* source, LPnuMetalLibrary_t* library);
int LPnuMetalLibraryGetFunction(LPnuMetalLibrary_t library, const char* name, LPnuMetalFunction_t* function);

/* Pipeline functions */
int LPnuMetalRenderPipelineCreate(LPnuMetalDevice_t device, LPnuMetalLibrary_t library, const char* vertexFunc, const char* fragmentFunc, LPnuMetalRenderPipeline_t* pipeline);
void LPnuMetalRenderPipelineDestroy(LPnuMetalRenderPipeline_t pipeline);
void LPnuMetalRenderPipelineSetBuffer(LPnuMetalRenderPipeline_t pipeline, uint32_t index, LPnuMetalBuffer_t buffer);
void LPnuMetalRenderPipelineSetTexture(LPnuMetalRenderPipeline_t pipeline, uint32_t index, LPnuMetalTexture_t texture);

int LPnuMetalComputePipelineCreate(LPnuMetalDevice_t device, LPnuMetalFunction_t function, LPnuMetalComputePipeline_t* pipeline);
void LPnuMetalComputePipelineDestroy(LPnuMetalComputePipeline_t pipeline);

/* GPU family support */
int LPnuMetalDeviceSupportsFamily(LPnuMetalDevice_t device, int family);
int LPnuMetalDeviceHasHeap(LPnuMetalDevice_t device);

/* Heap functions */
IOReturn LPnuMetalHeapCreate(LPnuMetalDevice_t device, uint64_t size, LPnuMetalHeap_t* heap);
void LPnuMetalHeapDestroy(LPnuMetalHeap_t heap);
IOReturn LPnuMetalHeapCreateBuffer(LPnuMetalHeap_t heap, uint64_t size, LPnuMetalBuffer_t* buffer);
IOReturn LPnuMetalHeapCreateTexture(LPnuMetalHeap_t heap, uint32_t width, uint32_t height, uint32_t format, LPnuMetalTexture_t* texture);

/* Fence functions */
IOReturn LPnuMetalFenceCreate(LPnuMetalDevice_t device, LPnuMetalFence_t* fence);
void LPnuMetalFenceDestroy(LPnuMetalFence_t fence);
IOReturn LPnuMetalFenceSignal(LPnuMetalFence_t fence);
IOReturn LPnuMetalFenceWait(LPnuMetalFence_t fence, uint64_t timeout);

#ifdef __cplusplus
}
#endif

#endif