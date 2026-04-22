/*
 * LPNU IOKit - IOService Implementation
 * 
 * IOKit kernel framework for LPNU.
 * Based on XNU IOKit but compatible with Linux.
 */

#ifndef _LPNU_IOKIT_H
#define _LPNU_IOKIT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* IOKit types */
typedef int32_t IOReturn;
typedef uint32_t IOOptionBits;
typedef int32_t IOServiceNotificationToken;
typedef void* IOTimerData;
typedef void (*IOTimerCallback)(void* target, void* ref);

/* Return codes */
#define kIOReturnSuccess         0
#define kIOReturnError          -1
#define kIOReturnNoMemory      -2
#define kIOReturnNoResource    -3
#define kIOReturnIPCError      -4
#define kIOReturnNoDevice      -5
#define kIOReturnNotPrivileged -6
#define kIOReturnBadArgument   -7
#define kIOReturnLockedState  -8
#define kIOReturnNotOpen     -9
#define kIOReturnStateError  -10
#define kIOReturnOffline    -11
#define kIOReturnNoData    -12

/* IOService states */
enum {
    kIOService01State = 0x01,
    kIOService02State = 0x02,
    kIOService04State = 0x04,
    kIOService08State = 0x08,
    kIOService10State = 0x10,
    kIOService20State = 0x20,
    kIOService40State = 0x40,
    kIOService80State = 0x80,
    kIOServiceReady = kIOService40State
};

/* IOOptionBits */
enum {
    kIOFBOptionBitsSpeedMask = 0x00000003,
    kIOFBOptionBitsSpeedDefault = 0x00000000,
    kIOFBOptionBitsSpeedUnknown = 0x80000000
};

/* IOInterruptSource types */
enum {
    kIOInterruptTypeMask = 0x00000007,
    kIOInterruptTypeSimple = 0x00000001,
    kIOInterruptTypeLevel = 0x00000002,
    kIOInterruptTypeEdge = 0x00000003,
    kIOInterruptTypePCLevel = 0x00000004,
    kIOInterruptTypeTimer = 0x00000005
};

/* IOEventSource types */
enum {
    kIOEventSourceDefault = 0,
    kIOEventSourceTimer,
    kIOEventSourceInterrupt,
    kIOEventSourceCommand,
    kIOEventSourceData
};

/* IOMemoryDescriptor options */
enum {
    kIOMemoryDirectionIn = 0,
    kIOMemoryDirectionOut = 1,
    kIOMemoryDirectionInOut = 2
};

/* IOService matching */
enum {
    kIOServiceMatching = 1,
    kIOMatchingServiceName = 2,
    kIOMatchedPropertyName = 3
};

/* Forward declarations */
struct LPNUIOService;
struct LPNUIORegistryEntry;
struct LPNUIODictionary;
struct LPNUIODataQueue;

/* IOService base */
typedef struct LPNUIOService {
    const char* name;
    const char* className;
    uint32_t state;
    uint32_t refCount;
    struct LPNUIOService* parent;
    struct LPNUIOService* child;
    struct LPNUIOService* sibling;
    void* provider;
    void* userClient;
} LPNUIOService;

/* IORegistryEntry */
typedef struct LPNUIORegistryEntry {
    const char* name;
    const char* className;
    void* allocs[8];
    uint32_t allocCount;
} LPNUIORegistryEntry;

/* IODictionary */
typedef struct LPNUIODictionary {
    char** keys;
    void** values;
    uint32_t count;
    uint32_t capacity;
} LPNUIODictionary;

/* IODataQueue */
typedef struct LPNUIODataQueue {
    void* data;
    uint32_t size;
    uint32_t head;
    uint32_t tail;
    uint32_t capacity;
} LPNUIODataQueue;

/* IOService functions */
IOReturn LPNUIOServiceCreate(LPNUIOService** service, const char* name);
void LPNUIOServiceDestroy(LPNUIOService* service);

IOReturn LPNUIOServiceStart(LPNUIOService* service);
IOReturn LPNUIOServiceStop(LPNUIOService* service);
IOReturn LPNUIOServiceProbe(LPNUIOService* service, void* probeScore);

IOReturn LPNUIOServiceAddChild(LPNUIOService* parent, LPNUIOService* child);
IOReturn LPNUIOServiceRemoveChild(LPNUIOService* parent, LPNUIOService* child);

void* LPNUIOServiceGetProvider(LPNUIOService* service);
IOReturn LPNUIOServiceAddProvider(LPNUIOService* service, void* provider);
IOReturn LPNUIOServiceRemoveProvider(LPNUIOService* service, void* provider);

IOReturn LPNUIOServiceRegisterService(LPNUIOService* service);
IOReturn LPNUIOServiceMatch(LPNUIOService* service);

/* IORegistryEntry functions */
IOReturn LPNUIORegistryEntryCreate(LPNUIORegistryEntry** entry, const char* name);
void LPNUIORegistryEntryDestroy(LPNUIORegistryEntry* entry);
IOReturn LPNUIORegistryEntrySetProperty(LPNUIORegistryEntry* entry, const char* key, void* value);
void* LPNUIORegistryEntryCopyProperty(LPNUIORegistryEntry* entry, const char* key);

/* IODictionary functions */
IOReturn LPNUIODictionaryCreate(LPNUIODictionary** dict);
void LPNUIODictionaryDestroy(LPNUIODictionary* dict);
IOReturn LPNUIODictionarySet(LPNUIODictionary* dict, const char* key, void* value);
void* LPNUIODictionaryCopy(LPNUIODictionary* dict, const char* key);

/* IOTimer functions */
IOReturn LPNUIOTimerCreate(IOTimerData* timer);
IOReturn LPNUIOTimerSetTimeout(IOTimerData timer, uint64_t delay, IOTimerCallback callback, void* target, void* ref);
IOReturn LPNUIOTimerCancel(IOTimerData timer);

/* IODataQueue functions */
IOReturn LPNUIODataQueueCreate(LPNUIODataQueue** queue, uint32_t capacity);
void LPNUIODataQueueDestroy(LPNUIODataQueue* queue);
IOReturn LPNUIODataQueueEnqueue(LPNUIODataQueue* queue, void* data, uint32_t size);
IOReturn LPNUIODataQueueDequeue(LPNUIODataQueue* queue, void* data, uint32_t* size);
uint32_t LPNUIODataQueueGetSize(LPNUIODataQueue* queue);
uint32_t LPNUIODataQueueGetCapacity(LPNUIODataQueue* queue);
bool LPNUIODataQueueIsEmpty(LPNUIODataQueue* queue);
bool LPNUIODataQueueIsFull(LPNUIODataQueue* queue);

/* IOLock functions */
typedef void* LPNUIOLock;
IOReturn LPNUIOLockCreate(LPNUIOLock* lock);
void LPNUIOLockDestroy(LPNUIOLock lock);
void LPNUIOLockAcquire(LPNUIOLock lock);
bool LPNUIOLockTryAcquire(LPNUIOLock lock);
void LPNUIOLockRelease(LPNUIOLock lock);

/* IOSimpleLock functions */
typedef void* LPNUIOSimpleLock;
IOReturn LPNUIOSimpleLockCreate(LPNUIOSimpleLock* lock);
void LPNUIOSimpleLockDestroy(LPNUIOSimpleLock lock);
void LPNUIOSimpleLockAcquire(LPNUIOSimpleLock lock);
void LPNUIOSimpleLockRelease(LPNUIOSimpleLock lock);

/* IOInterruptEventSource functions */
typedef void (*IOInterruptHandler)(void* target, void* ref, uint32_t vector);
typedef void* LPNUIOInterruptEventSource;
IOReturn LPNUIOInterruptEventSourceCreate(LPNUIOInterruptEventSource* source, 
                                       void* controller,
                                       IOInterruptHandler handler,
                                       void* target,
                                       void* ref);
void LPNUIOInterruptEventSourceDestroy(LPNUIOInterruptEventSource source);
IOReturn LPNUIOInterruptEventSourceEnable(LPNUIOInterruptEventSource source);
IOReturn LPNUIOInterruptEventSourceDisable(LPNUIOInterruptEventSource source);

/* IOTimerEventSource functions */
typedef void (*IOTimerCallback)(void* target, void* ref);
typedef void* LPNUIOTimerEventSource;
IOReturn LPNUIOTimerEventSourceCreate(LPNUIOTimerEventSource* source,
                                         void* owner,
                                         IOTimerCallback callback,
                                         void* target);
void LPNUIOTimerEventSourceDestroy(LPNUIOTimerEventSource source);
IOReturn LPNUIOTimerEventSourceSetTimeout(LPNUIOTimerEventSource source, uint64_t delay);
IOReturn LPNUIOTimerEventSourceCancel(LPNUIOTimerEventSource source);
IOReturn LPNUIOTimerEventSourceArm(LPNUIOTimerEventSource source, uint64_t delay);
IOReturn LPNUIOTimerEventSourceDisarm(LPNUIOTimerEventSource source);

/* IOCommandGate functions */
typedef void* LPNUIOCommandGate;
IOReturn LPNUIOCommandGateCreate(LPNUIOCommandGate* gate, void* workLoop);
void LPNUIOCommandGateDestroy(LPNUIOCommandGate gate);
IOReturn LPNUIOCommandGateRun(LPNUIOCommandGate gate, void* function, void* arg);

/* IOMemoryDescriptor functions */
typedef struct {
    void* address;
    uint64_t length;
    uint32_t direction;
    void* pageRef;
} LPNUIOMemoryDescriptor;
IOReturn LPNUIOMemoryDescriptorCreate(LPNUIOMemoryDescriptor** desc,
                                          void* address,
                                          uint64_t length,
                                          uint32_t direction);
void LPNUIOMemoryDescriptorDestroy(LPNUIOMemoryDescriptor* desc);
void* LPNUIOMemoryDescriptorGetAddress(LPNUIOMemoryDescriptor* desc);
uint64_t LPNUIOMemoryDescriptorGetLength(LPNUIOMemoryDescriptor* desc);
IOReturn LPNUIOMemoryDescriptorMap(LPNUIOMemoryDescriptor* desc, void** map, uint64_t offset);

/* IOUserClient functions */
typedef void* LPNUIOUserClient;
IOReturn LPNUIOUserClientCreate(LPNUIOUserClient** client, void* service);
void LPNUIOUserClientDestroy(LPNUIOUserClient* client);
IOReturn LPNUIOUserClientConnect(LPNUIOUserClient* client, void* task);
IOReturn LPNUIOUserClientDisconnect(LPNUIOUserClient* client);
IOReturn LPNUIOUserClientSendScalar(LPNUIOUserClient* client, void* scalar, uint32_t count);
IOReturn LPNUIOUserClientSendData(LPNUIOUserClient* client, void* data, uint32_t size);
IOReturn LPNUIOUserClientCopyScalar(LPNUIOUserClient* client, void** scalar, uint32_t* count);
IOReturn LPNUIOUserClientCopyData(LPNUIOUserClient* client, void** data, uint32_t* size);

/* Initialization */
IOReturn LPNUIOKitInitialize(void);
const char* LPNUIOKitGetVersion(void);

#ifdef __cplusplus
}
#endif

#endif