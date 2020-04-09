#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/qtimer.h>

#define DEBUG 1
#ifdef DEBUG
#include <qthread/barrier.h>
#endif

#include "argparsing.h"

// Configuration constants
/*{{{*/
#define NUM_NEIGHBORS 5
#define NUM_STAGES 2
#define BOUNDARY 42
/*}}}*/

static int num_timesteps;

// Initial settings of point values and statuses
static const syncvar_t boundary_value = SYNCVAR_STATIC_INITIALIZE_TO(BOUNDARY);
#ifdef DEBUG
static const syncvar_t internal_value = SYNCVAR_STATIC_INITIALIZE_TO(10);
#define INTERNAL_POINT internal_value
#else
#define INTERNAL_POINT    SYNCVAR_INITIALIZER
#endif
#define GHOST_POINT_EMPTY SYNCVAR_EMPTY_INITIALIZER
#define GHOST_POINT_FULL  SYNCVAR_INITIALIZER
#define EDGE_POINT_EMPTY  SYNCVAR_EMPTY_INITIALIZER
#define EDGE_POINT_FULL   SYNCVAR_INITIALIZER

// Stencil and partition and position data structures
/*{{{*/
typedef enum position {
    NW=0, NORTH, NE, WEST, CENTER, EAST, SW, SOUTH, SE, 
    COL_NORTH, COL_SOUTH, COL_CENTER, 
    ROW_WEST, ROW_EAST, ROW_CENTER,
    NWES
} position_t;

static char *pos_strs[16] = {
    "NW", "NORTH", "NE", "WEST", "CENTER", "EAST", "SW", "SOUTH", "SE",
    "COL-NORTH", "COL-SOUTH", "COL-CENTER",
    "ROW-WEST", "ROW-EAST", "ROW-CENTER",
    "NWES"};

typedef struct partition_s {
    Q_ALIGNED(8) position_t pos;    // Position of this partition in matrix
    size_t row;                     // Row index in partition matrix
    size_t col;                     // Col index in partition matrix
    size_t nrows;                   // Number of rows in this partition
    size_t ncols;                   // Number of cols in this partition
    size_t brows;                   // Number of row blocks
    size_t bcols;                   // Number of col blocks
    syncvar_t **stages[NUM_STAGES]; // The local stencil points
} partition_t;

typedef struct stencil_s {
    size_t nrows;         // Number of rows in stencil
    size_t ncols;         // Number of cols in stencil
    size_t prows;         // Number of rows in partition matrix
    size_t pcols;         // Number of cols in partition matrix
    size_t bsize;         // Size of a message block
    partition_t **parts;  // The stencil partitions
} stencil_t;
/*}}}*/

#ifdef DEBUG
static qt_barrier_t *debug_barrier;
#endif

////////////////////////////////////////////////////////////////////////////////
// Helpers

// Position-related helper stuff
/*{{{*/
#define NORTH_OF(stage, i, j) stage[i+1][j  ]
#define WEST_OF(stage, i, j)  stage[i  ][j-1]
#define EAST_OF(stage, i, j)  stage[i  ][j+1]
#define SOUTH_OF(stage, i, j) stage[i-1][j  ]

#define GHOST_NORTH(pos) (WEST == pos || CENTER == pos || EAST == pos || \
                          SW == pos || SOUTH == pos || SE == pos || \
                          COL_CENTER == pos || COL_SOUTH == pos)
#define GHOST_WEST(pos) (NORTH == pos || NE == pos || CENTER == pos || \
                         EAST == pos || SOUTH == pos || SE == pos || \
                         ROW_CENTER == pos || ROW_EAST == pos)
#define GHOST_EAST(pos) (NW == pos || NORTH == pos || WEST == pos || \
                         CENTER == pos || SW == pos || SOUTH == pos || \
                         ROW_WEST == pos || ROW_CENTER == pos)
#define GHOST_SOUTH(pos) (NW == pos || NORTH == pos || NE == pos || \
                          WEST == pos || CENTER == pos || EAST == pos || \
                          COL_NORTH == pos || COL_CENTER == pos)

#define HAS_NW_CORNER(pos) (CENTER == pos || \
                           EAST == pos || SOUTH == pos || SE == pos)
#define HAS_SW_CORNER(pos) (CENTER == pos || \
                           NORTH == pos || NE == pos || EAST == pos)
#define HAS_NE_CORNER(pos) (CENTER == pos || \
                           WEST == pos || SW == pos || SOUTH == pos)
#define HAS_SE_CORNER(pos) (CENTER == pos || \
                           NW == pos || NORTH == pos || WEST == pos)
/*}}}*/

// Syncvar-related stuff
#define SYNCVAR_EVAL(x) INT60TOINT64(x.u.s.data)
#define SYNCVAR_BIND(x,v) x.u.s.data = INT64TOINT60(v)
#define SYNCVAR_COPY(dest,src) dest.u.s.data = src.u.s.data;

static inline void print_stencil(stencil_t *stencil, size_t step)
{/*{{{*/
    fprintf(stderr, "Stencil:\n");
    fprintf(stderr, "\tpoints:     %lu x %lu\n",stencil->nrows,stencil->ncols);
    fprintf(stderr, "\tpartitions: %lu x %lu\n",stencil->prows,stencil->pcols);

    const size_t num_parts = stencil->prows * stencil->pcols;
    for (int pi = 0; pi < num_parts; pi++) {
        const partition_t *part = stencil->parts[pi];
        fprintf(stderr, "\tPartition: (%lu,%lu) %s, %lu x %lu, %lu x %lu\n",
            part->row, part->col, pos_strs[part->pos], part->nrows,part->ncols,
            part->brows, part->bcols);
        for (int i = part->nrows-1; i >= 0; i--) {
            fprintf(stderr, "\t\t%02lu", 
                (unsigned long)SYNCVAR_EVAL(part->stages[step][i][0]));
            for (int j = 1; j < part->ncols; j++) {
                fprintf(stderr, " %02lu", 
                    (unsigned long)SYNCVAR_EVAL(part->stages[step][i][j]));
            }
            fprintf(stderr, "\n");
        }
    }
}/*}}}*/

static inline void print_status(stencil_t *stencil, size_t step)
{/*{{{*/
    fprintf(stderr, "Stencil:\n");
    fprintf(stderr, "\tpoints:     %lu x %lu\n",stencil->nrows,stencil->ncols);
    fprintf(stderr, "\tpartitions: %lu x %lu\n",stencil->prows,stencil->pcols);

    const size_t num_parts = stencil->prows * stencil->pcols;
    for (int pi = 0; pi < num_parts; pi++) {
        const partition_t *part = stencil->parts[pi];
        fprintf(stderr, "\tPartition: (%lu,%lu) %s, %lu x %lu, %lu x %lu\n",
            part->row, part->col, pos_strs[part->pos], part->nrows,part->ncols,
            part->brows, part->bcols);
        for (int i = part->nrows-1; i >= 0; i--) {
            fprintf(stderr, "\t\t%d", 
                qthread_syncvar_status(&part->stages[step][i][0]));
            for (int j = 1; j < part->ncols; j++) {
                fprintf(stderr, " %d", 
                    qthread_syncvar_status(&part->stages[step][i][j]));
            }
            fprintf(stderr, "\n");
        }
    }
}/*}}}*/

static inline size_t prev_stage(size_t stage)
{ /*{{{*/
    return (stage == 0) ? NUM_STAGES-1 : stage - 1;
} /*}}}*/

static inline size_t next_stage(size_t stage)
{ /*{{{*/
    return (stage == NUM_STAGES-1) ? 0 : stage + 1;
} /*}}}*/

// Find the position of the partition in the partition matrix
static inline void get_position(stencil_t *points, partition_t *part)
{ /*{{{*/
    const size_t i = part->row;
    const size_t j = part->col;
    const size_t prows = points->prows;
    const size_t pcols = points->pcols;

    if (prows > 1 && pcols > 1) {
        if (i == 0)
            if (j == 0)
                part->pos = SW;
            else if (j == pcols-1)
                part->pos = SE;
            else
                part->pos = SOUTH;
        else if (i == prows-1)
            if (j == 0)
                part->pos = NW;
            else if (j == pcols-1)
                part->pos = NE;
            else
                part->pos = NORTH;
        else
            if (j == 0)
                part->pos = WEST;
            else if (j == pcols-1)
                part->pos = EAST;
            else
                part->pos = CENTER;
    } else if (prows == 1 && pcols > 1) {
        if (j == 0)
            part->pos = ROW_WEST;
        else if (j == pcols-1)
            part->pos = ROW_EAST;
        else
            part->pos = ROW_CENTER;
    } else if (pcols == 1 && prows > 1) {
        if (i == 0)
            part->pos = COL_SOUTH;
        else if (i == prows-1)
            part->pos = COL_NORTH;
        else
            part->pos = COL_CENTER;
    } else {
        part->pos = NWES;
    }
} /*}}}*/

// Logical-to-physical mapping assumes point (0,0) is in bottom left corner.
static inline void get_pid(size_t lid, size_t ncols, size_t *row, size_t *col)
{/*{{{*/
    *row = lid / ncols;
    *col = lid - (*row * ncols);
}/*}}}*/

static inline size_t get_lid(size_t row, size_t col, size_t ncols)
{/*{{{*/
    return (row * ncols) + col;
}/*}}}*/

////////////////////////////////////////////////////////////////////////////////
// Stencil point update code

typedef struct upx_args_s {
    syncvar_t            ***stages;
    size_t                  now;
    size_t                  timestep;
    Q_ALIGNED(8) position_t dir;
    Q_ALIGNED(8) position_t pos;
    size_t                  num_rows;
    size_t                  num_cols;
    size_t                  part_lid;
} upx_args_t;

typedef struct upi_args_s {
    syncvar_t ***stages;
    size_t       now;
    size_t       col;
} upi_args_t;

// Update internal point
/*static void update_point_internal(const size_t start, const size_t stop,
                                  void *arg_)
{
    const upi_args_t *arg = (upi_args_t *)arg_;
    syncvar_t ***stages = arg->stages;
    const size_t now    = arg->now;
    const size_t i      = start;
    const size_t j      = arg->col;

    syncvar_t **S = stages[prev_stage(now)];

    const uint64_t sum = SYNCVAR_EVAL(S[i][j]) +
                         SYNCVAR_EVAL(NORTH_OF(S,i,j)) +
                         SYNCVAR_EVAL(SOUTH_OF(S,i,j)) +
                         SYNCVAR_EVAL(WEST_OF(S,i,j)) +
                         SYNCVAR_EVAL(EAST_OF(S,i,j));

    SYNCVAR_BIND(stages[now][i][j], sum/NUM_NEIGHBORS);
}*/

// Spawn internal point tasks over columns
/*static void update_point_internal_loop(const size_t start, const size_t stop,
                                       void *arg_)
{
    const upx_args_t *arg = (upx_args_t *)arg_;
    const size_t num_rows = arg->num_rows;
    const position_t pos  = arg->pos;

    const size_t col_start = GHOST_SOUTH(pos) ? 2 : 1;
    const size_t col_stop  = GHOST_NORTH(pos) ? num_rows-2 : num_rows-1;

    upi_args_t upi_args = {arg->stages, arg->now, start};
    qt_loop(col_start, col_stop, update_point_internal, &upi_args);
}*/

typedef struct cpc_args_s {
    syncvar_t *prev;
    syncvar_t *curr;
    syncvar_t *neighbor1;
    syncvar_t *neighbor2;
} cpc_args_t;

/*
 * Pre:
 * - now neighbors are full
 * - now this point is full
 *
 * Post:
 * - now this point is empty
 */
static aligned_t clear_point_corner(void *arg_)
{
    cpc_args_t *arg = (cpc_args_t *)arg_;

    qthread_syncvar_readFF(NULL, arg->neighbor1);
    qthread_syncvar_readFF(NULL, arg->neighbor2);
    qthread_syncvar_readFF(NULL, arg->curr);

    qthread_syncvar_readFE(NULL, arg->prev);

    return 0;
}

/*
 * Pre:
 * - prev ghost-point neighbors are full
 * - prev edge-point neighbors are full
 * - now this edge-point is empty
 *
 * Post:
 * - now this edge-point is full
 *
 * Also spawns task to clear this prev edge-point.
 */
static aligned_t update_point_corner(void *arg_)
{
    const upx_args_t *arg = (upx_args_t *)arg_;
    syncvar_t ***stages   = arg->stages;
    const position_t dir  = arg->dir;
    size_t timestep = arg->timestep;
    const size_t num_rows = arg->num_rows;
    const size_t num_cols = arg->num_cols;
    //const size_t part_lid = arg->part_lid;

    size_t i       = 0;
    size_t j       = 0;
    uint64_t sum   = 0;
    uint64_t value = 0;

    cpc_args_t cpc_args;

    size_t prev_stage_id = prev_stage(arg->now);
    size_t this_stage_id = prev_stage_id;

    while (timestep <= num_timesteps) {
        prev_stage_id = this_stage_id;
        this_stage_id = next_stage(this_stage_id);

        syncvar_t **prev = stages[prev_stage_id];
        syncvar_t **curr  = stages[this_stage_id];

        switch (dir) {
            case NW: 
                i = num_rows - 2;
                j = 1;
                // Clear E & S
                cpc_args.prev = &prev[i][j];
                cpc_args.curr = &curr[i][j];
                cpc_args.neighbor1 = &EAST_OF(curr,i,j);
                cpc_args.neighbor2 = &SOUTH_OF(curr,i,j);
                qthread_fork_copyargs(clear_point_corner,
                    &cpc_args,sizeof(cpc_args_t),NULL);
                break;
            case NE:
                i = num_rows - 2;
                j = num_cols - 2;
                cpc_args.prev = &prev[i][j];
                cpc_args.curr = &curr[i][j];
                cpc_args.neighbor1 = &WEST_OF(curr,i,j);
                cpc_args.neighbor2 = &SOUTH_OF(curr,i,j);
                qthread_fork_copyargs(clear_point_corner,
                    &cpc_args,sizeof(cpc_args_t),NULL);
                break;
            case SW:
                i = 1;
                j = 1;
                cpc_args.prev = &prev[i][j];
                cpc_args.curr = &curr[i][j];
                cpc_args.neighbor1 = &EAST_OF(curr,i,j);
                cpc_args.neighbor2 = &NORTH_OF(curr,i,j);
                qthread_fork_copyargs(clear_point_corner,
                    &cpc_args,sizeof(cpc_args_t),NULL);
                break;
            case SE:
                i = 1;
                j = num_cols - 2;
                cpc_args.prev = &prev[i][j];
                cpc_args.curr = &curr[i][j];
                cpc_args.neighbor1 = &WEST_OF(curr,i,j);
                cpc_args.neighbor2 = &NORTH_OF(curr,i,j);
                qthread_fork_copyargs(clear_point_corner,
                    &cpc_args,sizeof(cpc_args_t),NULL);
                break;
            default:
                abort();
                break;
        }

        iprintf("\tCLEARED CORNER\n");
        sum = SYNCVAR_EVAL(prev[i][j]);
        qthread_syncvar_readFE(&value, &NORTH_OF(prev,i,j)); // Ghost
        sum += value;
        qthread_syncvar_readFE(&value, &WEST_OF(prev,i,j)); // Ghost
        sum += value;
        qthread_syncvar_readFF(&value, &SOUTH_OF(prev,i,j)); // Edge
        sum += value;
        qthread_syncvar_readFF(&value, &EAST_OF(prev,i,j)); // Edge
        sum += value;
        qthread_syncvar_writeEF_const(&curr[i][j],sum/NUM_NEIGHBORS);
        iprintf("\tDONE CORNER\n");

        timestep += 1;
    }

#ifdef DEBUG
    qt_barrier_enter(debug_barrier);
#endif
    return 0;
}

/*
 * Pre:
 * - prev edge-point neighbors are full
 * - prev ghost-point neighbor is full
 * - now this point is empty
 *
 * Post:
 * - now this point is full
 */
static void update_point_edge(const size_t start, const size_t stop, void *arg_)
{
    const upx_args_t *arg = (upx_args_t *)arg_;
    syncvar_t ***stages = arg->stages;
    const position_t dir = arg->dir;
    const size_t num_rows = arg->num_rows;
    const size_t num_cols = arg->num_cols;

    size_t prev_stage_id = prev_stage(arg->now);
    size_t curr_stage_id = arg->now;

    syncvar_t **prev = stages[prev_stage_id];
    syncvar_t **curr = stages[curr_stage_id];

    size_t i       = 0;
    size_t j       = 0;
    uint64_t value = 0;
    uint64_t sum   = 0;

    cpc_args_t cpc_args;

    switch (dir) {
        case NORTH:
            i = num_rows-2;
            j = start;

            cpc_args.prev = &prev[i][j];
            cpc_args.curr = &curr[i][j];
            cpc_args.neighbor1 = &SOUTH_OF(curr,i,j);
            cpc_args.neighbor2 = &NORTH_OF(curr,i,j);
            qthread_fork_copyargs(clear_point_corner,
                &cpc_args,sizeof(cpc_args_t),NULL);

            sum = SYNCVAR_EVAL(prev[i][j]);
            qthread_syncvar_readFE(&value, &NORTH_OF(prev,i,j)); // Ghost
            sum += value;
            qthread_syncvar_readFF(&value, &WEST_OF(prev,i,j));
            sum += value;
            qthread_syncvar_readFF(&value, &EAST_OF(prev,i,j));
            sum += value;

            sum += SYNCVAR_EVAL(SOUTH_OF(prev,i,j));

            break;
        case SOUTH:
            i = 1;
            j = start;

            cpc_args.prev = &prev[i][j];
            cpc_args.curr = &curr[i][j];
            cpc_args.neighbor1 = &SOUTH_OF(curr,i,j);
            cpc_args.neighbor2 = &NORTH_OF(curr,i,j);
            qthread_fork_copyargs(clear_point_corner,
                &cpc_args,sizeof(cpc_args_t),NULL);

            sum = SYNCVAR_EVAL(prev[i][j]);
            qthread_syncvar_readFF(&value, &WEST_OF(prev,i,j));
            sum += value;
            qthread_syncvar_readFE(&value, &SOUTH_OF(prev,i,j)); // Ghost
            sum += value;
            qthread_syncvar_readFF(&value, &EAST_OF(prev,i,j));
            sum += value;

            sum += SYNCVAR_EVAL(NORTH_OF(prev,i,j));
            break;
        case WEST:
            i = start;
            j = 1;

            cpc_args.prev = &prev[i][j];
            cpc_args.curr = &curr[i][j];
            cpc_args.neighbor1 = &WEST_OF(curr,i,j);
            cpc_args.neighbor2 = &EAST_OF(curr,i,j);
            qthread_fork_copyargs(clear_point_corner,
                &cpc_args,sizeof(cpc_args_t),NULL);

            sum = SYNCVAR_EVAL(prev[i][j]);
            qthread_syncvar_readFF(&value, &NORTH_OF(prev,i,j));
            sum += value;
            qthread_syncvar_readFE(&value, &WEST_OF(prev,i,j)); // Ghost
            sum += value;
            qthread_syncvar_readFF(&value, &SOUTH_OF(prev,i,j));
            sum += value;

            sum += SYNCVAR_EVAL(EAST_OF(prev,i,j));
            break;
        case EAST:
            i = start;
            j = num_cols - 2;

            cpc_args.prev = &prev[i][j];
            cpc_args.curr = &curr[i][j];
            cpc_args.neighbor1 = &WEST_OF(curr,i,j);
            cpc_args.neighbor2 = &EAST_OF(curr,i,j);
            qthread_fork_copyargs(clear_point_corner,
                &cpc_args,sizeof(cpc_args_t),NULL);

            sum = SYNCVAR_EVAL(prev[i][j]);
            qthread_syncvar_readFF(&value, &NORTH_OF(prev,i,j));
            sum += value;
            qthread_syncvar_readFF(&value, &SOUTH_OF(prev,i,j));
            sum += value;
            qthread_syncvar_readFE(&value, &EAST_OF(prev,i,j)); // Ghost
            sum += value;

            sum += SYNCVAR_EVAL(WEST_OF(prev,i,j));
            break;
        default:
            abort();
            break;
    }
    
    qthread_syncvar_writeEF_const(&curr[i][j],sum/NUM_NEIGHBORS);
}

static aligned_t update_point_edge_loop(void *arg_)
{
    upx_args_t *arg = (upx_args_t *)arg_;
    const position_t dir = arg->dir;
    const position_t pos = arg->pos;
    size_t now      = arg->now;
    size_t timestep = arg->timestep;
    const size_t num_rows = arg->num_rows;
    const size_t num_cols = arg->num_cols;
    //const size_t part_lid = arg->part_lid;

    size_t next = next_stage(now);

    while (timestep <= num_timesteps) {

        arg->now = now;
        arg->timestep = timestep;

        switch (dir) {
            case NORTH:
            {
                const size_t lb = GHOST_WEST(pos) ? 2 : 1;
                const size_t ub = GHOST_EAST(pos) ? num_cols-2 : num_cols-1;

                qt_loop(lb, ub, update_point_edge, arg);
                break;
            }
            case SOUTH:
            {
                const size_t lb = GHOST_WEST(pos) ? 2 : 1;
                const size_t ub = GHOST_EAST(pos) ? num_cols-2 : num_cols-1;

                qt_loop(lb, ub, update_point_edge, arg);
                break;
            }
            case WEST:
            {
                const size_t lb = GHOST_SOUTH(pos) ? 2 : 1;
                const size_t ub = GHOST_NORTH(pos) ? num_rows-2 : num_rows-1;

                qt_loop(lb, ub, update_point_edge, arg);
                break;
            }
            case EAST:
            {
                const size_t lb = GHOST_SOUTH(pos) ? 2 : 1;
                const size_t ub = GHOST_NORTH(pos) ? num_rows-2 : num_rows-1;

                qt_loop(lb, ub, update_point_edge, arg);
                break;
            }
            default:
                abort();
        }

        timestep += 1;
        now = next;
        next = next_stage(now);
    }

#ifdef DEBUG
    qt_barrier_enter(debug_barrier);
#endif

    return 0;
}

typedef struct us_args_s {
    size_t part_lid;
    size_t timestep;
    partition_t *part;
    const stencil_t *points;
} us_args_t;

/*
* - tasks for updating all points are spawned when called
* - this task does not return until all 
*/
/*static aligned_t update_stage(void *arg_)
{
    const us_args_t *arg = (us_args_t *)arg_;
    const position_t pos = arg->part->pos;
    const size_t num_rows = arg->part->nrows;
    const size_t num_cols = arg->part->ncols;
    const size_t now      = arg->timestep % 2;

    syncvar_t ***const stages = arg->part->stages;

    syncvar_t rets[8];
    for (int i = 0; i < 8; i++)
        rets[i] = SYNCVAR_INITIALIZER;

    upx_args_t upx_args = {stages, now, arg->timestep, 0, pos, num_rows, num_cols, arg->part_lid};

    // Process internal points
    upx_args.dir = 0;
    upx_args.pos = pos;

    const size_t start = GHOST_WEST(pos) ? 2 : 1;
    const size_t stop  = GHOST_EAST(pos) ? num_cols-2 : num_cols-1;
    qt_loop(start, stop, update_point_internal_loop, &upx_args);

    // Wait for corner and edge cases to finish
    for (int i = 0; i < 8; i++)
        qthread_syncvar_readFF(NULL, &rets[i]);

    return 0;
}*/

////////////////////////////////////////////////////////////////////////////////
// Halo exchange

typedef struct sb_args_s {
    syncvar_t ***target;
    syncvar_t ***source;
    Q_ALIGNED(8) position_t dir;
    size_t stage;
    size_t timestep;
    size_t lb;
    size_t ub;
    size_t sk;
    size_t tk;
    size_t part_lid;
} sb_args_t;

typedef struct sbc_args_s {
    syncvar_t *const target1;
    syncvar_t *const target2;
    syncvar_t *const source;
} sbc_args_t;

typedef struct su_args_s {
    size_t part_lid;
    const stencil_t *points;
} su_args_t;

/*
 * Pre:
 * - now this edge-point is full
 * - now ghost-point is empty
 *
 * Post:
 * - now ghost-point is full
 */
static aligned_t send_block(void *arg_) {
    sb_args_t *arg = (sb_args_t *)arg_;
    const position_t dir = arg->dir;
    size_t curr_stage_id = arg->stage;
    size_t timestep = arg->timestep;
    const size_t lb = arg->lb;
    const size_t ub = arg->ub;
    const size_t sk  = arg->sk;
    const size_t tk  = arg->tk;
    //const size_t part_lid = arg->part_lid;

    syncvar_t ***local  = arg->source;
    syncvar_t ***remote = arg->target;

    uint64_t value;

    switch (dir) {
        case NORTH:
            while (timestep <= num_timesteps) {
                // Before the (remote) ghost-point can be updated, it must be
                // empty, and the (local) edge-point must be full.
                for (int j = lb; j < ub; j++) {
                    syncvar_t *const ep = &local[curr_stage_id][sk][j];
                    qthread_syncvar_readFF(&value, ep);

                    syncvar_t *const gp = &remote[curr_stage_id][tk][j];
                    qthread_syncvar_writeEF(gp, &value);
                }

                timestep += 1;
                curr_stage_id = next_stage(curr_stage_id);
            }
            break;
        case SOUTH:
            while (timestep <= num_timesteps) {
                for (int j = lb; j < ub; j++) {
                    syncvar_t *const ep = &local[curr_stage_id][sk][j];
                    qthread_syncvar_readFF(&value, ep);

                    syncvar_t *const gp = &remote[curr_stage_id][tk][j];
                    qthread_syncvar_writeEF(gp, &value);
                }

                timestep += 1;
                curr_stage_id = next_stage(curr_stage_id);
            }
            break;
        case WEST:
            while (timestep <= num_timesteps) {
                for (int i = lb; i < ub; i++) {
                    syncvar_t *const ep = &local[curr_stage_id][i][sk];
                    qthread_syncvar_readFF(&value, ep);

                    syncvar_t *const gp = &remote[curr_stage_id][i][tk];
                    qthread_syncvar_writeEF(gp, &value);
                }

                timestep += 1;
                curr_stage_id = next_stage(curr_stage_id);
            }
            break;
        case EAST:
            while (timestep <= num_timesteps) {
                for (int i = lb; i < ub; i++) {
                    syncvar_t *const ep = &local[curr_stage_id][i][sk];
                    qthread_syncvar_readFF(&value, ep);

                    syncvar_t *const gp = &remote[curr_stage_id][i][tk];
                    qthread_syncvar_writeEF(gp, &value);
                }

                timestep += 1;
                curr_stage_id = next_stage(curr_stage_id);
            }
            break;
        default:
            abort();
            break;
    }

#ifdef DEBUG
    qt_barrier_enter(debug_barrier);
#endif
    return 0;
}

static aligned_t send_updates(void *arg_)
{
    const su_args_t *arg = (su_args_t *)arg_;
    const size_t src_lid    = arg->part_lid;
    const stencil_t *points = arg->points;

    partition_t *src_part  = points->parts[src_lid];
    const position_t   src_pos   = src_part->pos;
    syncvar_t       ***src_stage = src_part->stages;
    const size_t       src_nrows = src_part->nrows;
    const size_t       src_ncols = src_part->ncols;

    const size_t bsize  = points->bsize;

    const size_t stage = 0;
    const size_t timestep = 1;

    // Send updates along edges
    if (GHOST_NORTH(src_pos)) {
        const size_t tgt_lid = 
            get_lid(src_part->row + 1, src_part->col, points->pcols);
        partition_t *tgt_part   = points->parts[tgt_lid];
        syncvar_t ***tgt_stage   = tgt_part->stages;

        const size_t lb = 1;
        const size_t ub = src_ncols-1;
        for (size_t j = lb; j < ub; j+=bsize) {
            sb_args_t sb_args;
            sb_args.stage = stage;
            sb_args.timestep = timestep;
            sb_args.target = tgt_stage;
            sb_args.source = src_stage;
            sb_args.dir    = NORTH;
            sb_args.lb     = j;
            sb_args.ub     = j + bsize;
            sb_args.sk     = src_nrows - 2;
            sb_args.tk     = 0;
            sb_args.part_lid = src_lid;
            qthread_fork_copyargs(send_block, &sb_args, 
                sizeof(sb_args_t), NULL);
        }
    }
    if (GHOST_SOUTH(src_pos)) {
        const size_t tgt_lid = 
            get_lid(src_part->row - 1, src_part->col, points->pcols);
        partition_t *tgt_part   = points->parts[tgt_lid];
        syncvar_t  ***tgt_stage  = tgt_part->stages;
        const size_t       tgt_nrows  = tgt_part->nrows;

        const size_t lb = 1;
        const size_t ub = src_ncols-1;
        for (size_t j = lb; j < ub; j+=bsize) {
            sb_args_t sb_args;
            sb_args.stage = stage;
            sb_args.timestep = timestep;
            sb_args.target = tgt_stage;
            sb_args.source = src_stage;
            sb_args.dir    = SOUTH;
            sb_args.lb     = j;
            sb_args.ub     = j + bsize;
            sb_args.sk     = 1;
            sb_args.tk     = tgt_nrows-1;
            sb_args.part_lid = src_lid;
            qthread_fork_copyargs(send_block, &sb_args, 
                sizeof(sb_args_t), NULL);
        }
    }
    if (GHOST_WEST(src_pos)) {
        const size_t tgt_lid = 
            get_lid(src_part->row, src_part->col-1, points->pcols);
        partition_t *tgt_part   = points->parts[tgt_lid];
        syncvar_t  ***tgt_stage  = tgt_part->stages;
        const size_t       tgt_ncols  = tgt_part->ncols;

        const size_t lb = 1;
        const size_t ub = src_nrows-1;
        for (size_t i = lb; i < ub; i+=bsize) {
            sb_args_t sb_args;
            sb_args.stage = stage;
            sb_args.timestep = timestep;
            sb_args.target = tgt_stage;
            sb_args.source = src_stage;
            sb_args.dir    = WEST;
            sb_args.lb     = i;
            sb_args.ub     = i + bsize;
            sb_args.sk     = 1;
            sb_args.tk     = tgt_ncols-1;
            sb_args.part_lid = src_lid;
            qthread_fork_copyargs(send_block, &sb_args, 
                sizeof(sb_args_t), NULL);
        }
    }
    if (GHOST_EAST(src_pos)) {
        const size_t tgt_lid = 
            get_lid(src_part->row, src_part->col+1, points->pcols);
        partition_t *tgt_part   = points->parts[tgt_lid];
        syncvar_t  ***tgt_stage  = tgt_part->stages;

        const size_t lb = 1;
        const size_t ub = src_nrows-1;
        for (size_t i = lb; i < ub; i+=bsize) {
            sb_args_t sb_args;
            sb_args.stage = stage;
            sb_args.timestep = timestep;
            sb_args.target = tgt_stage;
            sb_args.source = src_stage;
            sb_args.dir    = EAST;
            sb_args.lb     = i;
            sb_args.ub     = i + bsize;
            sb_args.sk     = src_ncols-2;
            sb_args.tk     = 0;
            sb_args.part_lid = src_lid;
            qthread_fork_copyargs(send_block, &sb_args, 
                sizeof(sb_args_t), NULL);
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Local work
/*
 *  -   Starts off local processing for a partition
 */
static void begin(const size_t start, const size_t stop, void *arg_)
{/*{{{*/
    stencil_t *arg = (stencil_t *)arg_;

    const size_t part_lid = start;

    // Send outgoing values to neighboring ghost cells
    const su_args_t su_args = {part_lid, arg};
    qthread_fork(send_updates, &su_args, NULL);

    // Setup update actions over corners and edges
    partition_t *part = arg->parts[part_lid];
    position_t pos = part->pos;

    upx_args_t upx_args;
    upx_args.stages = part->stages;
    upx_args.now = 1;
    upx_args.timestep = 1;
    upx_args.dir = NW;
    upx_args.pos = part->pos;
    upx_args.num_rows = part->nrows;
    upx_args.num_cols = part->ncols;

    // Process (up to) four corner cases
    if (HAS_NW_CORNER(pos)) {
        upx_args.dir = NW;
        qthread_fork_syncvar_copyargs(update_point_corner, 
            &upx_args, sizeof(upx_args), NULL);
    }
    if (HAS_NE_CORNER(pos)) {
        upx_args.dir = NE;
        qthread_fork_syncvar_copyargs(update_point_corner, 
            &upx_args, sizeof(upx_args), NULL);
    }
    if (HAS_SW_CORNER(pos)) {
        upx_args.dir = SW;
        qthread_fork_syncvar_copyargs(update_point_corner, 
            &upx_args, sizeof(upx_args), NULL);
    }
    if (HAS_SE_CORNER(pos)) {
        upx_args.dir = SE;
        qthread_fork_syncvar_copyargs(update_point_corner, 
            &upx_args, sizeof(upx_args), NULL);
    }

    // Process edge cases
    if (GHOST_NORTH(pos)) {
        upx_args.dir = NORTH;
        qthread_fork_syncvar_copyargs(
			update_point_edge_loop, &upx_args, sizeof(upx_args_t), NULL);
    }
    if (GHOST_SOUTH(pos)) {
        upx_args.dir = SOUTH;
        qthread_fork_syncvar_copyargs(
			update_point_edge_loop, &upx_args, sizeof(upx_args_t), NULL);
    }
    if (GHOST_WEST(pos)) {
        upx_args.dir = WEST;
        qthread_fork_syncvar_copyargs(
			update_point_edge_loop, &upx_args, sizeof(upx_args_t), NULL);
    }
    if (GHOST_EAST(pos)) {
        upx_args.dir = EAST;
        qthread_fork_syncvar_copyargs(
			update_point_edge_loop, &upx_args, sizeof(upx_args_t), NULL);
    }

    // Process internal points
    //for (size_t t = 1; t <= num_timesteps; t++) {
    //    // Compute a step
    //    const us_args_t us_args = {part_lid, t, arg->parts[part_lid], arg};
    //    syncvar_t up_ret = SYNCVAR_EMPTY_INITIALIZER;
    //    qthread_fork_syncvar(update_stage, &us_args, &up_ret);
    //    qthread_syncvar_readFF(NULL, &up_ret);
    //}

#ifdef DEBUG
    qt_barrier_enter(debug_barrier);
    if (part_lid == 0) {
        print_status(arg, 0 % 2);
        print_status(arg, 1 % 2);
    }
#endif

}/*}}}*/

////////////////////////////////////////////////////////////////////////////////
// Stencil setup and tear down

typedef struct ss_args_s {
    stencil_t *points;
    size_t *row_splits;
    size_t *col_splits;
} ss_args_t;

static void setup_stencil(const size_t start, const size_t stop, void *arg_)
{/*{{{*/
    ss_args_t *arg = (ss_args_t *)arg_;
    stencil_t *points = arg->points;
    size_t *row_splits = arg->row_splits;
    size_t *col_splits = arg->col_splits;

    const size_t part_lid = start;

    points->parts[part_lid] = malloc(sizeof(partition_t));
    assert(points->parts[part_lid]);

    partition_t *part = points->parts[part_lid];

    // Calculate position
    get_pid(part_lid, points->pcols, &part->row, &part->col);
    get_position(points, part);

    // Setup partition info
    const double row_per = row_splits[part->row] * 0.01;
    const double col_per = col_splits[part->col] * 0.01;
    part->nrows = (points->nrows / points->prows) * row_per;
    part->ncols = (points->ncols / points->pcols) * col_per;
    assert(part->nrows > 3 && part->ncols > 3);
    assert(0 == part->nrows % points->bsize);
    assert(0 == part->ncols % points->bsize);
    part->brows = points->nrows / points->bsize;
    part->bcols = points->ncols / points->bsize;

    // Allocate points
    {
        part->nrows += 2;  // Padding for boundary and ghost points
        part->ncols += 2;  // Padding for boundary and ghost points

        part->stages[0] = malloc(part->nrows * sizeof(syncvar_t *));
        assert(part->stages[0]);
        part->stages[1] = malloc(part->nrows * sizeof(syncvar_t *));
        assert(part->stages[1]);
        for (int pi = 0; pi < part->nrows; pi++) {
            part->stages[0][pi] = malloc(part->ncols * sizeof(syncvar_t));
            assert(part->stages[0][pi]);
            part->stages[1][pi] = malloc(part->ncols * sizeof(syncvar_t));
            assert(part->stages[1][pi]);
            for (int pj = 0; pj < part->ncols; pj++) {
                part->stages[0][pi][pj] = INTERNAL_POINT;
                part->stages[1][pi][pj] = INTERNAL_POINT;
            }
        }
    }

    // Setup boundary
    {
        const position_t pos = part->pos;
        const size_t nrows = part->nrows;
        const size_t ncols = part->ncols;

        if (!GHOST_SOUTH(pos))
            for (int j = 1; j < ncols-1; j++) {
                part->stages[0][0][j] = boundary_value;
                part->stages[1][0][j] = boundary_value;
            }
        if (!GHOST_WEST(pos))
            for (int i = 1; i < nrows-1; i++) {
                part->stages[0][i][0] = boundary_value;
                part->stages[1][i][0] = boundary_value;
            }
        if (!GHOST_EAST(pos))
            for (int i = 1; i < nrows-1; i++) {
                part->stages[0][i][ncols-1] = boundary_value;
                part->stages[1][i][ncols-1] = boundary_value;
            }
        if (!GHOST_NORTH(pos))
            for (int j = 1; j < ncols-1; j++) {
                part->stages[0][nrows-1][j] = boundary_value;
                part->stages[1][nrows-1][j] = boundary_value;
            }
    }

    // Setup ghostzones and edges:
    // 1) empty ghost points for both stages
    // 2) empty edge points only in second stage
    {
        const position_t pos = part->pos;
        const size_t num_rows = part->nrows;
        const size_t num_cols = part->ncols;
    
        if (GHOST_SOUTH(pos)) {
            const size_t i = 0;
            for (int j = 1; j < num_cols-1; j++) {
                // Ghost points
                part->stages[0][i][j] = GHOST_POINT_EMPTY;
                part->stages[1][i][j] = GHOST_POINT_EMPTY;
                // Edge points
                part->stages[0][i+1][j] = EDGE_POINT_FULL;
                part->stages[1][i+1][j] = EDGE_POINT_EMPTY;
            }
        }
        if (GHOST_NORTH(pos)) {
            const size_t i = num_rows - 1;
            for (int j = 1; j < num_cols-1; j++) {
                // Ghost points
                part->stages[0][i][j] = GHOST_POINT_EMPTY;
                part->stages[1][i][j] = GHOST_POINT_EMPTY;
                // Edge points
                part->stages[0][i-1][j] = EDGE_POINT_FULL;
                part->stages[1][i-1][j] = EDGE_POINT_EMPTY;
            }
        }
        if (GHOST_WEST(pos)) {
            const size_t j = 0;
            for (int i = 1; i < num_rows-1; i++) {
                // Ghost points
                part->stages[0][i][j] = GHOST_POINT_EMPTY;
                part->stages[1][i][j] = GHOST_POINT_EMPTY;
                // Edge points
                part->stages[0][i][j+1] = EDGE_POINT_FULL;
                part->stages[1][i][j+1] = EDGE_POINT_EMPTY;
            }
        }
        if (GHOST_EAST(pos)) {
            const size_t j = num_cols - 1;
            for (int i = 1; i < num_rows-1; i++) {
                // Ghost points
                part->stages[0][i][j] = GHOST_POINT_EMPTY;
                part->stages[1][i][j] = GHOST_POINT_EMPTY;
                // Edge points
                part->stages[0][i][j-1] = EDGE_POINT_FULL;
                part->stages[1][i][j-1] = EDGE_POINT_EMPTY;
            }
        }
    }
}/*}}}*/

static inline void destroy_stencil(stencil_t *points)
{/*{{{*/
    const size_t num_parts = points->prows * points->pcols;
    for (int i = 0; i < num_parts; i ++) {
        partition_t *part = points->parts[i];
        const size_t nrows = part->nrows;
        for (int pi = 0; pi < nrows; pi++) {
            free(part->stages[0][pi]);
            free(part->stages[1][pi]);
        }
        free(part->stages[0]);
        free(part->stages[1]);
        free(part);
    }
    free(points->parts);
}/*}}}*/

int main(int argc, char *argv[])
{/*{{{*/
    int nrows = 12;
    int ncols = 12;
    int prows = 2;
    int pcols = 2;
    int bsize = 3;
    num_timesteps = 10;
    int print_final = 0;
    int alltime = 0;

    CHECK_VERBOSE();
    NUMARG(nrows, "NROWS");
    NUMARG(ncols, "NCOLS");
    NUMARG(prows, "PROWS");
    NUMARG(pcols, "PCOLS");
    NUMARG(bsize, "BSIZE");
    NUMARG(num_timesteps, "TIMESTEPS");
    NUMARG(print_final, "PRINT_FINAL");
    NUMARG(alltime, "ALL_TIME");

    size_t row_splits[prows];
    size_t col_splits[pcols];
    NUMARRARG(row_splits, "ROW_SPLITS", prows, 100);
    NUMARRARG(col_splits, "COL_SPLITS", pcols, 100);

    const size_t num_parts = prows * pcols;

    // Check sanity
    assert(nrows > 0 && ncols > 0 && num_timesteps > 0);
    assert(nrows % prows == 0 && ncols % pcols == 0);
    assert(nrows/prows > 1 && ncols/pcols > 1);

    // Initialize Qthreads
    assert(qthread_initialize() == 0);

    qtimer_t setup_timer = qtimer_create();
    qtimer_t exec_timer = qtimer_create();

    // Setup stencil and partitions
    stencil_t points = {nrows, ncols, prows, pcols, bsize, NULL};
    {
        qtimer_start(setup_timer);
        points.parts = malloc(num_parts * sizeof(partition_t*));
        assert(points.parts);

        ss_args_t ss_args = {&points, row_splits, col_splits};
        qt_loop(0, num_parts, setup_stencil, &ss_args);
        qtimer_stop(setup_timer);
    }

#ifdef DEBUG
    int num_sends = (prows-1)*(ncols/bsize)*2 + 
                    (pcols-1)*(nrows/bsize)*2 + 
                    prows*pcols;
    int num_corners = (prows-1)*(pcols-1)*4;
    int num_edges = num_corners*2;
    iprintf("DEBUG: creating barrier with %d participants\n",
        num_sends +
        num_corners +
        num_edges);
    debug_barrier = qt_barrier_create(
        num_sends +
        num_corners +
        num_edges,
	REGION_BARRIER);
#endif

    // Start off computation on each partition
    {
        qtimer_start(exec_timer);
        qt_loop(0, num_parts, begin, &points);
        qtimer_stop(exec_timer);
    }

#ifdef DEBUG
qt_barrier_destroy(debug_barrier);
#endif

    // Print timing info
    if (alltime) {
        fprintf(stderr, "Setup time: %f\n", qtimer_secs(setup_timer));
        fprintf(stderr, "Execution time: %f\n", qtimer_secs(exec_timer));
    } else {
        fprintf(stdout, "%f\n", qtimer_secs(exec_timer));
    }
  
    if (print_final) {
        print_stencil(&points, (num_timesteps-1) % 2);
        print_stencil(&points, num_timesteps % 2);
    }

    qtimer_destroy(setup_timer);
    qtimer_destroy(exec_timer);

    // Free allocated memory
    destroy_stencil(&points);

    return 0;
}/*}}}*/

/* vim:set expandtab */
