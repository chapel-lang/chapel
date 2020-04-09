#ifdef HAVE_CONFIG_H
# include "config.h"                   /* for _GNU_SOURCE */
#endif

#include <qthread/qalloc.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>

int main(int argc,
         char *argv[])
{
    void *r, *r2;
    const char teststring[16] = "This is a test.";
    char filestat[30] = "/tmp/testqallocstatXXXXXX";
    char filedyn[30] = "/tmp/testqallocdynXXXXXX";
    char *ts, *ts2;
    off_t size = 4;
    int fd;

    size *= 1024;
    size *= 1024;
    if ((fd = mkstemp(filestat)) == -1) {
        perror("mktemp filestat");
        return -1;
    }
    close(fd);
    if ((fd = mkstemp(filedyn)) == -1) {
        perror("mktemp filedyn");
        return -1;
    }
    close(fd);
    /* making maps */
    r2 = qalloc_makedynmap(size, NULL, filedyn, 3);
    /*r2 = qalloc_loadmap("test2.img"); */
    if (r2 == NULL) {
        fprintf(stderr, "makedynmap returned NULL!\n");
        return -1;
    }
    r = qalloc_makestatmap(size, NULL, filestat, sizeof(teststring), 3);
    if (r == NULL) {
        fprintf(stderr, "makestatmap returned NULL!\n");
        return -1;
    }

    ts = (char *)qalloc_statmalloc((mapinfo_t *)r);
    if (ts == NULL) {
        fprintf(stderr, "statmalloc returned NULL!\n");
        return -1;
    }
    ts2 =
        (char *)qalloc_dynmalloc((dynmapinfo_t *)r2,
                                 strlen("012345678901") + 1);
    if (ts2 == NULL) {
        fprintf(stderr, "dynmalloc returned NULL!\n");
        return -1;
    }
    sprintf(ts, "%s", teststring);
    sprintf(ts2, "012345678901");
    qalloc_checkpoint();
    qalloc_free(ts, r);
    qalloc_free(ts2, r2);
    ts2 = (char *)qalloc_malloc(r2, 128);
    if (ts2 == NULL) {
        fprintf(stderr, "second dynmalloc returned NULL!\n");
        return -1;
    }
    memset(ts2, 0x55, 128);
    qalloc_free(ts2, r2);
    qalloc_cleanup();
    /* the following is just so that it can be used in the automake test: */
    if (unlink(filestat) != 0) {
        perror("unlinking filestat");
        return -1;
    }
    if (unlink(filedyn) != 0) {
        perror("unlinking test2.img");
        return -1;
    }
    return 0;
}

/* vim:set expandtab */
