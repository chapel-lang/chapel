#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include <qthread/spr.h>
#include <qthread/multinode.h>
#include "argparsing.h"

typedef struct {
    void *addr;
    size_t size;
} putter_args_t;

static aligned_t putter(void *args_)
{
    putter_args_t *args = (putter_args_t *)args_;
    void *dest_addr = args->addr;
    size_t buf_size = args->size;

    int const here = spr_locale_id();
    int const there = (here == 0) ? spr_num_locales() - 1 : here - 1;

    iprintf("[%03d] remote buffer %p of size %lu on %03d\n", 
            here, dest_addr, (unsigned long)buf_size, there);

    char *buf = malloc(buf_size * sizeof(char));
    assert(buf);

    for (size_t i = 0; i < buf_size; i++) {
        buf[i] = (char)(here + i);

        if (i % 10 == 0)
            iprintf("[%03d] filling local buffer[%d] = %d\n", here, i, buf[i]);
    }

    spr_put(there, dest_addr, buf, buf_size);

    iprintf("[%03d] done putting\n", here);

    free(buf);

    return 0;
}

int main(int argc, char *argv[])
{
    CHECK_VERBOSE();

    size_t small_buf_size = 16;
    size_t large_buf_size = 256;
    size_t custom_buf_size = 0;

    NUMARG(custom_buf_size, "BUF_SIZE");

    qthread_f actions[2] = {putter, NULL};
    assert(spr_init(SPR_SPMD, actions) == SPR_OK);

    int const num_locales = spr_num_locales();
    int const here = spr_locale_id();
    int const there = (here+1 == num_locales) ? 0 : here+1;

    int success = 1;

    /* Test small buffer size */
    if (0 == custom_buf_size) {
        size_t buf_size = small_buf_size;

        char *buf = calloc(buf_size, sizeof(char));
        assert(buf);

        iprintf("[%03d] small local buffer %p\n", here, buf);
        putter_args_t args = {buf, buf_size};

        aligned_t ret;
        qthread_fork_remote(putter, &args, &ret,
                            there, sizeof(putter_args_t));
        qthread_readFF(NULL, &ret);

        for (size_t i = 0; i < small_buf_size; i++) {
            if ((char)(there + i) != buf[i]) {
                success = 0;
                break;
            }

            if (i % 10 == 0)
                iprintf("[%03d] reading small local buffer (%p)[%d] = %d\n", 
                        here, buf, i, buf[i]);
        }

        free(buf);
    }

    /* Test large buffer size */
    if (success && (0 == custom_buf_size)) {
        size_t buf_size = large_buf_size;

        char *buf = calloc(buf_size, sizeof(char));
        assert(buf);

        iprintf("[%03d] large local buffer %p\n", here, buf);
        putter_args_t args = {buf, buf_size};

        aligned_t ret;
        qthread_fork_remote(putter, &args, &ret,
                            there, sizeof(putter_args_t));
        qthread_readFF(NULL, &ret);

        for (size_t i = 0; i < large_buf_size; i++) {
            if ((char)(there + i) != buf[i]) {
                success = 0;
                break;
            }

            if (i % 100 == 0)
                iprintf("[%03d] large reading local buffer (%p)[%d] = %d\n", 
                        here, buf, i, buf[i]);
        }

        free(buf);
    }

    /* Test custom buffer size */
    if (success && (0 < custom_buf_size)) {
        size_t buf_size = custom_buf_size;

        char *buf = calloc(buf_size, sizeof(char));
        assert(buf);

        iprintf("[%03d] custom local buffer %p\n", here, buf);
        putter_args_t args = {buf, buf_size};

        aligned_t ret;
        qthread_fork_remote(putter, &args, &ret,
                            there, sizeof(putter_args_t));
        qthread_readFF(NULL, &ret);

        for (size_t i = 0; i < custom_buf_size; i++) {
            if ((char)(there + i) != buf[i]) {
                success = 0;
                break;
            }

            if (i % 10 == 0)
                iprintf("[%03d] custom reading local buffer (%p)[%d] = %d\n", 
                        here, buf, i, buf[i]);
        }

        free(buf);
    }

    spr_fini();

    if (success) {
        iprintf("[%03d] SUCCESS\n", here);
        return 0;
    } else {
        iprintf("[%03d] FAILURE\n", here);
        return 1;
    }
}

