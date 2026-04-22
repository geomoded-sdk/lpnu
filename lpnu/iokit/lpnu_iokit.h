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

/* IOEventSource */
typedef void (*IOEventSourceAction)(void* owner, void* target);
typedef void* LPNUIOEventSource;
IOReturn LPNUIOEventSourceCreate(LPNUIOEventSource* source, void* owner, IOEventSourceAction action, void* target);
void LPNUIOEventSourceDestroy(LPNUIOEventSource source);
IOReturn LPNUIOEventSourceEnable(LPNUIOEventSource source);
IOReturn LPNUIOEventSourceDisable(LPNUIOEventSource source);
bool LPNUIOEventSourceIsEnabled(LPNUIOEventSource source);

/* IOSharedLock */
typedef void* LPNUIOSharedLock;
IOReturn LPNUIOSharedLockCreate(LPNUIOSharedLock* lock);
void LPNUIOSharedLockDestroy(LPNUIOSharedLock lock);
void LPNUIOSharedLockAcquireShared(LPNUIOSharedLock lock);
void LPNUIOSharedLockAcquireExclusive(LPNUIOSharedLock lock);
void LPNUIOSharedLockRelease(LPNUIOSharedLock lock);
bool LPNUIOSharedLockTryAcquireShared(LPNUIOSharedLock lock);
bool LPNUIOSharedLockTryAcquireExclusive(LPNUIOSharedLock lock);

/* IODeviceMemory */
typedef struct LPNUIODeviceMemory {
    void* base;
    uint64_t length;
    uint64_t physical;
} LPNUIODeviceMemory;
IOReturn LPNUIODeviceMemoryCreate(LPNUIODeviceMemory** memory, void* base, uint64_t length);
void LPNUIODeviceMemoryDestroy(LPNUIODeviceMemory* memory);
void* LPNUIODeviceMemoryGetBase(LPNUIODeviceMemory* memory);
uint64_t LPNUIODeviceMemoryGetLength(LPNUIODeviceMemory* memory);
uint64_t LPNUIODeviceMemoryGetPhysicalAddress(LPNUIODeviceMemory* memory);

/* IODMACommand */
typedef void* LPNUIODMACommand;
typedef void (*IODMACommandAction)(void* ref, void* segment, uint32_t numSegments);
IOReturn LPNUIODMACommandCreate(LPNUIODMACommand* command, LPNUIODeviceMemory* memory, uint32_t options);
void LPNUIODMACommandDestroy(LPNUIODMACommand command);
IOReturn LPNUIODMACommandPrepare(LPNUIODMACommand command);
IOReturn LPNUIODMACommandExecute(LPNUIODMACommand command, IODMACommandAction action, void* ref);

/* IOServiceMatching */
IOReturn LPNUIOServiceMatching(LPNUIOService** service, const char* name);
IOReturn LPNUIOServiceGetMatchingService(LPNUIOService* matching);
IOReturn LPNUIOServiceGetMatchingServices(LPNUIOService** services, int* count);

/* IONotifier */
typedef void (*IONotifierFunction)(void* ref, LPNUIOService* service);
typedef void* LPNUIONotifier;
IOReturn LPNUIONotifierCreate(LPNUIONotifier* notifier, IONotifierFunction function, void* ref);
void LPNUIONotifierDestroy(LPNUIONotifier notifier);
IOReturn LPNUIONotifierAdd(LPNUIONotifier notifier, LPNUIONotifier* newNotifier);

/* IODeviceTree */
IOReturn LPNUIODeviceTreeCreate(void** tree);
void LPNUIODeviceTreeDestroy(void* tree);
IOReturn LPNUIODeviceTreeAddChild(void* parent, const char* name, void** child);
IOReturn LPNUIODeviceTreeSetProperty(void* node, const char* name, void* data, uint32_t length);
IOReturn LPNUIODeviceTreeGetProperty(void* node, const char* name, void** data, uint32_t* length);

/* IOPMLegacy */
enum {
    kIOPMDevicePowerStateOn = 0,
    kIOPMDevicePowerStateOff = 1,
    kIOPMDevicePowerStateIdle = 2,
    kIOPMDevicePowerStateSleep = 3
};
typedef void* LPNUIOPMConnection;
IOReturn LPNUIOPMConnectionCreate(LPNUIOPMConnection* connection, const char* name);
void LPNUIOPMConnectionDestroy(LPNUIOPMConnection connection);
IOReturn LPNUIOPMConnectionSetPowerState(LPNUIOPMConnection connection, uint32_t state);
IOReturn LPNUIOPMConnectionRegisterInterest(LPNUIOPMConnection connection, const char* notification, void* ref, IONotifierFunction callback);

/* IOWorkLoop */
typedef struct LPNUIOWorkLoop {
    void* eventSource;
    void* gate;
    uint32_t state;
} LPNUIOWorkLoop;
IOReturn LPNUIOWorkLoopCreate(LPNUIOWorkLoop** workLoop);
void LPNUIOWorkLoopDestroy(LPNUIOWorkLoop* workLoop);
IOReturn LPNUIOWorkLoopAddEventSource(LPNUIOWorkLoop* workLoop, LPNUIOEventSource* source);
IOReturn LPNUIOWorkLoopRemoveEventSource(LPNUIOWorkLoop* workLoop, LPNUIOEventSource* source);
IOReturn LPNUIOWorkLoopRun(LPNUIOWorkLoop* workLoop, void* function, void* arg);
IOReturn LPNUIOWorkLoopWakeup(LPNUIOWorkLoop* workLoop);

/* IOBufferMemoryDescriptor */
typedef struct LPNUIOBufferMemoryDescriptor {
    void* buffer;
    uint64_t length;
    uint32_t options;
    void* map;
} LPNUIOBufferMemoryDescriptor;
IOReturn LPNUIOBufferMemoryDescriptorCreate(LPNUIOBufferMemoryDescriptor** desc, uint64_t length, uint32_t options);
void LPNUIOBufferMemoryDescriptorDestroy(LPNUIOBufferMemoryDescriptor* desc);
void* LPNUIOBufferMemoryDescriptorGetBuffer(LPNUIOBufferMemoryDescriptor* desc);
uint64_t LPNUIOBufferMemoryDescriptorGetLength(LPNUIOBufferMemoryDescriptor* desc);
IOReturn LPNUIOBufferMemoryDescriptorPrepare(LPNUIOBufferMemoryDescriptor* desc);
IOReturn LPNUIOBufferMemoryDescriptorComplete(LPNUIOBufferMemoryDescriptor* desc);

/* IOConditionLock */
typedef void* LPNUIOConditionLock;
IOReturn LPNUIOConditionLockCreate(LPNUIOConditionLock* lock, uint32_t value);
void LPNUIOConditionLockDestroy(LPNUIOConditionLock lock);
IOReturn LPNUIOConditionLockAcquire(LPNUIOConditionLock lock, uint32_t timeout);
IOReturn LPNUIOConditionLockRelease(LPNUIOConditionLock lock, uint32_t value);
bool LPNUIOConditionLockTryAcquire(LPNUIOConditionLock lock, uint32_t value);
uint32_t LPNUIOConditionLockGetValue(LPNUIOConditionLock lock);

/* IOCommandPool */
typedef struct LPNUIOCommandPool {
    void* commands;
    uint32_t count;
    uint32_t size;
} LPNUIOCommandPool;
IOReturn LPNUIOCommandPoolCreate(LPNUIOCommandPool** pool, uint32_t count, uint32_t size);
void LPNUIOCommandPoolDestroy(LPNUIOCommandPool* pool);
void* LPNUIOCommandPoolAllocate(LPNUIOCommandPool* pool);
IOReturn LPNUIOCommandPoolRelease(LPNUIOCommandPool* pool, void* command);

/* IOCommandQueue */
typedef struct LPNUIOCommandQueue {
    void* queue;
    void* gate;
    uint32_t count;
} LPNUIOCommandQueue;
IOReturn LPNUIOCommandQueueCreate(LPNUIOCommandQueue** queue, void* workLoop);
void LPNUIOCommandQueueDestroy(LPNUIOCommandQueue* queue);
IOReturn LPNUIOCommandQueueEnqueue(LPNUIOCommandQueue* queue, void* command);
void* LPNUIOCommandQueueDequeue(LPNUIOCommandQueue* queue);
bool LPNUIOCommandQueueIsEmpty(LPNUIOCommandQueue* queue);
uint32_t LPNUIOCommandQueueGetCount(LPNUIOCommandQueue* queue);

/* IOService additions - More utilities */
IOReturn LPNUIOServiceAddMatchingDictionary(LPNUIOService* service, const char* key, void* value);
IOReturn LPNUIOServiceGetPropertyTable(LPNUIOService* service, void** table);
IOReturn LPNUIOServiceGetParentIterator(LPNUIOService* service, void** iterator);
IOReturn LPNUIOServiceGetChildIterator(LPNUIOService* service, void** iterator);
void* LPNUIOServiceIterateOverChildren(LPNUIOService* service, void* iterator);
void* LPNUIOServiceIterateOverParents(LPNUIOService* service, void* iterator);

/* IOSharedDataQueue */
typedef void* LPNUIOSharedDataQueue;
IOReturn LPNUIOSharedDataQueueCreate(LPNUIOSharedDataQueue** queue, const char* name, uint32_t size);
void LPNUIOSharedDataQueueDestroy(LPNUIOSharedDataQueue queue);
IOReturn LPNUIOSharedDataQueueEnqueue(LPNUIOSharedDataQueue queue, void* data, uint32_t size);
bool LPNUIOSharedDataQueueDequeue(LPNUIOSharedDataQueue queue, void* data, uint32_t* size);
uint32_t LPNUIOSharedDataQueueGetSize(LPNUIOSharedDataQueue queue);
uint32_t LPNUIOSharedDataQueueGetCapacity(LPNUIOSharedDataQueue queue);

/* Initialization */
IOReturn LPNUIOKitInitialize(void);
const char* LPNUIOKitGetVersion(void);

#ifdef __cplusplus
}
#endif

#endif