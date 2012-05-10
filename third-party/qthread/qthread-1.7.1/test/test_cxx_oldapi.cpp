#include <qthread/futurelib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <qthread/syncvar.hpp>

#define NUM_LOCS()       3
#define FUTURE_PER_LOC() 128

static void my_main();

int main (int argc,
          char **argv)
{
    qthread_init(NUM_LOCS());
    future_init(FUTURE_PER_LOC());

    my_main();

    return 0;
}

static Q_UNUSED void set(int val,
                         int&i)
{
    i = val + 1;
    printf("set i (%p) = %d\n", (void*)&i, i);
}

static Q_UNUSED void output(const int&i)
{
    printf("output i (%p) = %d\n", (void*)&i, i);
}

static Q_UNUSED void output_double(double i)
{
    printf("output double i (%p) = %.4f\n", (void*)&i, i);
}

template <class T>
static void recvData(const T&t)
{
    // printf ("Got data reference @ %p\n", &t);
}

class BigData
{
    char buf[512];
public:
    static int copy_count;
    BigData()
    {
        printf("Make new BigData @ %p\n", (void*)this);
    }

    BigData(const BigData &cp)
    {
        copy_count++;
        printf("Copy big data %p into new data @ %p\n", (void*)&cp,
               (void*)this);
    }
};

class UserArray
{
    int *ptr;
public:
    UserArray(int size)
    {
        ptr = new int[size];
    }
    int&operator[] (int index)
    {
        return ptr[index];
    }
};

int BigData::copy_count = 0;

#define ALIGN_ATTR __attribute__ ((aligned(8)))

template <class ArrayT>
static int genericArraySet (ArrayT&arr,
                            int size,
                            const char* const name)
{
    printf(">>>>>>  Array setting %s <<<<<<<\n", name);
    mt_loop<Iterator, ArrayPtr, mt_loop_traits::Par> (set, 0, arr, 0, size);
    return 1;
}

template <class ArrayT>
static void genericArrayPrint (ArrayT&arr,
                               int size,
                               const char* const name)
{
    printf(">>>>>>  Array printing %s <<<<<<<\n", name);
    mt_loop<ArrayPtr, mt_loop_traits::Par> (output, arr, 0, size);
    printf(">>>>>>  Array printing double by value %s <<<<<<<\n", name);
    mt_loop<ArrayPtr, mt_loop_traits::Par> (output_double, arr, 0, size);
}

class add
{
    int a_;
public:
    add(int a) : a_(a) { }
    int operator() (int b)
    {
        return b + a_;
    }
};

class sub
{
    int s_;
public:
    sub(int s) : s_(s) { }
    int operator() (int b)
    {
        return b - s_;
    }
};

template <class OpT>
static void class_stuff (int value,
                         OpT op,
                         int times)
{
    int results[3];

    mt_mfun_loop_returns<ArrayPtr, Val, mt_loop_traits::Par>
        (&op, (int*)results, &OpT::operator(), value, 0, 3);

    /*for (int i = 0; i < 3; i++) {
     * printf ("[testq] Result = %d\n", results[i]);
     * }*/

    int sum = 0;
    mt_mfun_loop_returns<Collect<mt_loop_traits::Add>, Val,
                         mt_loop_traits::Par>
        (&op, sum, &OpT::operator(), value, 0, times);

    // printf ("[testq] Sum of Result (%d times) %d\n", times, sum);
}

void my_main()
{
    // printf ("[testq] Hello main\n");

    // array_stuff();
    // message_stuff();
    // vanilla_stuff();
    // big_data_stuff();
    class_stuff<add>(10, 4, 3);
    class_stuff<sub>(10, 4, 7);
}

/* vim:set expandtab */
