#include <stdio.h>
#include <qthread/qloop.h>
#include <qthread/qthread.h>

static void greeter(const size_t startat,
                         const size_t stopat, void *arg)
{
    size_t i;

    for (i = startat; i < stopat; i++)
       printf("#%lu: Hello, world!\n", i);

}

int main(int argc, char *argv[])
{
    qthread_initialize();

    qt_loop_balance(0, 10, greeter, NULL);

    return 0;
}
