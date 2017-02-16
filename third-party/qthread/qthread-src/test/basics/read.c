#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include "argparsing.h"

const char cfilename[] = "test_qthread_read.XXXXXXXXXXXXX";

static aligned_t reader(void *arg)
{
    int ret;
    char buf[strlen(cfilename)+1];

    memset(buf, 0, strlen(cfilename)+1);
    iprintf("in reader function\n");
    ret = read((int)(intptr_t)arg, buf, 5);
    iprintf("read '%s'\n", buf);
    iprintf("ret = %i\n", ret);
    assert(ret == 5);
    if (strncmp(buf, cfilename, 5)) {
	buf[5] = 0;
	fprintf(stderr, "Corrupt Read! '%s' != '%s'!\n", buf, cfilename);
	abort();
    }

    return 0;
}

int main(int argc,
         char *argv[])
{
    aligned_t t;
    int fd;
    char filename[strlen(cfilename)+1];
    ssize_t ret;

    CHECK_VERBOSE();

    snprintf(filename, strlen(cfilename), "%s", cfilename);
    iprintf("filename = '%s'\n", filename);

    /* First, set up a temporary file */
    fd = mkstemp(filename);
    iprintf("filename = '%s'\n", filename);
    iprintf("fd = %i\n", fd);
    if (fd < 0) {
	perror("mkstemp failed");
    }
    assert(fd >= 0);

    ret = write(fd, cfilename, strlen(cfilename));
    assert(ret == strlen(cfilename));

    ret = (ssize_t)lseek(fd, 0, SEEK_SET);
    assert(ret == 0);

    reader((void*)(intptr_t)fd);

    ret = (ssize_t)lseek(fd, 0, SEEK_SET);
    assert(ret == 0);

    assert(qthread_initialize() == 0);

    CHECK_VERBOSE();

    qthread_fork(reader, (void*)(intptr_t)fd, &t);
    qthread_readFF(NULL, &t);

    qthread_finalize();

    close(fd);
    unlink(filename);

    return 0;
}

/* vim:set expandtab */
