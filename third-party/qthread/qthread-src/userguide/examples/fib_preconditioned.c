#include <stdio.h>
#include <stdlib.h>
#include <qthread/qthread.h>

typedef struct {
    aligned_t n;
    aligned_t result;
} f_arg_t;

typedef struct {
    f_arg_t    fargs[2];
    aligned_t *target;
} fr_arg_t;

static aligned_t fib_result(void *arg)
{
    aligned_t *target = ((fr_arg_t *)arg)->target;
    aligned_t  r0     = ((fr_arg_t *)arg)->fargs[0].result;

    r0 += ((fr_arg_t *)arg)->fargs[1].result;

    qthread_writeEF(target, &r0);
    free(arg);

    return 0;
}

static aligned_t fib(void *arg)
{
    aligned_t  n      = ((f_arg_t *)arg)->n;
    aligned_t *result = &((f_arg_t *)arg)->result;

    if (n < 2) {
        qthread_writeEF_const(result, n);
        return 0;
    }

    fr_arg_t *fibs = malloc(sizeof(fr_arg_t));
    f_arg_t  *f1   = &fibs->fargs[0];
    f_arg_t  *f2   = &fibs->fargs[1];

    f1->n = n - 1;
    f2->n = n - 2;
    qthread_empty(&f1->result);
    qthread_empty(&f2->result);

    fibs->target = result;

    // Collect results of sub-actions
    qthread_fork_precond(fib_result, fibs, NULL, 2, 
                         &f1->result, &f2->result);

    // Fork off recursive actions
    qthread_fork(fib, f1, NULL);
    qthread_fork(fib, f2, NULL);

    return 0;
}

int main(int argc, char *argv[])
{
    unsigned long n = 0;
    f_arg_t args = { 0, 0 };

    if (argc > 1)
       n = atol(argv[1]);

    args.n = (aligned_t) n;

    qthread_initialize();

    qthread_empty(&args.result);

    qthread_fork(fib, &args, NULL);

    qthread_readFF(NULL, &args.result);

    printf("fib(%lu) = %lu\n", n, args.result);

    return 0;
}
