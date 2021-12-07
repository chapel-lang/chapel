#include <assert.h>
#include <stdio.h>
#include <qthread/qthread.h>
#include <qthread/qloop.h>

static int64_t count = 0;
static aligned_t lock;

static void task(size_t start, size_t stop, void  *args_) {
  qthread_lock(&lock);
  count++;
  qthread_unlock(&lock);
}

int main(int argc, char *argv[]) {
    uint64_t iters = 1000000l;
    assert(qthread_initialize() == 0);
    qt_loop(0, iters, task, NULL);
    assert(iters == count);
    return 0;
}
