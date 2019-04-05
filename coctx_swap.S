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

.globl coctx_swap
#if !defined( __APPLE__ ) && !defined( __FreeBSD__ )
.type  coctx_swap, @function
#endif
coctx_swap:

#if defined(__i386__)
	leal 4(%esp), %eax //sp 
	movl 4(%esp), %esp 
	leal 32(%esp), %esp //parm a : &regs[7] + sizeof(void*)

	pushl %eax //esp ->parm a 

	pushl %ebp
	pushl %esi
	pushl %edi
	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl -4(%eax)

	
	movl 4(%eax), %esp //parm b -> &regs[0]

	popl %eax  //ret func addr
	popl %ebx  # s
	popl %ecx
	popl %edx
	popl %edi
	popl %esi
	popl %ebp
	popl %esp
	pushl %eax //set ret func addr

	xorl %eax, %eax
	ret

#elif defined(__x86_64__)

	leaq 8(%rsp),%rax  // 地址传送指令，将存储器操作数的地址保存到rax寄存器中。即执行完此指令后，rax的值是(rsp+8)，即指向函数返回地址
	leaq 112(%rdi),%rsp // 让栈顶指针指向第一个参数（当前协程的结构体）的regs[14]的位置，这就是libaco的作者所说的bug所在的地方，即让rsp没有指向栈顶。
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx

	pushq -8(%rax) //ret func addr 这是寄存器间接寻址，取%rax加上偏移-8作为操作数的地址。 即刚进该函数的时候，rsp指向的地址存储的就是函数的返回地址。

	pushq %rsi
	pushq %rdi
	pushq %rbp
	pushq %r8
	pushq %r9
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15  // 寄存器入栈，即保存到当前协程的结构体中
	
	movq %rsi, %rsp // 让栈顶指针指向第二个参数（目标协程的结构体）
	popq %r15 // 出栈，恢复目标协程保存的寄存器
	popq %r14
	popq %r13
	popq %r12
	popq %r9
	popq %r8
	popq %rbp
	popq %rdi
	popq %rsi
	popq %rax //ret func addr 此时，rax里面的指针指向（保存返回地址的地方再低8字节）
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rsp // 此时，rsp指向（保存函数返回地址的地方）.
	pushq %rax // 此时，rsp指向（保存函数返回地址的地方 - 8），本函数的返回地址即变为了
	
	xorl %eax, %eax // 把函数的返回值置0
	ret // 从栈中取得返回地址，并跳转到该位置执行


	// ****************************************************************
	/*
	movq %rsp,%rax  // 地址传送指令，将存储器操作数的地址保存到rax寄存器中。即执行完此指令后，rax的值是(rsp+8)，即指向函数返回地址
    leaq 112(%rdi),%rsp // 让栈顶指针指向第一个参数（当前协程的结构体）的regs[14]的位置
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx

    pushq (%rax) // 无关紧要

    pushq %rsi
    pushq %rdi
    pushq %rbp
    pushq %r8
    pushq %r9
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15  // 寄存器入栈，即保存到当前协程的结构体中

    movq %rsi, %rsp // 让栈顶指针指向第二个参数（目标协程的结构体）
    popq %r15 // 出栈，恢复目标协程保存的寄存器
    popq %r14
    popq %r13
    popq %r12
    popq %r9
    popq %r8
    popq %rbp
    popq %rdi
    popq %rsi
    popq %rax //ret func addr
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rsp // 此时，rsp指向（保存函数返回地址的地方）

    xorl %eax, %eax // 把函数的返回值置0
    ret // 从栈中取得返回地址，并跳转到该位置执行
    */
#endif
