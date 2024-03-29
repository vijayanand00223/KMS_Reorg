/**
 * @file os_port_freertos.h
 * @brief RTOS abstraction layer (FreeRTOS)
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

#ifndef _OS_PORT_FREERTOS_H
#define _OS_PORT_FREERTOS_H

//Dependencies
#ifdef IDF_VER
   #include "freertos/FreeRTOS.h"
   #include "freertos/task.h"
   #include "freertos/semphr.h"
#else
   #include "FreeRTOS.h"
   #include "task.h"
   #include "semphr.h"
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
   #define OS_TASK_PRIORITY_NORMAL (tskIDLE_PRIORITY + 1)
#endif

//Task priority (high)
#ifndef OS_TASK_PRIORITY_HIGH
   #define OS_TASK_PRIORITY_HIGH (tskIDLE_PRIORITY + 2)
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
#ifndef osEnterTask
   #define osEnterTask()
#endif

//Task epilogue
#ifndef osExitTask
   #define osExitTask()
#endif

//Interrupt service routine prologue
#ifndef osEnterIsr
   #if defined(portENTER_SWITCHING_ISR)
      #define osEnterIsr() portENTER_SWITCHING_ISR()
   #else
      #define osEnterIsr()
   #endif
#endif

//Interrupt service routine epilogue
#ifndef osExitIsr
   #if defined(__XTENSA__)
      #define osExitIsr(flag) if(flag) portYIELD_FROM_ISR()
   #elif defined(portEXIT_SWITCHING_ISR)
      #define osExitIsr(flag) portEXIT_SWITCHING_ISR()
   #elif defined(portEND_SWITCHING_ISR)
      #define osExitIsr(flag) portEND_SWITCHING_ISR(flag)
   #elif defined(portYIELD_FROM_ISR)
      #define osExitIsr(flag) portYIELD_FROM_ISR(flag)
   #else
      #define osExitIsr(flag)
   #endif
#endif

//Static object allocation
#ifndef configSUPPORT_STATIC_ALLOCATION
   #define configSUPPORT_STATIC_ALLOCATION 0
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Task identifier
 **/

typedef TaskHandle_t OsTaskId;


/**
 * @brief Task control block
 **/

typedef StaticTask_t OsTaskTcb;


/**
 * @brief Stack data type
 **/

typedef StackType_t OsStackType;


/**
 * @brief Event object
 **/

typedef struct
{
   SemaphoreHandle_t handle;
#if (configSUPPORT_STATIC_ALLOCATION == 1)
   StaticSemaphore_t buffer;
#endif
} OsEvent;


/**
 * @brief Semaphore object
 **/

typedef struct
{
   SemaphoreHandle_t handle;
#if (configSUPPORT_STATIC_ALLOCATION == 1)
   StaticSemaphore_t buffer;
#endif
} OsSemaphore;


/**
 * @brief Mutex object
 **/

typedef struct
{
   SemaphoreHandle_t handle;
#if (configSUPPORT_STATIC_ALLOCATION == 1)
   StaticSemaphore_t buffer;
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
