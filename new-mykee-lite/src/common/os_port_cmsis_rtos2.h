/**
 * @file os_port_cmsis_rtos2.h
 * @brief RTOS abstraction layer (CMSIS-RTOS 2 / RTX v5)
 *
 * @section License
 *
 * Copyright (C) 2010-2021 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Ultimate.
 *
 * This software is provided under a commercial license. You may
 * use this software under the conditions stated in the license
 * terms. This source code cannot be redistributed.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 2.1.2
 **/

#ifndef _OS_PORT_CMSIS_RTOS2_H
#define _OS_PORT_CMSIS_RTOS2_H

//Dependencies
#include "cmsis_os2.h"

#ifdef RTE_CMSIS_RTOS2_RTX5
   #include "rtx_os.h"
#endif

#ifdef RTE_CMSIS_RTOS2_FreeRTOS
   #include "freertos.h"
#endif


//Use static or dynamic memory allocation for tasks
#ifndef OS_STATIC_TASK_SUPPORT
   #define OS_STATIC_TASK_SUPPORT DISABLED
#elif (OS_STATIC_TASK_SUPPORT != ENABLED && OS_STATIC_TASK_SUPPORT != DISABLED)
   #error OS_STATIC_TASK_SUPPORT parameter is not valid
#endif

//Invalid task identifier
#define OS_INVALID_TASK_ID NULL
//Self task identifier
#define OS_SELF_TASK_ID NULL

//Task priority (normal)
#ifndef OS_TASK_PRIORITY_NORMAL
   #define OS_TASK_PRIORITY_NORMAL osPriorityNormal
#endif

//Task priority (high)
#ifndef OS_TASK_PRIORITY_HIGH
   #define OS_TASK_PRIORITY_HIGH osPriorityAboveNormal
#endif

//Milliseconds to system ticks
#ifndef OS_MS_TO_SYSTICKS
   #define OS_MS_TO_SYSTICKS(n) (n)
#endif

//System ticks to milliseconds
#ifndef OS_SYSTICKS_TO_MS
   #define OS_SYSTICKS_TO_MS(n) (n)
#endif

//Task prologue
#define osEnterTask()
//Task epilogue
#define osExitTask()
//Interrupt service routine prologue
#define osEnterIsr()
//Interrupt service routine epilogue
#define osExitIsr(flag)

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Task identifier
 **/

typedef osThreadId_t OsTaskId;


/**
 * @brief Task control block
 **/

typedef struct
{
#if defined(os_CMSIS_RTX)
   os_thread_t cb;
#endif
#if defined(osRtxVersionKernel)
   osRtxThread_t cb;
#endif
#if defined(configSUPPORT_STATIC_ALLOCATION)
   StaticTask_t cb;
#endif
   uint64_t dummy;
} OsTaskTcb;


/**
 * @brief Stack data type
 **/

typedef uint32_t OsStackType;


/**
 * @brief Event object
 **/

typedef struct
{
   osEventFlagsId_t id;
#if defined(os_CMSIS_RTX)
   os_event_flags_t cb;
#endif
#if defined(osRtxVersionKernel)
   osRtxEventFlags_t cb;
#endif
} OsEvent;


/**
 * @brief Semaphore object
 **/

typedef struct
{
   osSemaphoreId_t id;
#if defined(os_CMSIS_RTX)
   os_semaphore_t cb;
#endif
#if defined(osRtxVersionKernel)
   osRtxSemaphore_t cb;
#endif
} OsSemaphore;


/**
 * @brief Mutex object
 **/

typedef struct
{
   osMutexId_t id;
#if defined(os_CMSIS_RTX)
   os_mutex_t cb;
#endif
#if defined(osRtxVersionKernel)
   osRtxMutex_t cb;
#endif
} OsMutex;


/**
 * @brief Task routine
 **/

typedef void (*OsTaskCode)(void *param);


//Kernel management
void osInitKernel(void);
void osStartKernel(void);

//Task management
OsTaskId osCreateTask(const char_t *name, OsTaskCode taskCode,
   void *param, size_t stackSize, int_t priority);

OsTaskId osCreateStaticTask(const char_t *name, OsTaskCode taskCode,
   void *param, OsTaskTcb *tcb, OsStackType *stack, size_t stackSize,
   int_t priority);

void osDeleteTask(OsTaskId taskId);
void osDelayTask(systime_t delay);
void osSwitchTask(void);
void osSuspendAllTasks(void);
void osResumeAllTasks(void);

//Event management
bool_t osCreateEvent(OsEvent *event);
void osDeleteEvent(OsEvent *event);
void osSetEvent(OsEvent *event);
void osResetEvent(OsEvent *event);
bool_t osWaitForEvent(OsEvent *event, systime_t timeout);
bool_t osSetEventFromIsr(OsEvent *event);

//Semaphore management
bool_t osCreateSemaphore(OsSemaphore *semaphore, uint_t count);
void osDeleteSemaphore(OsSemaphore *semaphore);
bool_t osWaitForSemaphore(OsSemaphore *semaphore, systime_t timeout);
void osReleaseSemaphore(OsSemaphore *semaphore);

//Mutex management
bool_t osCreateMutex(OsMutex *mutex);
void osDeleteMutex(OsMutex *mutex);
void osAcquireMutex(OsMutex *mutex);
void osReleaseMutex(OsMutex *mutex);

//System time
systime_t osGetSystemTime(void);

//Memory management
void *osAllocMem(size_t size);
void osFreeMem(void *p);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
