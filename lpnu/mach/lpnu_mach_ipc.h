/*
 * LPNU Mach IPC - Mach Ports Implementation
 * 
 * Mach IPC compatible layer for LPNU.
 * Based on XNU Mach but compatible with Linux.
 */

#ifndef _LPNU_MACH_IPC_H
#define _LPNU_MACH_IPC_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Mach types */
typedef uint32_t mach_port_t;
typedef uint32_t mach_msg_return_t;
typedef uint32_t kern_return_t;
typedef uint64_t mach_port_right_t;
typedef uint32_t mach_msg_type_name_t;

/* Port rights */
enum {
    MACH_PORT_RIGHT_NONE = 0,
    MACH_PORT_RIGHT_SEND = 1,
    MACH_PORT_RIGHT_RECEIVE = 2,
    MACH_PORT_RIGHT_SEND_ONCE = 3,
    MACH_PORT_RIGHT_PORT_SET = 4,
    MACH_PORT_RIGHT_DEAD_NAME = 5
};

/* Return codes */
enum {
    KERN_SUCCESS = 0,
    KERN_INVALID_ADDRESS = 1,
    KERN_PROTECTION_FAILURE = 2,
    KERN_NO_SPACE = 3,
    KERN_INVALID_TASK = 4,
    KERN_INVALID_RIGHT = 5,
    KERN_INVALID_VALUE = 6,
    KERN_RIGHT_EXISTS = 7,
    KERN_INVALID_CAPABILITY = 8,
    KERN_ALREADY_WAITING = 9,
    KERN_DEFAULT_HANDLER = 10,
    KERN_INVALID_REPLY = 11,
    KERN_INVALID_ARGUMENT = 12,
    KERN_UNAVAILABLE = 13,
    KERN_INVALID_OBJECT = 14,
    KERN_INVALID_LEADER = 15,
    KERN_NAME_EXISTS = 16,
    KERN_ABORTED = 17,
    KERN_INVALID_HOST = 20,
    KERN_MEMORY_ERROR = 21,
    KERN_NOT_SUPPORTED = 22,
    KERN_FAILURE = 33,
    KERN_RESOURCE_SHORTAGE = 34,
    KERN_INTERNAL_ERROR = 35,
    KERN_WTIMEOUT = 36,
    KERN_WALREADY = KERN_ALREADY_WAITING
};

/* Mach port flags */
enum {
    MACH_PORT_FLAG_NONE = 0,
    MACH_PORT_FLAG_IMPORTANCE = 0x01,
    MACH_PORT_FLAG_QOS = 0x02,
    MACH_PORT_FLAG_NO_PUBLISH = 0x10
};

/* Mach message */
typedef struct LPNUMachMsg {
    mach_port_t header;
    uint32_t size;
    void* data;
} LPNUMachMsg;

/* Port set */
typedef struct LPNUMachPortSet {
    mach_port_t port;
    mach_port_t* ports;
    uint32_t count;
    uint32_t capacity;
} LPNUMachPortSet;

/* IPC space */
typedef struct LPNUMachIPCSpace {
    uint32_t flags;
    void* task;
} LPNUMachIPCSpace;

/* Port allocation */
kern_return_t LPNUMachPortAlloc(mach_port_t* port, mach_port_right_t right);
kern_return_t LPNUMachPortDealloc(mach_port_t port);

/* Port sets */
kern_return_t LPNUMachPortSetCreate(LPNUMachPortSet** portSet);
void LPNUMachPortSetDestroy(LPNUMachPortSet* portSet);
kern_return_t LPNUMachPortSetAdd(LPNUMachPortSet* portSet, mach_port_t port);
kern_return_t LPNUMachPortSetRemove(LPNUMachPortSet* portSet, mach_port_t port);

/* Port insertion */
kern_return_t LPNUMachPortInsertRight(mach_port_t dest, mach_port_t port, void* obj, mach_msg_type_name_t type);

/* Message sending */
kern_return_t LPNUMachMsgSend(mach_port_t port, const void* data, uint32_t size);
kern_return_t LPNUMachMsgReceive(mach_port_t port, void* data, uint32_t* size, uint64_t timeout);

/* Task */
typedef void* LPNUMachTask;

kern_return_t LPNUMachTaskCreate(LPNUMachTask* task, uint32_t flags);
void LPNUMachTaskDestroy(LPNUMachTask task);
LPNUMachTask LPNUMachTaskSelf(void);

/* Thread */
typedef void* LPNUMachThread;

kern_return_t LPNUMachThreadCreate(LPNUMachThread* thread, LPNUMachTask task);
void LPNUMachThreadDestroy(LPNUMachThread thread);
LPNUMachThread LPNUMachThreadSelf(void);

/* Virtual memory */
kern_return_t LPNUMachVMAllocate(LPNUMachTask task, void** addr, uint64_t size);
kern_return_t LPNUMachVMDeallocate(LPNUMachTask task, void* addr, uint64_t size);
kern_return_t LPNUMachVMMap(LPNUMachTask task, void** addr, uint64_t size, void* memory, bool shared);

/* Semaphore */
typedef struct LPNUMachSemaphore {
    mach_port_t port;
    int count;
    void* lock;
} LPNUMachSemaphore;

kern_return_t LPNUMachSemaphoreCreate(LPNUMachSemaphore* sem);
void LPNUMachSemaphoreDestroy(LPNUMachSemaphore* sem);
kern_return_t LPNUMachSemaphoreSignal(LPNUMachSemaphore* sem);
kern_return_t LPNUMachSemaphoreWait(LPNUMachSemaphore* sem);
kern_return_t LPNUMachSemaphoreTimedWait(LPNUMachSemaphore* sem, uint64_t timeout);

/* Clock */
typedef void* LPNUMachClock;

kern_return_t LPNUMachClockCreate(LPNUMachClock* clock, uint64_t attributes);
void LPNUMachClockDestroy(LPNUMachClock clock);
kern_return_t LPNUMachClockGetTime(LPNUMachClock clock, uint64_t* time);
kern_return_t LPNUMachClockSetTime(LPNUMachClock clock, uint64_t time);

/* Version */
const char* LPNUMachGetVersion(void);

#ifdef __cplusplus
}
#endif

#endif