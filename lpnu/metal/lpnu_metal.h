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

/* Pixel formats */
#define LPnuMTLPixelFormatInvalid 0
#define LPnuMTLPixelFormatRGBA8Unorm 70
#define LPnuMTLPixelFormatBGRA8Unorm 80
#define LPnuMTLPixelFormatRGBA16Float 83
#define LPnuMTLPixelFormatRGBA32Float 111

/* Buffer attributes */
#define LPnuMTLBufferAttributeReadOnly (1 << 0)
#define LPnuMTLBufferAttributeStorageModeShared (1 << 1)

/* Functions */
int LPnuMetalCreateDevice(LPnuMetalDevice_t* device);
void LPnuMetalReleaseDevice(LPnuMetalDevice_t device);

int LPnuMetalBufferCreate(LPnuMetalDevice_t device, size_t length, 
                       uint32_t options, LPnuMetalBuffer_t* buffer);
void LPnuMetalBufferDestroy(LPnuMetalBuffer_t buffer);
void* LPnuMetalBufferGetContents(LPnuMetalBuffer_t buffer);
size_t LPnuMetalBufferGetLength(LPnuMetalBuffer_t buffer);

int LPnuMetalTextureCreate(LPnuMetalDevice_t device, 
                          uint32_t pixelFormat,
                          uint32_t width, uint32_t height,
                          uint32_t mipLevels,
                          LPnuMetalTexture_t* texture);
void LPnuMetalTextureDestroy(LPnuMetalTexture_t texture);

int LPnuMetalSamplerCreate(LPnuMetalDevice_t device, 
                          LPnuMetalSampler_t* sampler);
void LPnuMetalSamplerDestroy(LPnuMetalSampler_t sampler);

int LPnuMetalCommandQueueCreate(LPnuMetalDevice_t device,
                              LPnuMetalCommandQueue_t* queue);
void LPnuMetalCommandQueueDestroy(LPnuMetalCommandQueue_t queue);

int LPnuMetalCommandBufferCreate(LPnuMetalCommandQueue_t queue,
                               LPnuMetalCommandBuffer_t* buffer);
void LPnuMetalCommandBufferDestroy(LPnuMetalCommandBuffer_t buffer);
void LPnuMetalCommandBufferCommit(LPnuMetalCommandBuffer_t buffer);
void LPnuMetalCommandBufferWait(LPnuMetalCommandBuffer_t buffer);

int LPnuMetalLibraryCreateFromSource(LPnuMetalDevice_t device,
                                 const char* source,
                                 LPnuMetalLibrary_t* library);
int LPnuMetalLibraryGetFunction(LPnuMetalLibrary_t library,
                            const char* name,
                            LPnuMetalFunction_t* function);

int LPnuMetalRenderPipelineCreate(LPnuMetalDevice_t device,
                                LPnuMetalLibrary_t library,
                                const char* vertexFunc,
                                const char* fragmentFunc,
                                LPnuMetalRenderPipeline_t* pipeline);
void LPnuMetalRenderPipelineDestroy(LPnuMetalRenderPipeline_t pipeline);
void LPnuMetalRenderPipelineSetBuffer(LPnuMetalRenderPipeline_t pipeline,
                                       uint32_t index,
                                       LPnuMetalBuffer_t buffer);
void LPnuMetalRenderPipelineSetTexture(LPnuMetalRenderPipeline_t pipeline,
                                    uint32_t index,
                                    LPnuMetalTexture_t texture);

int LPnuMetalComputePipelineCreate(LPnuMetalDevice_t device,
                               LPnuMetalFunction_t function,
                               LPnuMetalComputePipeline_t* pipeline);
void LPnuMetalComputePipelineDestroy(LPnuMetalComputePipeline_t pipeline);

/* Info */
const char* LPnuMetalGetDriverName(void);
const char* LPnuMetalGetDriverVersion(void);

#ifdef __cplusplus
}
#endif

#endif