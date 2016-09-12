/**
 * Instructions:
 *  1) gcc -o main main.c -lpthread -lqthread
 *  2) ./main
 *
 *  Credit to Sam <sam@ltsnet.net>
 *
 */
#include <stdio.h>
#include <pthread.h>
#include "qthread/qthread.h"
#include "argparsing.h"

syncvar_t x = SYNCVAR_STATIC_EMPTY_INITIALIZER;
uint64_t  i = 1;

static void *pthread_routine(void *arg)
{
    qthread_syncvar_writeEF_const(&x, 1);
    return NULL;
}

int main(void)
{
    pthread_t thread;
    uint64_t  readout = 0;

    CHECK_VERBOSE();

    qthread_initialize();

    pthread_create(&thread, NULL, pthread_routine, NULL);

    qthread_syncvar_readFE(&readout, &x);
    iprintf("found readout = %lu\n", (unsigned long)readout);

    return 0;
}

/* vim:set expandtab: */
