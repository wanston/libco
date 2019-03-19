//
// Created by tong on 19-3-13.
//
#include "co_routine.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <stack>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>



#include<iostream>
using namespace std;

static stCoRoutine_t *coa,*cob, *coc;

static void* A(void *arg) {
    printf("1 ");
//    co_yield_ct();  // 切出到主协程
    printf("2 ");
}


static void* B(void *arg) {
    using namespace std;
    printf("x ");
    co_yield_ct();  // 切出到主协程
    printf("y ");
}


static void* C(void *arg) {
    using namespace std;
    printf("alpha ");
    co_yield_ct();  // 切出到主协程
    printf("beta ");

    co_resume(coa);
    co_resume(cob);
}


int main(void) {
    co_create(&coa,NULL,A,NULL);// 创建了主协程，和A协程，其都有上下文和栈空间，现在仍然处于正常的运行栈内
//    co_create(&cob,NULL,B,NULL);
//    co_create(&coc,NULL,C,NULL);
    co_resume(coa);
    co_resume(coa);
//    co_resume(cob);
//    co_resume(coc);
//    co_resume(coc);
    printf("\n");
}
