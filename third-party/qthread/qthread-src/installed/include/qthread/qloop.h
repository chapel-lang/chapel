#ifndef QTHREAD_QLOOP
#define QTHREAD_QLOOP

#include "qthread.h"

Q_STARTCXX                             /* */
/* for convenient arguments to qt_loop */
typedef void (*qt_loop_f)(const size_t startat,
                          const size_t stopat,
                          void        *arg);
typedef void (*qt_loop_step_f)(void *arg);
typedef void (*qt_loopr_f)(const size_t   startat,
                           const size_t   stopat,
                           void *restrict arg,
                           void *restrict ret);
typedef void (*qt_accum_f)(void *restrict       a,
                           const void *restrict b);

typedef struct qqloop_handle_s qqloop_handle_t;
typedef struct qqloop_step_handle_s qqloop_step_handle_t;

void qt_loop(size_t    start,
             size_t    stop,
             qt_loop_f func,
             void     *argptr);
void qt_loop_simple(size_t    start,
                    size_t    stop,
                    qt_loop_f func,
                    void     *argptr);
void qt_loop_sv(size_t    start,
                size_t    stop,
                qt_loop_f func,
                void     *argptr);
void qt_loop_dc(size_t    start,
                size_t    stop,
                qt_loop_f func,
                void     *argptr);
void qt_loop_aligned(size_t    start,
                     size_t    stop,
                     qt_loop_f func,
                     void     *argptr);
void qt_loop_sinc(size_t    start,
                  size_t    stop,
                  qt_loop_f func,
                  void     *argptr);
void qt_loop_step(size_t         start,
                  size_t         stop,
                  size_t         stride,
                  qt_loop_step_f func,
                  void          *argptr);
void qt_loop_balance(const size_t    start,
                     const size_t    stop,
                     const qt_loop_f func,
                     void           *argptr);
void qt_loop_balance_simple(const size_t    start,
                            const size_t    stop,
                            const qt_loop_f func,
                            void           *argptr);
void qt_loop_balance_sv(const size_t    start,
                        const size_t    stop,
                        const qt_loop_f func,
                        void           *argptr);
void qt_loop_balance_dc(const size_t    start,
                        const size_t    stop,
                        const qt_loop_f func,
                        void           *argptr);
void qt_loop_balance_aligned(const size_t    start,
                             const size_t    stop,
                             const qt_loop_f func,
                             void           *argptr);
void qt_loop_balance_sinc(const size_t    start,
                          const size_t    stop,
                          const qt_loop_f func,
                          void           *argptr);
void qt_loopaccum_balance(const size_t     start,
                          const size_t     stop,
                          const size_t     size,
                          void *restrict   out,
                          const qt_loopr_f func,
                          void *restrict   argptr,
                          const qt_accum_f acc);
void qt_loopaccum_balance_sinc(const size_t     start,
                               const size_t     stop,
                               const size_t     size,
                               void *restrict   out,
                               const qt_loopr_f func,
                               void *restrict   argptr,
                               const qt_accum_f acc);
void qt_loopaccum_balance_sv(const size_t     start,
                             const size_t     stop,
                             const size_t     size,
                             void *restrict   out,
                             const qt_loopr_f func,
                             void *restrict   argptr,
                             const qt_accum_f acc);
void qt_loopaccum_balance_dc(const size_t     start,
                             const size_t     stop,
                             const size_t     size,
                             void *restrict   out,
                             const qt_loopr_f func,
                             void *restrict   argptr,
                             const qt_accum_f acc);

typedef enum {CHUNK, GUIDED, FACTORED, TIMED} qt_loop_queue_type;
qqloop_handle_t *qt_loop_queue_create(const qt_loop_queue_type type,
                                      const size_t             start,
                                      const size_t             stop,
                                      const size_t             incr,
                                      const qt_loop_f          func,
                                      void *const              argptr);
qqloop_step_handle_t *qt_loop_step_queue_create(const qt_loop_queue_type type,
                                                size_t                   start,
                                                size_t                   stop,
                                                size_t                   incr,
                                                qt_loop_step_f           func,
                                                void                    *argptr);
void qt_loop_queue_setchunk(qqloop_handle_t *l,
                            size_t           chunk);
void qt_loop_queue_run(qqloop_handle_t *loop);
void qt_loop_queue_run_there(qqloop_handle_t      *loop,
                             qthread_shepherd_id_t shep);
void qt_loop_queue_addworker(qqloop_handle_t            *loop,
                             const qthread_shepherd_id_t shep);


double qt_double_sum(double *array,
                     size_t  length,
                     int     checkfeb);
double qt_double_prod(double *array,
                      size_t  length,
                      int     checkfeb);
double qt_double_max(double *array,
                     size_t  length,
                     int     checkfeb);
double qt_double_min(double *array,
                     size_t  length,
                     int     checkfeb);

saligned_t qt_int_sum(saligned_t *array,
                      size_t      length,
                      int         checkfeb);
saligned_t qt_int_prod(saligned_t *array,
                       size_t      length,
                       int         checkfeb);
saligned_t qt_int_max(saligned_t *array,
                      size_t      length,
                      int         checkfeb);
saligned_t qt_int_min(saligned_t *array,
                      size_t      length,
                      int         checkfeb);

aligned_t qt_uint_sum(aligned_t *array,
                      size_t     length,
                      int        checkfeb);
aligned_t qt_uint_prod(aligned_t *array,
                       size_t     length,
                       int        checkfeb);
aligned_t qt_uint_max(aligned_t *array,
                      size_t     length,
                      int        checkfeb);
aligned_t qt_uint_min(aligned_t *array,
                      size_t     length,
                      int        checkfeb);

/* These are some utility accumulator functions */
static Q_UNUSED void qt_dbl_add_acc(void *restrict       a,
                                    const void *restrict b)
{
    *(double *)a += *(double *)b;
}

static Q_UNUSED void qt_int_add_acc(void *restrict       a,
                                    const void *restrict b)
{
    *(saligned_t *)a += *(saligned_t *)b;
}

static Q_UNUSED void qt_uint_add_acc(void *restrict       a,
                                     const void *restrict b)
{
    *(aligned_t *)a += *(aligned_t *)b;
}

static Q_UNUSED void qt_dbl_prod_acc(void *restrict       a,
                                     const void *restrict b)
{
    *(double *)a *= *(double *)b;
}

static Q_UNUSED void qt_int_prod_acc(void *restrict       a,
                                     const void *restrict b)
{
    *(saligned_t *)a *= *(saligned_t *)b;
}

static Q_UNUSED void qt_uint_prod_acc(void *restrict       a,
                                      const void *restrict b)
{
    *(aligned_t *)a *= *(aligned_t *)b;
}

static Q_UNUSED void qt_dbl_max_acc(void *restrict       a,
                                    const void *restrict b)
{
    if (*(double *)b > *(double *)a) {
        *(double *)a = *(double *)b;
    }
}

static Q_UNUSED void qt_int_max_acc(void *restrict       a,
                                    const void *restrict b)
{
    if (*(saligned_t *)b > *(saligned_t *)a) {
        *(saligned_t *)a = *(saligned_t *)b;
    }
}

static Q_UNUSED void qt_uint_max_acc(void *restrict       a,
                                     const void *restrict b)
{
    if (*(aligned_t *)b > *(aligned_t *)a) {
        *(aligned_t *)a = *(aligned_t *)b;
    }
}

static Q_UNUSED void qt_dbl_min_acc(void *restrict       a,
                                    const void *restrict b)
{
    if (*(double *)b < *(double *)a) {
        *(double *)a = *(double *)b;
    }
}

static Q_UNUSED void qt_int_min_acc(void *restrict       a,
                                    const void *restrict b)
{
    if (*(saligned_t *)b < *(saligned_t *)a) {
        *(saligned_t *)a = *(saligned_t *)b;
    }
}

static Q_UNUSED void qt_uint_min_acc(void *restrict       a,
                                     const void *restrict b)
{
    if (*(aligned_t *)b < *(aligned_t *)a) {
        *(aligned_t *)a = *(aligned_t *)b;
    }
}

void qt_qsort(double      *array,
              const size_t length);

Q_ENDCXX                               /* */
#endif // ifndef QTHREAD_QLOOP
/* vim:set expandtab: */
