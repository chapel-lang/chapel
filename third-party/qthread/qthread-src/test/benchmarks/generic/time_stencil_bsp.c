#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/qtimer.h>

#include "argparsing.h"

#define NUM_NEIGHBORS 5
#define NUM_STAGES 2
#define BOUNDARY 42

//#define TIME_WORKLOAD

static int num_timesteps;

static int workload;
static int workload_per;
static int workload_var;

typedef struct stencil {
    size_t N;
    size_t M;
    aligned_t **stage[NUM_STAGES];
} stencil_t;

typedef struct update_args {
    stencil_t *points;
    size_t i;
    size_t stage;
} update_args_t;

typedef struct rows_args {
    stencil_t *points;
    size_t stage;
} rows_args_t;

////////////////////////////////////////////////////////////////////////////////
static inline void perform_local_work(void)
{
# ifdef TIME_WORKLOAD
    qtimer_t work_timer = qtimer_create();
    qtimer_start(work_timer);
# endif // TIME_WORKLOAD
    volatile unsigned long work = workload;
    long rand_per = (long)qtimer_fastrand();
    long rand_var = (long)qtimer_fastrand();

    rand_per = (rand_per<0) ? (-rand_per)%100 : rand_per%100;
    if (rand_per < workload_per) {
        rand_var = (rand_var<0) ? (-rand_var)%100 : rand_var%100;
        work += (workload * (workload_var * 0.01)) * (rand_var * 0.01);
    }

    for (int i = 0; i < work; i++) {
        work = work % 1000000000;
    }
    work++;
# ifdef TIME_WORKLOAD
    qtimer_stop(work_timer);
    fprintf(stdout, "Worked for %f\n", qtimer_secs(work_timer));
    qtimer_destroy(work_timer);
# endif // TIME_WORKLOAD
}

static inline void print_stage(stencil_t *points, size_t stage)
{
    for (int i = 0; i < points->N; i++) {
        fprintf(stderr, "%02lu", (unsigned long)points->stage[stage][i][0]);
        for (int j = 1; j < points->M; j++) {
            fprintf(stderr, "  %02lu", (unsigned long)points->stage[stage][i][j]);
        }
        fprintf(stderr, "\n");
    }
}

static inline size_t prev_stage(size_t stage)
{
    return (stage == 0) ? NUM_STAGES-1 : stage - 1;
}

static inline size_t next_stage(size_t stage)
{
    return (stage == NUM_STAGES-1) ? 0 : stage + 1;
}

////////////////////////////////////////////////////////////////////////////////
static void update(const size_t start, const size_t stop, void *arg)
{
    stencil_t *points = ((update_args_t *)arg)->points;
    size_t i = ((update_args_t *)arg)->i;
    size_t j = start;
    size_t stage = ((update_args_t *)arg)->stage;

    size_t prev = prev_stage(stage);

    // Perform local work
    perform_local_work();
    aligned_t sum = points->stage[prev][i  ][j-1];
    sum          += points->stage[prev][i-1][j  ];
    sum          += points->stage[prev][i  ][j  ];
    sum          += points->stage[prev][i+1][j  ];
    sum          += points->stage[prev][i  ][j+1];

    points->stage[stage][i][j] = sum/NUM_NEIGHBORS;
}

static void spawn_rows(const size_t start, const size_t stop, void *arg) {
    stencil_t *points = ((rows_args_t *)arg)->points;
    size_t stage = ((rows_args_t *)arg)->stage;

    update_args_t args = {points, start, stage};
    qt_loop(1, points->M-1, update, &args);
}

int main(int argc, char *argv[])
{
    int n = 10;
    int m = 10;
    num_timesteps = 10;
    workload = 0;
    workload_per = 0;
    workload_var = 0;
    int print_final = 0;
    int alltime = 0;

    CHECK_VERBOSE();
    NUMARG(n, "N");
    NUMARG(m, "M");
    NUMARG(num_timesteps, "TIMESTEPS");
    NUMARG(workload, "WORKLOAD");
    NUMARG(workload_per, "WORKLOAD_PER");
    NUMARG(workload_var, "WORKLOAD_VAR");
    NUMARG(print_final, "PRINT_FINAL");
    NUMARG(alltime, "ALL_TIME");

    assert (n > 0 && m > 0);

    // Initialize Qthreads
    assert(qthread_initialize() == 0);

    qtimer_t alloc_timer = qtimer_create();
    qtimer_t init_timer = qtimer_create();
    qtimer_t exec_timer = qtimer_create();

    // Allocate memory for 3-stage stencil (with boundary padding)
    qtimer_start(alloc_timer);
    stencil_t points;
    points.N = n + 2;
    points.M = m + 2;

    points.stage[0] = malloc(points.N*sizeof(aligned_t *));
    assert(NULL != points.stage[0]);
    points.stage[1] = malloc(points.N*sizeof(aligned_t *));
    assert(NULL != points.stage[1]);

    for (int i = 0; i < points.N; i++) {
        points.stage[0][i] = calloc(points.M, sizeof(aligned_t));
        assert(NULL != points.stage[0][i]);
        points.stage[1][i] = calloc(points.M, sizeof(aligned_t));
        assert(NULL != points.stage[1][i]);
    }
    qtimer_stop(alloc_timer);

    // Initialize first stage and set boundary conditions
    qtimer_start(init_timer);
    for (int i = 1; i < points.N-1; i++) {
        for (int j = 1; j < points.M-1; j++) {
            points.stage[0][i][j] = 0;
        }
    }
    for (int i = 0; i < points.N; i++) {
        points.stage[0][i][0] = BOUNDARY;
        points.stage[0][i][points.M-1] = BOUNDARY;
        points.stage[1][i][0] = BOUNDARY;
        points.stage[1][i][points.M-1] = BOUNDARY;
    }
    for (int j = 0; j < points.M; j++) {
        points.stage[0][0][j] = BOUNDARY;
        points.stage[0][points.N-1][j] = BOUNDARY;
        points.stage[1][0][j] = BOUNDARY;
        points.stage[1][points.N-1][j] = BOUNDARY;
    }
    qtimer_stop(init_timer);

    // Spawn tasks to start calculating updates at each point
    qtimer_start(exec_timer);
    rows_args_t args = {&points, 1};
    for (int t = 1; t <= num_timesteps; t++) {
        qt_loop(1, points.N-1, spawn_rows, &args);
        args.stage = next_stage(args.stage);
    }
    qtimer_stop(exec_timer);

    // Print timing info
    if (alltime) {
        fprintf(stderr, "Allocation time: %f\n", qtimer_secs(alloc_timer));
        fprintf(stderr, "Initialization time: %f\n", qtimer_secs(init_timer));
        fprintf(stderr, "Execution time: %f\n", qtimer_secs(exec_timer));
    } else {
        fprintf(stdout, "%f\n", qtimer_secs(exec_timer));
    }

    // Print stencils
    if (print_final) {
        size_t final = (num_timesteps % NUM_STAGES);
        iprintf("\nStage %lu:\n", prev_stage(final));
        print_stage(&points, prev_stage(final));
        iprintf("\nStage %lu:\n", final);
        print_stage(&points, final);
    }

    qtimer_destroy(alloc_timer);
    qtimer_destroy(init_timer);
    qtimer_destroy(exec_timer);

    // Free allocated memory
    for (int i = 0; i < points.N; i++) {
        free(points.stage[0][i]);
        free(points.stage[1][i]);
    }
    free(points.stage[0]);
    free(points.stage[1]);

    return 0;
}

/* vim:set expandtab */
