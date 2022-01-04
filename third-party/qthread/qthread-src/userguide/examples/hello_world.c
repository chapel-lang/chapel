#include <stdio.h>
#include <qthread/qthread.h>

static aligned_t greeter(void *arg)
{
    printf("Hello, world!\n");

    return 0;
}

int main(int argc, char *argv[])
{
    aligned_t return_value = 0;

    qthread_initialize();
    qthread_fork(greeter, NULL, &return_value);
    qthread_readFF(NULL, &return_value);

    return return_value;
}
