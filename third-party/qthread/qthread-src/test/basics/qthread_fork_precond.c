#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include "argparsing.h"

#define NUM_CONSUMERS 3
#define NUM_MULTI     3

// //////////////////////////////////////////////////////////////////////////////
static aligned_t consumer(void *arg)
{
    aligned_t *v = (aligned_t *)arg;
    aligned_t  value;

    value = *v; // Not using readFF() since we know it is ready?
    iprintf("Consumer: got value %u\n", (unsigned)value);

    return value;
}

static aligned_t producer(void *arg)
{
    aligned_t      *v     = (aligned_t *)arg;
    const aligned_t value = 42;

    iprintf("Producer: setting value %u\n", (unsigned)value);
    qthread_writeEF_const(v, value);

    return 0;
}

// //////////////////////////////////////////////////////////////////////////////
static aligned_t multi_consumer(void *arg)
{
    aligned_t **m   = (aligned_t **)arg;
    aligned_t   sum = 0;

    for (int i = 0; i < 3; i++) {
        iprintf("Multi-consumer: got value %u\n", (unsigned)*m[i]);
        sum += *m[i];
    }

    return sum;
}

static aligned_t multi_producer(void *arg)
{
    aligned_t      *m     = (aligned_t *)arg;
    const aligned_t value = 42;

    iprintf("Multi-producer: setting value %u\n", (unsigned)value);
    qthread_writeEF_const(m, value);

    return 0;
}

// //////////////////////////////////////////////////////////////////////////////
static aligned_t array_consumer(void *arg)
{
    aligned_t *m   = (aligned_t *)arg;
    aligned_t  sum = 0;

    for (int i = 0; i < NUM_MULTI; i++) {
        iprintf("Multi-consumer: got value %u\n", (unsigned)m[i]);
        sum += m[i];
    }

    return sum;
}

static aligned_t array_producer(void *arg)
{
    aligned_t      *m     = (aligned_t *)arg;
    const aligned_t value = 42;

    iprintf("Multi-producer: setting value %u\n", (unsigned)value);
    qthread_writeEF_const(m, value);

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
    assert(qthread_init(2) == 0);

    CHECK_VERBOSE();

    iprintf("\n***** Test multiple consumers, single producer *****\n");
    {
        aligned_t rets[NUM_CONSUMERS + 1];

        // Initialize values and empty
        aligned_t v = 69;
        qthread_empty(&v);

        for (int i = 0; i < NUM_CONSUMERS; i++) qthread_fork_precond(consumer, &v, &rets[i], 1, &v);
        qthread_fork(producer, &v, &rets[NUM_CONSUMERS]);

        for (int i = 0; i <= NUM_CONSUMERS; i++) {
            qthread_readFF(&rets[i], &rets[i]);

            // Verify return values
            if ((rets[i] != 42) && (i != NUM_CONSUMERS)) {
                iprintf("Bad return value %i! Wanted 42, got %u\n", i, (unsigned int)rets[i]);
		abort();
            }
        }
    }

    iprintf("\n***** Test single consumer, multiple producers *****\n");
    {
        aligned_t ret;

        // Initialize values and empty
        aligned_t v1 = 1;
        qthread_empty(&v1);

        aligned_t v2 = 2;
        qthread_empty(&v2);

        aligned_t v3 = 3;
        qthread_empty(&v3);

        aligned_t *args[3] = { &v1, &v2, &v3 };
        qthread_fork_precond(multi_consumer, &args, &ret, 3, &v1, &v2, &v3);
        qthread_fork(multi_producer, &v1, NULL);
        qthread_fork(multi_producer, &v2, NULL);
        qthread_fork(multi_producer, &v3, NULL);

        qthread_readFF(&ret, &ret);

        // Verify return value
        if (ret != 3 * 42) {
            iprintf("Bad return value! Hrm. Wanted %u, got %u\n", 3 * 42, (unsigned int)ret);
            return 1;
        }
    }

    iprintf("\n***** Test single consumer, multiple producers (array) *****\n");
    {
        aligned_t ret;

        // Initialize values and empty
        aligned_t  v[NUM_MULTI];
        aligned_t *vptr[NUM_MULTI];
        for (int i = 0; i < NUM_MULTI; i++) {
            vptr[i] = &v[i];
            v[i]    = i + 1;
            qthread_empty(&v[i]);
        }

        qthread_fork_precond(array_consumer, v, &ret, -NUM_MULTI, vptr);
        for (int i = 0; i < NUM_MULTI; i++) qthread_fork(array_producer, &v[i], NULL);

        qthread_readFF(&ret, &ret);

        // Verify return value
        if (ret != NUM_MULTI * 42) {
            iprintf("Bad return value! Wanted %u, got %u\n", NUM_MULTI * 42, (unsigned int)ret);
            return 1;
        }
    }

    iprintf("\n***** Test migration: MC/SP with C on 1 and S on 0 *****\n");
    {
        assert(qthread_num_shepherds() == 2);

        aligned_t rets[NUM_CONSUMERS + 1];

        // Initialize values and empty
        aligned_t v = 69;
        qthread_empty(&v);

        for (int i = 0; i < NUM_CONSUMERS; i++) qthread_fork_precond_to(consumer, &v, &rets[i], 1, 1, &v);
        qthread_fork_to(producer, &v, &rets[NUM_CONSUMERS], 0);

        for (int i = 0; i <= NUM_CONSUMERS; i++) {
            qthread_readFF(&rets[i], &rets[i]);

            // Verify return values
            if ((rets[i] != 42) && (i != NUM_CONSUMERS)) {
                iprintf("Bad return value! Wanted 42, got %u\n", (unsigned int)rets[i]);
                return 1;
            }
        }
    }

    iprintf("\n***** Test migration: SC/MP with C on 1 and S on 0 *****\n");
    {
        assert(qthread_num_shepherds() == 2);

        aligned_t ret;

        // Initialize values and empty
        aligned_t v1 = 1;
        qthread_empty(&v1);

        aligned_t v2 = 2;
        qthread_empty(&v2);

        aligned_t v3 = 3;
        qthread_empty(&v3);

        aligned_t *args[3] = { &v1, &v2, &v3 };
        qthread_fork_precond_to(multi_consumer, &args, &ret, 1, 3, &v1, &v2, &v3);
        qthread_fork_to(multi_producer, &v1, NULL, 0);
        qthread_fork_to(multi_producer, &v2, NULL, 0);
        qthread_fork_to(multi_producer, &v3, NULL, 0);

        qthread_readFF(&ret, &ret);

        // Verify return value
        if (ret != 3 * 42) {
            iprintf("Bad return value! Wanted %u, got %u\n", 3 * 42, (unsigned int)ret);
            return 1;
        }
    }

    iprintf("\n***** Test migration: SC/MP with C on 1 and S on 0 (array) *****\n");
    {
        assert(qthread_num_shepherds() == 2);

        aligned_t ret;

        // Initialize values and empty
        aligned_t  v[NUM_MULTI];
        aligned_t *vptr[NUM_MULTI];
        for (int i = 0; i < NUM_MULTI; i++) {
            vptr[i] = &v[i];
            v[i]    = i + 1;
            qthread_empty(&v[i]);
        }

        qthread_fork_precond_to(array_consumer, v, &ret, 1, -NUM_MULTI, vptr);
        for (int i = 0; i < NUM_MULTI; i++) qthread_fork_to(array_producer, &v[i], NULL, 0);

        qthread_readFF(&ret, &ret);

        // Verify return value
        if (ret != NUM_MULTI * 42) {
            iprintf("Bad return value! Wanted %u, got %u\n", NUM_MULTI * 42, (unsigned int)ret);
            return 1;
        }
    }

    iprintf("Success!\n");

    return 0;
}

/* vim:set expandtab */
