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

/* IOFilterInterruptEventSource */
typedef void* LPNUIOFilterInterruptEventSource;
typedef bool (*IOFilterInterruptFilter)(void* target, void* ref, uint32_t vector);
IOReturn LPNUIOFilterInterruptEventSourceCreate(LPNUIOFilterInterruptEventSource* source,
                                               void* controller,
                                               IOFilterInterruptFilter filter,
                                               IOInterruptHandler handler,
                                               void* target,
                                               void* ref);
void LPNUIOFilterInterruptEventSourceDestroy(LPNUIOFilterInterruptEventSource source);
IOReturn LPNUIOFilterInterruptEventSourceEnable(LPNUIOFilterInterruptEventSource source);
IOReturn LPNUIOFilterInterruptEventSourceDisable(LPNUIOFilterInterruptEventSource source);

/* IODMAEventSource */
typedef void* LPNUIODMAEventSource;
typedef void (*IODMAAction)(void* target, void* ref, void* segment, uint32_t numSegments);
IOReturn LPNUIODMAEventSourceCreate(LPNUIODMAEventSource* source,
                                    void* controller,
                                    IODMAAction action,
                                    void* target);
void LPNUIODMAEventSourceDestroy(LPNUIODMAEventSource source);
IOReturn LPNUIODMAEventSourceStart(LPNUIODMAEventSource source);
IOReturn LPNUIODMAEventSourceStop(LPNUIODMAEventSource source);
IOReturn LPNUIODMAEventSourceExecute(LPNUIODMAEventSource source, void* dmacommand, IODMAAction callback, void* target);

/* IOInterruptController */
typedef void* LPNUIOInterruptController;
IOReturn LPNUIOInterruptControllerCreate(LPNUIOInterruptController* controller, const char* name);
void LPNUIOInterruptControllerDestroy(LPNUIOInterruptController controller);
IOReturn LPNUIOInterruptControllerAddVector(LPNUIOInterruptController controller, uint32_t vector, IOInterruptHandler handler, void* target);
IOReturn LPNUIOInterruptControllerEnable(LPNUIOInterruptController controller, uint32_t vector);
IOReturn LPNUIOInterruptControllerDisable(LPNUIOInterruptController controller, uint32_t vector);
IOReturn LPNUIOInterruptControllerComplete(LPNUIOInterruptController controller, uint32_t vector);

/* IOInterruptAccounting */
typedef void* LPNUIOInterruptAccounting;
IOReturn LPNUIOInterruptAccountingCreate(LPNUIOInterruptAccounting** accounting);
void LPNUIOInterruptAccountingDestroy(LPNUIOInterruptAccounting accounting);
IOReturn LPNUIOInterruptAccountingSetEnabled(LPNUIOInterruptAccounting accounting, bool enabled);
bool LPNUIOInterruptAccountingGetEnabled(LPNUIOInterruptAccounting accounting);
IOReturn LPNUIOInterruptAccountingCountInterrupt(LPNUIOInterruptAccounting accounting, uint32_t vector);
uint32_t LPNUIOInterruptAccountingGetCount(LPNUIOInterruptAccounting accounting, uint32_t vector);

/* IOMapper */
typedef void* LPNUIOMapper;
IOReturn LPNUIOMapperCreate(LPNUIOMapper* mapper);
void LPNUIOMapperDestroy(LPNUIOMapper mapper);
IOReturn LPNUIOMapperMapMemory(LPNUIOMapper mapper, void* address, uint64_t length, void** map);
IOReturn LPNUIOMapperUnmapMemory(LPNUIOMapper mapper, void* map);
void* LPNUIOMapperGetAddress(LPNUIOMapper mapper, void* map);

/* IOMemoryCursor */
typedef void* LPNUIOMemoryCursor;
IOReturn LPNUIOMemoryCursorCreate(LPNUIOMemoryCursor* cursor, uint64_t alignment);
void LPNUIOMemoryCursorDestroy(LPNUIOMemoryCursor cursor);
IOReturn LPNUIOMemoryCursorGetPhysical(LPNUIOMemoryCursor cursor, void* segment, void** physAddr, uint64_t* length);

/* IOMultiMemoryDescriptor */
typedef void* LPNUIOMultiMemoryDescriptor;
IOReturn LPNUIOMultiMemoryDescriptorCreate(LPNUIOMultiMemoryDescriptor** desc, uint32_t numDescriptors);
void LPNUIOMultiMemoryDescriptorDestroy(LPNUIOMultiMemoryDescriptor desc);
IOReturn LPNUIOMultiMemoryDescriptorSetDescriptor(LPNUIOMultiMemoryDescriptor desc, uint32_t index, void* address, uint64_t length, uint32_t direction);

/* IOInterleavedMemoryDescriptor */
typedef void* LPNUIOInterleavedMemoryDescriptor;
IOReturn LPNUIOInterleavedMemoryDescriptorCreate(LPNUIOInterleavedMemoryDescriptor** desc, uint32_t numSegments);
void LPNUIOInterleavedMemoryDescriptorDestroy(LPNUIOInterleavedMemoryDescriptor desc);
IOReturn LPNUIOInterleavedMemoryDescriptorCopyFrom(LPNUIOInterleavedMemoryDescriptor desc, void* src, uint64_t length);
IOReturn LPNUIOInterleavedMemoryDescriptorCopyTo(LPNUIOInterleavedMemoryDescriptor desc, void* dst, uint64_t length);

/* IOSubMemoryDescriptor */
typedef void* LPNUIOSubMemoryDescriptor;
IOReturn LPNUIOSubMemoryDescriptorCreate(LPNUIOSubMemoryDescriptor** desc, void* parent, uint64_t offset, uint64_t length);
void LPNUIOSubMemoryDescriptorDestroy(LPNUIOSubMemoryDescriptor desc);
uint64_t LPNUIOSubMemoryDescriptorGetOffset(LPNUIOSubMemoryDescriptor desc);

/* IOGuardPageMemoryDescriptor */
typedef void* LPNUIOGuardPageMemoryDescriptor;
IOReturn LPNUIOGuardPageMemoryDescriptorCreate(LPNUIOGuardPageMemoryDescriptor** desc, uint64_t length);
void LPNUIOGuardPageMemoryDescriptorDestroy(LPNUIOGuardPageMemoryDescriptor desc);
IOReturn LPNUIOGuardPageMemoryDescriptorAllocate(LPNUIOGuardPageMemoryDescriptor desc);
IOReturn LPNUIOGuardPageMemoryDescriptorDeallocate(LPNUIOGuardPageMemoryDescriptor desc);

/* IOReportTypes */
typedef void* LPNUIOReportType;
typedef void* LPNUIOReportChannel;
IOReturn LPNUIOReportTypeCreate(LPNUIOReportType* type, uint32_t id, const char* name, uint32_t dataSize);
void LPNUIOReportTypeDestroy(LPNUIOReportType type);
IOReturn LPNUIOReportChannelCreate(LPNUIOReportChannel* channel, LPNUIOReportType type);
void LPNUIOReportChannelDestroy(LPNUIOReportChannel channel);
IOReturn LPNUIOReportChannelSubmit(LPNUIOReportChannel channel, const void* data, uint64_t timestamp);

/* IOStatistics */
typedef void* LPNUIOStatistics;
typedef void* LPNUIOStatisticsEntry;
IOReturn LPNUIOStatisticsCreate(LPNUIOStatistics* stats);
void LPNUIOStatisticsDestroy(LPNUIOStatistics stats);
IOReturn LPNUIOStatisticsIncrement(LPNUIOStatistics stats, const char* name, int64_t delta);
IOReturn LPNUIOStatisticsGetValue(LPNUIOStatistics stats, const char* name, int64_t* value);
IOReturn LPNUIOStatisticsCreateEntry(LPNUIOStatistics stats, LPNUIOStatisticsEntry* entry, const char* name);

/* IOCatalogue */
typedef void* LPNUIOCatalogue;
IOReturn LPNUIOCatalogueCreate(LPNUIOCatalogue* catalogue);
void LPNUIOCatalogueDestroy(LPNUIOCatalogue catalogue);
IOReturn LPNUIOCatalogueAddModule(LPNUIOCatalogue catalogue, const char* name, void* data, uint32_t length);
IOReturn LPNUIOCatalogueGetModule(LPNUIOCatalogue catalogue, const char* name, void** data, uint32_t* length);
IOReturn LPNUIOCatalogueRemoveModule(LPNUIOCatalogue catalogue, const char* name);
uint32_t LPNUIOCatalogueGetCount(LPNUIOCatalogue catalogue);

/* IOPlatformExpert */
typedef void* LPNUIOPlatformExpert;
IOReturn LPNUIOPlatformExpertCreate(LPNUIOPlatformExpert** expert);
void LPNUIOPlatformExpertDestroy(LPNUIOPlatformExpert expert);
IOReturn LPNUIOPlatformExpertGetMachineID(LPNUIOPlatformExpert expert, void* id);
IOReturn LPNUIOPlatformExpertGetPlatformName(LPNUIOPlatformExpert expert, char* name, uint32_t maxLength);

/* IOPlatformActions */
typedef void* LPNUIOPlatformAction;
typedef void (*IOPlatformActionCallback)(void* ref);
IOReturn LPNUIOPlatformActionCreate(LPNUIOPlatformAction* action, IOPlatformActionCallback callback, void* ref);
void LPNUIOPlatformActionDestroy(LPNUIOPlatformAction action);
IOReturn LPNUIOPlatformActionPerform(LPNUIOPlatformAction action);
IOReturn LPNUIOPlatformActionSetTimeout(LPNUIOPlatformAction action, uint64_t timeout);

/* IODeviceTreeSupport */
typedef void* LPNUIODeviceTreeNode;
IOReturn LPNUIODeviceTreeNodeCreate(LPNUIODeviceTreeNode* node);
void LPNUIODeviceTreeNodeDestroy(LPNUIODeviceTreeNode node);
IOReturn LPNUIODeviceTreeNodeAddChild(LPNUIODeviceTreeNode parent, const char* name, LPNUIODeviceTreeNode* child);
IOReturn LPNUIODeviceTreeNodeSetProperty(LPNUIODeviceTreeNode node, const char* name, const void* data, uint32_t length);
IOReturn LPNUIODeviceTreeNodeGetProperty(LPNUIODeviceTreeNode node, const char* name, void** data, uint32_t* length);
IOReturn LPNUIODeviceTreeNodeGetChild(LPNUIODeviceTreeNode node, uint32_t index, LPNUIODeviceTreeNode* child);
uint32_t LPNUIODeviceTreeNodeGetChildCount(LPNUIODeviceTreeNode node);

/* IONVRAM */
typedef void* LPNUIONVRAM;
IOReturn LPNUIONVRAMOpen(LPNUIONVRAM* nvram);
void LPNUIONVRAMClose(LPNUIONVRAM nvram);
IOReturn LPNUIONVRAMRead(LPNUIONVRAM nvram, const char* key, void* data, uint32_t* length);
IOReturn LPNUIONVRAMWrite(LPNUIONVRAM nvram, const char* key, const void* data, uint32_t length);
IOReturn LPNUIONVRAMErase(LPNUIONVRAM nvram, const char* key);
IOReturn LPNUIONVRAMSync(LPNUIONVRAM nvram);

/* IOSyncer */
typedef void* LPNUIOSyncer;
IOReturn LPNUIOSyncerCreate(LPNUIOSyncer* syncer);
void LPNUIOSyncerDestroy(LPNUIOSyncer syncer);
IOReturn LPNUIOSyncerArm(LPNUIOSyncer syncer);
IOReturn LPNUIOSyncerWait(LPNUIOSyncer syncer, uint64_t timeout);
IOReturn LPNUIOSyncerCancel(LPNUIOSyncer syncer);

/* IOInterrupts */
typedef void* LPNUIOInterrupt;
IOReturn LPNUIOInterruptAttach(LPNUIOInterrupt* interrupt, void* controller, uint32_t vector, IOInterruptHandler handler, void* target);
IOReturn LPNUIOInterruptDetach(LPNUIOInterrupt interrupt);
IOReturn LPNUIOInterruptEnable(LPNUIOInterrupt interrupt);
IOReturn LPNUIOInterruptDisable(LPNUIOInterrupt interrupt);
IOReturn LPNUIOInterruptSetEnabled(LPNUIOInterrupt interrupt, bool enabled);

/* IOServicePM - Power Management */
typedef void* LPNUIOServicePM;
typedef void (*IOPowerCallback)(void* ref, uint32_t oldState, uint32_t newState);
IOReturn LPNUIOServicePMCreate(LPNUIOServicePM* pm, void* service);
void LPNUIOServicePMDestroy(LPNUIOServicePM pm);
IOReturn LPNUIOServicePMSetPowerState(LPNUIOServicePM pm, uint32_t state);
IOReturn LPNUIOServicePMGetPowerState(LPNUIOServicePM pm, uint32_t* state);
IOReturn LPNUIOServicePMRegisterCallback(LPNUIOServicePM pm, IOPowerCallback callback, void* ref);
IOReturn LPNUIOServicePMRequestState(LPNUIOServicePM pm, uint32_t state);

/* IOUserServer */
typedef void* LPNUIOUserServer;
IOReturn LPNUIOUserServerCreate(LPNUIOUserServer* server, const char* name);
void LPNUIOUserServerDestroy(LPNUIOUserServer server);
IOReturn LPNUIOUserServerStart(LPNUIOUserServer server);
IOReturn LPNUIOUserServerStop(LPNUIOUserServer server);
IOReturn LPNUIOUserServerAddClient(LPNUIOUserServer server, void* client);

/* IOExtensiblePaniclog */
typedef void* LPNUIOExtensiblePaniclog;
IOReturn LPNUIOExtensiblePaniclogOpen(LPNUIOExtensiblePaniclog* log);
void LPNUIOExtensiblePaniclogClose(LPNUIOExtensiblePaniclog log);
IOReturn LPNUIOExtensiblePaniclogWrite(LPNUIOExtensiblePaniclog log, const void* data, uint32_t length);
IOReturn LPNUIOExtensiblePaniclogRead(LPNUIOExtensiblePaniclog log, void* data, uint32_t* length);
uint32_t LPNUIOExtensiblePaniclogGetSize(LPNUIOExtensiblePaniclog log);

/* IOPolledInterface */
typedef void* LPNUIOPolledInterface;
typedef void (*IOPolledIOHandler)(void* ref, void* sender, uint32_t vector);
IOReturn LPNUIOPolledInterfaceCreate(LPNUIOPolledInterface* interface);
void LPNUIOPolledInterfaceDestroy(LPNUIOPolledInterface interface);
IOReturn LPNUIOPolledInterfaceSetHandler(LPNUIOPolledInterface interface, IOPolledIOHandler handler, void* ref);
IOReturn LPNUIOPolledInterfacePoll(LPNUIOPolledInterface interface, uint32_t vector);

/* IOKernelReporter */
typedef void* LPNUIOKernelReporter;
typedef void (*IOKernelReportCallback)(void* ref, const void* data, uint32_t length);
IOReturn LPNUIOKernelReporterCreate(LPNUIOKernelReporter* reporter, uint32_t type, uint32_t size);
void LPNUIOKernelReporterDestroy(LPNUIOKernelReporter reporter);
IOReturn LPNUIOKernelReporterSubmit(LPNUIOKernelReporter reporter, const void* data, uint32_t length);
IOReturn LPNUIOKernelReporterSetCallback(LPNUIOKernelReporter reporter, IOKernelReportCallback callback, void* ref);

/* IODeviceTree - Additional */
typedef void* LPNUIODeviceTreeIterator;
IOReturn LPNUIODeviceTreeIteratorCreate(LPNUIODeviceTreeIterator* iter, void* root);
void LPNUIODeviceTreeIteratorDestroy(LPNUIODeviceTreeIterator iter);
IOReturn LPNUIODeviceTreeIteratorEnter(LPNUIODeviceTreeIterator iter, void* node);
IOReturn LPNUIODeviceTreeIteratorExit(LPNUIODeviceTreeIterator iter);
void* LPNUIODeviceTreeIteratorCurrent(LPNUIODeviceTreeIterator iter);
bool LPNUIODeviceTreeIteratorIsValid(LPNUIODeviceTreeIterator iter);
IOReturn LPNUIODeviceTreeIteratorNext(LPNUIODeviceTreeIterator iter);

/* IOTimeStamp */
typedef void* LPNUIOTimeStamp;
IOReturn LPNUIOTimeStampCreate(LPNUIOTimeStamp* ts);
void LPNUIOTimeStampDestroy(LPNUIOTimeStamp ts);
IOReturn LPNUIOTimeStampNow(LPNUIOTimeStamp ts, uint64_t* timestamp);
IOReturn LPNUIOTimeStampGetDelta(LPNUIOTimeStamp ts1, LPNUIOTimeStamp ts2, uint64_t* delta);
IOReturn LPNUIOTimeStampToAbsTime(LPNUIOTimeStamp ts, uint64_t absTime);

/* IODeviceTree - Properties */
typedef void* LPNUIODeviceTreeProperty;
IOReturn LPNUIODeviceTreePropertyCreate(LPNUIODeviceTreeProperty* prop, const char* name, const void* data, uint32_t length);
void LPNUIODeviceTreePropertyDestroy(LPNUIODeviceTreeProperty prop);
const char* LPNUIODeviceTreePropertyGetName(LPNUIODeviceTreeProperty prop);
void* LPNUIODeviceTreePropertyGetData(LPNUIODeviceTreeProperty prop);
uint32_t LPNUIODeviceTreePropertyGetLength(LPNUIODeviceTreeProperty prop);

/* IORangeAllocator */
typedef void* LPNUIORangeAllocator;
typedef struct { uint64_t start; uint64_t end; } LPNUIOAddressRange;
IOReturn LPNUIORangeAllocatorCreate(LPNUIORangeAllocator* allocator, uint64_t base, uint64_t length);
void LPNUIORangeAllocatorDestroy(LPNUIORangeAllocator allocator);
IOReturn LPNUIORangeAllocatorAllocate(LPNUIORangeAllocator allocator, uint64_t length, uint64_t alignment, LPNUIOAddressRange* range);
IOReturn LPNUIORangeAllocatorFree(LPNUIORangeAllocator allocator, LPNUIOAddressRange range);
bool LPNUIORangeAllocatorContains(LPNUIORangeAllocator allocator, uint64_t address);

/* IOServiceStateNotificationEventSource */
typedef void* LPNUIOServiceStateNotificationEventSource;
IOReturn LPNUIOServiceStateNotificationEventSourceCreate(LPNUIOServiceStateNotificationEventSource* source,
                                                     void* service,
                                                     IOEventSourceAction action,
                                                     void* target);
void LPNUIOServiceStateNotificationEventSourceDestroy(LPNUIOServiceStateNotificationEventSource source);
IOReturn LPNUIOServiceStateNotificationEventSourceSetState(LPNUIOServiceStateNotificationEventSource source, uint32_t state);
uint32_t LPNUIOServiceStateNotificationEventSourceGetState(LPNUIOServiceStateNotificationEventSource source);

/* IOServiceMatching - More utilities */
typedef void* LPNUIOServiceMatchingDict;
IOReturn LPNUIOServiceMatchingDictCreate(LPNUIOServiceMatchingDict* dict);
void LPNUIOServiceMatchingDictDestroy(LPNUIOServiceMatchingDict dict);
IOReturn LPNUIOServiceMatchingDictSetProperty(LPNUIOServiceMatchingDict dict, const char* key, const void* value, uint32_t length);
IOReturn LPNUIOServiceMatchingDictSetString(LPNUIOServiceMatchingDict dict, const char* key, const char* value);
IOReturn LPNUIOServiceMatchingDictSetNumber(LPNUIOServiceMatchingDict dict, const char* key, uint64_t value);
IOReturn LPNUIOServiceMatchingDictGetProperty(LPNUIOServiceMatchingDict dict, const char* key, void** value, uint32_t* length);
const char* LPNUIOServiceMatchingDictGetString(LPNUIOServiceMatchingDict dict, const char* key);
uint64_t LPNUIOServiceMatchingDictGetNumber(LPNUIOServiceMatchingDict dict, const char* key);
bool LPNUIOServiceMatchingDictHasProperty(LPNUIOServiceMatchingDict dict, const char* key);

/* IOEventSource - More utils */
typedef struct {
    void* owner;
    IOEventSourceAction action;
    void* target;
    bool enabled;
    uint32_t checkStamp;
} LPNUIOEventSourceData;
IOReturn LPNUIOEventSourceSetCheckInterval(LPNUIOEventSource source, uint64_t interval);
uint64_t LPNUIOEventSourceGetCheckInterval(LPNUIOEventSource source);
IOReturn LPNUIOEventSourceSetMaxEventsPerRun(LPNUIOEventSource source, uint32_t maxEvents);
uint32_t LPNUIOEventSourceGetMaxEventsPerRun(LPNUIOEventSource source);
bool LPNUIOEventSourceIsAWOL(LPNUIOEventSource source);

/* Initialization */
IOReturn LPNUIOKitInitialize(void);
const char* LPNUIOKitGetVersion(void);

#ifdef __cplusplus
}
#endif

#endif