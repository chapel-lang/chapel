#include <assert.h>
#include <stdio.h>
#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <pthread.h>


static int64_t count;
static aligned_t lock;
bool is_recursive_lock = true;
bool is_not_recursive_lock = false;

pthread_mutex_t count_mutex;

static void task_1(size_t start, size_t stop, void  *args_) {
  qthread_lock(&lock);
  count++;
  qthread_unlock(&lock);
}

static void task_2(size_t start, size_t stop, void  *args_) {
  qthread_lock(&lock);
  qthread_lock(&lock);
  count++;
  qthread_unlock(&lock);
  qthread_unlock(&lock);
}

int main(int argc, char *argv[]) {
    uint64_t iters = 10000l;
    assert(qthread_initialize() == 0);

    /* Simple lock acquire and release */
    count = 0;
    qt_loop(0, iters, task_1, NULL);
    assert(iters == count);

    /* Recursive lock acquire and release, no recursion */
    qthread_lock_init(&lock, is_recursive_lock);
    {
      /* Recursive lock acquire and release, no recursion */
      count = 0;
      qt_loop(0, iters, task_1, NULL);
      assert(iters == count);
    
      /* Recursive lock acquire and release */
      count = 0;
      qt_loop(0, iters, task_2, NULL);
      assert(iters == count);
    }
    qthread_lock_destroy(&lock);

    return 0;
}
