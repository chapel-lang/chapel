#include <stdio.h>
#include <qthread/qthread.h>

static aligned_t greeter(void *arg)
{
    printf("#%lu: Hello, world!\n", (unsigned long)(uintptr_t)arg);

    return 0;
}

int main(int argc, char *argv[])
{
    aligned_t return_value[10] = { 0 };
    unsigned long i;

    qthread_initialize();

    for (i = 0; i < 10; i++)
       qthread_fork(greeter, (void *)i, &return_value[i]);

    for (i = 0; i < 10; i++)
       qthread_readFF(NULL, &return_value[i]);

    return 0;
}
