#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include "argparsing.h"

static aligned_t use_default_space(void *arg)
{
    unsigned int size = 64 * sizeof(int);

    iprintf("use_default_space(): size_tasklocal() is %d\n", qthread_size_tasklocal());
    iprintf("use_default_space(): get_tasklocal(%d)\n", size);
    int *local_int_arr = (int *)qthread_get_tasklocal(size);
    iprintf("use_default_space(): size_tasklocal() is %d\n", qthread_size_tasklocal());
    iprintf("use_default_space(): local_int_arr at %p\n", local_int_arr);
    local_int_arr = (int *)qthread_get_tasklocal(size);
    for (int i=0; i<64; ++i) {
	local_int_arr[i] = i;
    }
    iprintf("use_default_space(): assigned tasklocal data successfully (42 = %d)\n", local_int_arr[42]);
    local_int_arr = NULL;
    qthread_yield();
    local_int_arr = (int *)qthread_get_tasklocal(size);
    iprintf("use_default_space(): local_int_arr at %p\n", local_int_arr);
    for (int i=0; i<64; ++i) {
	assert(local_int_arr[i] == i);
    }
    iprintf("use_default_space(): all assertions passed!\n");

    return 0;
}

static aligned_t use_allocated_space(void *arg)
{
    unsigned int size = 2048 * sizeof(int);

    iprintf("use_allocated_space(): size_tasklocal() is %d\n", qthread_size_tasklocal());
    iprintf("use_allocated_space(): get_tasklocal(%d)\n", size);
    int *local_int_arr = (int *)qthread_get_tasklocal(size);
    iprintf("use_allocated_space(): size_tasklocal() is %d\n", qthread_size_tasklocal());
    iprintf("use_allocated_space(): local_int_arr at %p\n", local_int_arr);
    local_int_arr = (int *)qthread_get_tasklocal(size);
    for (int i=0; i<2048; ++i) {
	local_int_arr[i] = 2048 - i;
    }
    iprintf("use_allocated_space(): assigned tasklocal data successfully (42 = %d)\n", local_int_arr[42]);
    local_int_arr = NULL;
    qthread_yield();
    local_int_arr = (int *)qthread_get_tasklocal(size);
    iprintf("use_allocated_space(): local_int_arr at %p\n", local_int_arr);
    for (int i=0; i<2048; ++i) {
	assert(local_int_arr[i] == 2048 - i);
    }
    iprintf("use_allocated_space(): all assertions passed!\n");

    return 0;
}

#ifdef __INTEL_COMPILER
int setenv(const char *name,
           const char *value,
           int         overwrite);
#endif

int main(int   argc,
         char *argv[])
{
    aligned_t t;

    setenv("QTHREAD_TASKLOCAL_SIZE", "0", 1);
    qthread_initialize();

    CHECK_VERBOSE();

    iprintf("set QTHREAD_TASKLOCAL_SIZE to 0\n");
    // Test default size (0 < size < tasklocal_size)
    iprintf("\n***** Testing default size *****\n");
    {
        unsigned int size = sizeof(int);
        iprintf("main(): size_tasklocal() is %d\n", qthread_size_tasklocal());
        iprintf("main(): get_tasklocal(%d)\n", size);
        int *local_int_ptr = (int *)qthread_get_tasklocal(size);
        iprintf("main(): size_tasklocal() is %d\n", qthread_size_tasklocal());
        iprintf("main(): tasklocal data at %p\n", local_int_ptr);
        *local_int_ptr = 42;
        iprintf("set tasklocal data to '42'\n");
        local_int_ptr = NULL;
        local_int_ptr = (int *)qthread_get_tasklocal(size);
        assert(42 == *local_int_ptr);
        iprintf("main(): retrieved tasklocal data successfully (42 == %d)\n", *local_int_ptr);
        local_int_ptr = NULL;
    }
    // Test allocating tasklocal (0 < tasklocal_size < size)
    iprintf("\n***** Testing allocating data *****\n");
    {
        unsigned int size = 1024 * sizeof(int);
        iprintf("main(): size_tasklocal() is %d\n", qthread_size_tasklocal());
        iprintf("main(): get_tasklocal(%d)\n", size);
        int *local_int_arr = (int *)qthread_get_tasklocal(size);
        iprintf("main(): size_tasklocal() is %d\n", qthread_size_tasklocal());
        iprintf("main(): local_int_arr at %p\n", local_int_arr);
        local_int_arr[42] = 42;
        iprintf("main(): set local_int_arr[42] = %d\n", local_int_arr[42]);
        local_int_arr = NULL;
        local_int_arr = (int *)qthread_get_tasklocal(size);
        assert(42 == local_int_arr[0]);
        iprintf("main(): retrieved copied tasklocal data successfully (42 == %d)\n", local_int_arr[42]);
        assert(42 == local_int_arr[42]);
        iprintf("main(): retrieved tasklocal data successfully (42 == %d)\n", local_int_arr[42]);
        local_int_arr = NULL;
    }
    // Test (0 < size < tasklocal_size)
    iprintf("\n***** Testing accessing allocated data *****\n");
    {
        unsigned int size = 64 * sizeof(int);
        iprintf("main(): size_tasklocal() is %d\n", qthread_size_tasklocal());
        iprintf("main(): get_tasklocal(%d)\n", size);
        int *local_int_arr = (int *)qthread_get_tasklocal(size);
        iprintf("main(): size_tasklocal() is %d\n", qthread_size_tasklocal());
        iprintf("main(): local_int_arr at %p\n", local_int_arr);
        local_int_arr = (int *)qthread_get_tasklocal(size);
        assert(42 == local_int_arr[42]);
        iprintf("main(): retrieved tasklocal data successfully (42 == %d)\n", local_int_arr[42]);
        local_int_arr = NULL;
    }
    // Test resizing
    iprintf("\n***** Testing resizing data allocation *****\n");
    {
        unsigned int size = 2048 * sizeof(int);
        iprintf("main(): size_tasklocal() is %d\n", qthread_size_tasklocal());
        iprintf("main(): get_tasklocal(%d)\n", size);
        int *local_int_arr = (int *)qthread_get_tasklocal(size);
        iprintf("main(): size_tasklocal() is %d\n", qthread_size_tasklocal());
        iprintf("main(): local_int_arr at %p\n", local_int_arr);
        local_int_arr = (int *)qthread_get_tasklocal(size);
        assert(42 == local_int_arr[42]);
        iprintf("main(): retrieved tasklocal data successfully (42 == %d)\n", local_int_arr[42]);
        local_int_arr = NULL;
    }
    // Test dijoint data spaces
    iprintf("\n***** Testing disjoint task data *****\n");
    {
        qthread_fork(use_default_space, NULL, &t);
        qthread_readFE(&t, &t);
        qthread_fork(use_allocated_space, NULL, &t);
        qthread_readFF(&t, &t);
    }

    return 0;
}

/* vim:set expandtab */
