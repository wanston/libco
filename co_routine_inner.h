/*
* Tencent is pleased to support the open source community by making Libco available.

* Copyright (C) 2014 THL A29 Limited, a Tencent company. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License"); 
* you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, 
* software distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and 
* limitations under the License.
*/


#ifndef __CO_ROUTINE_INNER_H__

#include "co_routine.h"
#include "coctx.h"
struct stCoRoutineEnv_t;
struct stCoSpec_t
{
	void *value;
};

struct stStackMem_t
{
	stCoRoutine_t* occupy_co; // 指向正在占用此栈的协程
	int stack_size;
	char* stack_bp; //stack_buffer + stack_size
	char* stack_buffer; // 指向堆上的地址

};

struct stShareStack_t
{
	unsigned int alloc_idx; // 一个计数，表示分配共享栈的次数
	int stack_size; // 需求的栈大小
	int count; // 协程栈的数目
	stStackMem_t** stack_array; // 所有的协程栈
};



struct stCoRoutine_t
{
	stCoRoutineEnv_t *env; // 环境，每个线程一份
	pfn_co_routine_t pfn; // 协程函数
	void *arg; // 函数参数
	coctx_t ctx; // 协程上下文

	char cStart; // 是否已经开始执行协程函数
	char cEnd; // 是否已经结束执行协程函数
	char cIsMain; // 是否为主协程
	char cEnableSysHook; // 是否允许hook系统调用
	char cIsShareStack;

	void *pvEnv;

	//char sRunStack[ 1024 * 128 ];
	stStackMem_t* stack_mem; // 指向协程栈，在调用co_create_env时被赋值。
	// 其来源有两种，要么是由co_alloc_stackmem()从堆上分配；要么是由co_get_stackmem()从现有的协程栈中取一个作为共享栈。


	//save satck buffer while confilct on same stack_buffer;
	char* stack_sp; // 在co_swap中使用，用于记录栈顶，从而可以确定要保存的栈的大小
	unsigned int save_size;
	char* save_buffer; // 保存的栈的内容

	stCoSpec_t aSpec[1024];

};



//1.env
void 				co_init_curr_thread_env();
stCoRoutineEnv_t *	co_get_curr_thread_env();

//2.coroutine
void    co_free( stCoRoutine_t * co );
void    co_yield_env(  stCoRoutineEnv_t *env );

//3.func



//-----------------------------------------------------------------------------------------------

struct stTimeout_t;
struct stTimeoutItem_t ;

stTimeout_t *AllocTimeout( int iSize );
void 	FreeTimeout( stTimeout_t *apTimeout );
int  	AddTimeout( stTimeout_t *apTimeout,stTimeoutItem_t *apItem ,uint64_t allNow );

struct stCoEpoll_t;
stCoEpoll_t * AllocEpoll();
void 		FreeEpoll( stCoEpoll_t *ctx );

stCoRoutine_t *		GetCurrThreadCo();
void 				SetEpoll( stCoRoutineEnv_t *env,stCoEpoll_t *ev );

typedef void (*pfnCoRoutineFunc_t)();

#endif

#define __CO_ROUTINE_INNER_H__
