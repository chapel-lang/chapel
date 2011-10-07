#include <stdio.h>
#include <qthread/qthread.h>
#include <qthread/qloop.hpp>
#include <qthread/qloop.h>

#define NUM_SHEPHERDS 3

struct test_struct {
    test_struct(int _x, int* _answer) : x(_x), answer(_answer) {}

    void operator() (int start, int stop)
    {
      printf("start: %4d    stop: %4d\n", start, stop);

      for (int i = start; i < stop; ++i) answer[i] = x * i;
    }

    int x;
    int* answer;
};

void test_func()
{
    int* answer = new int[100];
    for (int i = 0; i < 100; ++i) answer[i] = 0;

//    qt_loop(0, 100, test_struct(3, answer));
//    qt_loop_future(0, 100, test_struct(3, answer));
    qt_loop_balance(0, 100, test_struct(3, answer));
//    qt_loop_balance_future(0, 100, test_struct(3, answer));

    printf("\nanswer:");
    for (int i = 0; i < 100; ++i)
    {
      printf("%6d", answer[i]);
      if (i % 10 == 9) printf("\n       ");
    }
    printf("\n");

    delete [] answer;
}

int main(int argc, char **argv)
{
    qthread_init(NUM_SHEPHERDS);

    test_func();

    return 0;
}
