#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>

static aligned_t greeter(void *arg)
{
    printf("Hello World!\n");

    return 0;
}

int main(int argc, char *argv[])
{
    aligned_t return_value = 0;
    int status;

    status = qthread_initialize();
    assert(status == QTHREAD_SUCCESS);

    status = qthread_fork(greeter, NULL, &return_value);
    assert(status == QTHREAD_SUCCESS);

    status = qthread_readFF(NULL, &return_value);
    assert(return_value == QTHREAD_SUCCESS);

    printf("greeter returned %lu\n", 
           (unsigned long)return_value);

    return EXIT_SUCCESS;
}
