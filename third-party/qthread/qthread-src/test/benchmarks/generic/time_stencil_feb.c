#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/barrier.h>
#include <qthread/qtimer.h>

#include "argparsing.h"

#define NUM_STAGES 3
#define BOUNDARY 42
//#define BOUNDARY_SYNC

//#define TIME_WORKLOAD

#define NUM_NEIGHBORS 5
#if NUM_NEIGHBORS == 5
# define NORTH(stage,i,j) &stage[i-1][j  ]
# define WEST(stage,i,j)  &stage[i  ][j-1]
# define HERE(stage,i,j)  &stage[i  ][j  ]
# define EAST(stage,i,j)  &stage[i  ][j+1]
# define SOUTH(stage,i,j) &stage[i+1][j  ]
# define NEIGHBORS(stage,i,j) NORTH(stage,i,j), WEST(stage,i,j), EAST(stage,i,j), SOUTH(stage,i,j)
#endif // NUM_NEIGHBORS == 5

static int num_timesteps;

static int workload;
static int workload_per;
static int workload_var;

typedef struct stencil {
    size_t N;
    size_t M;
    aligned_t **stage[NUM_STAGES];
    qt_barrier_t *barrier;
} stencil_t;

typedef struct update_args {
    stencil_t *points;
    size_t i;
    size_t j;
    size_t stage;
    size_t step;
} update_args_t;

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
static aligned_t update_prime(void *arg)
{
    stencil_t *points = ((update_args_t *)arg)->points;
    size_t i = ((update_args_t *)arg)->i;
    size_t j = ((update_args_t *)arg)->j;
    size_t stage = ((update_args_t *)arg)->stage;
    size_t step = ((update_args_t *)arg)->step;

    size_t next_stage_id = next_stage(stage);

    // Sum all neighboring values from previous stage
    aligned_t **prev = points->stage[prev_stage(stage)];
#ifdef BOUNDARY_SYNC
    qthread_readFF(NORTH(prev, i, j), NORTH(prev, i, j));
    qthread_readFF(WEST(prev, i, j), WEST(prev, i, j));
    qthread_readFF(EAST(prev, i, j), EAST(prev, i, j));
    qthread_readFF(SOUTH(prev, i, j), SOUTH(prev, i, j));
#else
    if (i == 1) {                   // North edge
        if (j == 1) {                   // West edge: EAST & SOUTH
            qthread_readFF(EAST(prev, i, j), EAST(prev, i, j));
            qthread_readFF(SOUTH(prev, i, j), SOUTH(prev, i, j));
        } else if (j == points->M-2) {    // East edge: WEST & SOUTH
            qthread_readFF(WEST(prev, i, j), WEST(prev, i, j));
            qthread_readFF(SOUTH(prev, i, j), SOUTH(prev, i, j));
        } else                            // Interior: WEST & EAST & SOUTH
            qthread_readFF(WEST(prev, i, j), WEST(prev, i, j));
            qthread_readFF(EAST(prev, i, j), EAST(prev, i, j));
            qthread_readFF(SOUTH(prev, i, j), SOUTH(prev, i, j));
    } else if (i == points->N-2) {  // South edge
        if (j == 1) {                   // West edge: NORTH & EAST
            qthread_readFF(NORTH(prev, i, j), NORTH(prev, i, j));
            qthread_readFF(EAST(prev, i, j), EAST(prev, i, j));
        } else if (j == points->M-2) {    // East edge: NORTH & WEST
            qthread_readFF(NORTH(prev, i, j), NORTH(prev, i, j));
            qthread_readFF(WEST(prev, i, j), WEST(prev, i, j));
        } else                            // Interior: NORTH & WEST & EAST
            qthread_readFF(NORTH(prev, i, j), NORTH(prev, i, j));
            qthread_readFF(WEST(prev, i, j), WEST(prev, i, j));
            qthread_readFF(EAST(prev, i, j), EAST(prev, i, j));
    } else {                        // Interior
        if (j == 1) {                   // West edge: NORTH & EAST & SOUTH
            qthread_readFF(NORTH(prev, i, j), NORTH(prev, i, j));
            qthread_readFF(EAST(prev, i, j), EAST(prev, i, j));
            qthread_readFF(SOUTH(prev, i, j), SOUTH(prev, i, j));
        } else if (j == points->M-2) {    // East edge: NORTH & WEST & SOUTH
            qthread_readFF(NORTH(prev, i, j), NORTH(prev, i, j));
            qthread_readFF(WEST(prev, i, j), WEST(prev, i, j));
            qthread_readFF(SOUTH(prev, i, j), SOUTH(prev, i, j));
        } else                            // Interior: ALL
            qthread_readFF(NORTH(prev, i, j), NORTH(prev, i, j));
            qthread_readFF(WEST(prev, i, j), WEST(prev, i, j));
            qthread_readFF(EAST(prev, i, j), EAST(prev, i, j));
            qthread_readFF(SOUTH(prev, i, j), SOUTH(prev, i, j));
    }
#endif // BOUNDARY_SYNC

    // Perform local work
    perform_local_work();
    aligned_t sum = *(NORTH(prev, i, j)) 
                  + *(WEST(prev, i, j)) 
                  + *(HERE(prev, i, j)) 
                  + *(EAST(prev, i, j)) 
                  + *(SOUTH(prev, i, j));

    // Empty the next stage for this index
    qthread_empty(&points->stage[next_stage_id][i][j]);

    // Update this point
    qthread_writeEF_const(&points->stage[stage][i][j], sum/NUM_NEIGHBORS);
    
    if (step < num_timesteps) {
        // Spawn next stage
        update_args_t args = {points, i, j, next_stage_id, step+1};
        qthread_fork_syncvar_copyargs(update_prime, &args, sizeof(update_args_t), NULL);
    }
    else
        qt_barrier_enter(points->barrier);

    return 0;
}

static aligned_t update(void *arg)
{
    stencil_t *points = ((update_args_t *)arg)->points;
    size_t i = ((update_args_t *)arg)->i;
    size_t j = ((update_args_t *)arg)->j;
    size_t stage = ((update_args_t *)arg)->stage;
    size_t step = ((update_args_t *)arg)->step;

    size_t next_stage_id = next_stage(stage);

    // Sum all neighboring values from previous stage
    aligned_t **prev = points->stage[prev_stage(stage)];

    // Perform local work
    perform_local_work();
    aligned_t sum = *(NORTH(prev, i, j)) 
                  + *(WEST(prev, i, j)) 
                  + *(HERE(prev, i, j)) 
                  + *(EAST(prev, i, j)) 
                  + *(SOUTH(prev, i, j));

    // Empty the next stage for this index
    qthread_empty(&points->stage[next_stage_id][i][j]);

    // Update this point
    qthread_writeEF_const(&points->stage[stage][i][j], sum/NUM_NEIGHBORS);
    
    if (step < num_timesteps) {
        // Spawn next stage
        update_args_t args = {points, i, j, next_stage_id, step+1};
        qthread_fork_syncvar_copyargs(update_prime, &args, sizeof(update_args_t), NULL);
    }
    else
        qt_barrier_enter(points->barrier);

    return 0;
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
    NUMARG(n,"N");
    NUMARG(m,"M");
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

    points.stage[0] = calloc(points.N,sizeof(aligned_t *));
    assert(NULL != points.stage[0]);
    points.stage[1] = calloc(points.N,sizeof(aligned_t *));
    assert(NULL != points.stage[1]);
    points.stage[2] = calloc(points.N,sizeof(aligned_t *));
    assert(NULL != points.stage[2]);

    for (int i = 0; i < points.N; i++) {
        points.stage[0][i] = calloc(points.M, sizeof(aligned_t));
        assert(NULL != points.stage[0][i]);
        points.stage[1][i] = calloc(points.M, sizeof(aligned_t));
        assert(NULL != points.stage[1][i]);
        points.stage[2][i] = calloc(points.M, sizeof(aligned_t));
        assert(NULL != points.stage[2][i]);
    }
    qtimer_stop(alloc_timer);

    // Initialize first stage and set boundary conditions
    qtimer_start(init_timer);
    for (int i = 1; i < points.N-1; i++) {
        for (int j = 1; j < points.M-1; j++) {
            qthread_writeF_const(&points.stage[0][i][j], 0);
            qthread_empty(&points.stage[1][i][j]);
            qthread_empty(&points.stage[2][i][j]);
        }
    }
    for (int i = 0; i < points.N; i++) {
#ifdef BOUNDARY_SYNC
        qthread_writeF_const(&points.stage[0][i][0], BOUNDARY);
        qthread_writeF_const(&points.stage[0][i][points.M-1], BOUNDARY);
        qthread_writeF_const(&points.stage[1][i][0], BOUNDARY);
        qthread_writeF_const(&points.stage[1][i][points.M-1], BOUNDARY);
        qthread_writeF_const(&points.stage[2][i][0], BOUNDARY);
        qthread_writeF_const(&points.stage[2][i][points.M-1], BOUNDARY);
#else
        points.stage[0][i][0] =BOUNDARY;
        points.stage[0][i][points.M-1] = BOUNDARY;
        points.stage[1][i][0] = BOUNDARY;
        points.stage[1][i][points.M-1] = BOUNDARY;
        points.stage[2][i][0] = BOUNDARY;
        points.stage[2][i][points.M-1] = BOUNDARY;
#endif // BOUNDARY_SYNC
    }
    for (int j = 0; j < points.M; j++) {
#ifdef BOUNDARY_SYNC
        qthread_writeF_const(&points.stage[0][0][j], BOUNDARY);
        qthread_writeF_const(&points.stage[0][points.N-1][j], BOUNDARY);
        qthread_writeF_const(&points.stage[1][0][j], BOUNDARY);
        qthread_writeF_const(&points.stage[1][points.N-1][j], BOUNDARY);
        qthread_writeF_const(&points.stage[2][0][j], BOUNDARY);
        qthread_writeF_const(&points.stage[2][points.N-1][j], BOUNDARY);
#else
        points.stage[0][0][j] = BOUNDARY;
        points.stage[0][points.N-1][j] = BOUNDARY;
        points.stage[1][0][j] = BOUNDARY;
        points.stage[1][points.N-1][j] = BOUNDARY;
        points.stage[2][0][j] = BOUNDARY;
        points.stage[2][points.N-1][j] = BOUNDARY;
#endif // BOUNDARY_SYNC
    }
    qtimer_stop(init_timer);

    // Create barrier to synchronize on completion of calculations
    qtimer_start(exec_timer);
    points.barrier = qt_barrier_create(n*m+1, REGION_BARRIER);

    // Spawn tasks to start calculating updates at each point
    update_args_t args = {&points, -1, -1, 1, 1};
    for (int i = 1; i < points.N-1; i++) {
        for (int j = 1; j < points.M-1; j++) {
            args.i = i;
            args.j = j;
            qthread_fork_syncvar_copyargs(update, &args, sizeof(update_args_t), NULL);
        }
    }

    // Wait for calculations to finish
    qt_barrier_enter(points.barrier);

    // Stop timer
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
        iprintf("Stage %lu:\n", prev_stage(prev_stage(final)));
        print_stage(&points, prev_stage(prev_stage(final)));
        iprintf("\nStage %lu:\n", prev_stage(final));
        print_stage(&points, prev_stage(final));
        iprintf("\nStage %lu:\n", final);
        print_stage(&points, final);
    }

    qt_barrier_destroy(points.barrier);
    qtimer_destroy(alloc_timer);
    qtimer_destroy(init_timer);
    qtimer_destroy(exec_timer);

    // Free allocated memory
    for (int i = 0; i < points.N; i++) {
        free(points.stage[0][i]);
        free(points.stage[1][i]);
        free(points.stage[2][i]);
    }
    free(points.stage[0]);
    free(points.stage[1]);
    free(points.stage[2]);

    return 0;
}

/* vim:set expandtab */
