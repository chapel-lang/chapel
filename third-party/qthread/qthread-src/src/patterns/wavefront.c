#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>                    /* for malloc */

#include <qthread/qthread.h>
#include <qthread/qdqueue.h>
#include <qthread/wavefront.h>
#include <qthread/qpool.h>

#include "qt_alloc.h"
#include "qt_asserts.h"
#include "qt_int_ceil.h"

static qpool *workunit_pool = NULL;

static void qt_wavefront_regionworker(qarray *restrict left,
                                      qarray *restrict below,
                                      char *restrict  *R,
                                      wave_comp_f      func)
{
    const size_t U_S        = left->unit_size;
    const size_t Vert_Count = left->count;
    const size_t Col_Count  = below->count - 1;

    /* the first column */
    func(qarray_elem_nomigrate(left, 0),        /* left */
         qarray_elem_nomigrate(below, 0),       /* belowleft */
         qarray_elem_nomigrate(below, 1),       /* below */
         R[0] + 0 /* output */);
    for (size_t row = 1; row < Vert_Count; row++) {
        func(qarray_elem_nomigrate(left, row) /* left */,
             qarray_elem_nomigrate(left, row - 1) /* belowleft */,
             R[0] + ((row - 1) * U_S) /* below */,
             R[0] + (row * U_S) /* output */);
    }
    /* the rest of the columns */
    for (size_t col = 1; col < Col_Count; col++) {
        func(R[col - 1] + 0 /* left */,
             qarray_elem_nomigrate(below, col) /* belowleft */,
             qarray_elem_nomigrate(below, col + 1) /* below */,
             R[col] + 0 /* output */);
        for (size_t row = 1; row < Vert_Count; row++) {
            func(R[col - 1] + (row * U_S) /* left */,
                 R[col - 1] + ((row - 1) * U_S) /* belowleft */,
                 R[col] + ((row - 1) * U_S) /* below */,
                 R[col] + (row * U_S) /* output */);
        }
    }
}

struct qt_wavefront_laths_s {
    qarray * /*restrict */ *restrict *strips;
    size_t                            num;             /* the number of strips */
    size_t                            segs;            /* qarray*'s per strip */
    size_t                            seg_len;         /* elements per qarray */
};

struct qt_wavefront_lattice_s {
    struct qt_wavefront_laths_s slats;  /* horizontal pieces */
    struct qt_wavefront_laths_s struts; /* vertical pieces */
    size_t                      unit_size;
};

struct qt_wavefront_wargs {
    qdqueue_t *const          work_queue;
    aligned_t *restrict const no_more_work;
    aligned_t *restrict const donecount;
    wave_comp_f               func;
    qt_wavefront_lattice     *L;
};

struct qt_wavefront_workunit {
    size_t                        col, row;
    struct qt_wavefront_workunit *next;
};

// aligned_t requeued=0;

static void qt_wavefront_worker(struct qt_wavefront_wargs *const arg)
{
    assert(arg);
    qt_wavefront_lattice *const L     = arg->L;
    qarray                     *local = NULL;
    char                      **R     = NULL;

    {                                  /* step 0: allocate local memory */
        const size_t horizCount = L->slats.seg_len - 1;
        const size_t vertCount  = L->struts.seg_len;

        local = qarray_create_configured(vertCount * horizCount, L->unit_size,
                                         ALL_LOCAL, 1, 1);
        assert(local);
        R = qt_calloc(horizCount, sizeof(char *));
        assert(R);
        for (size_t i = 0; i < horizCount - 1; i++) {
            R[i] = qarray_elem_nomigrate(local, i * vertCount);
            assert(R[i]);
        }
    }
    while (1) {
        struct qt_wavefront_workunit *const wu = qdqueue_dequeue(arg->work_queue);

        if (wu == NULL) {
            if (arg->no_more_work) {
                qthread_incr(arg->donecount, 1);
                break;
            }
            qthread_yield();
        } else {
            // printf("worker pulled col %i, row %i\n", (int)wu->col, (int)wu->row);
            /* step 1: locate the input and output qarrays */
            qarray *left, *below, *right, *above;
            left  = L->struts.strips[wu->col][wu->row];
            below = L->slats.strips[wu->row][wu->col];
            right = L->struts.strips[wu->col + 1][wu->row];
            above = L->slats.strips[wu->row + 1][wu->col];
            assert(below);
            assert(left);
            if (qthread_cas_ptr((void **)&(L->slats.strips[wu->row + 1][wu->col]),
                                NULL,
                                (void *)1) == NULL) {
                /* I win! time to compute! */
                assert(right == NULL);
                right = qarray_create_configured(left->count, L->unit_size, ALL_LOCAL, 1, 1);
                assert(right);
                assert(above == NULL); // when I found it, that is
                above = qarray_create_configured(below->count, L->unit_size, ALL_LOCAL, 1, 1);
                assert(above);
                R[below->count - 2] = qarray_elem_nomigrate(right, 0);  // so the computation dumps right into the qarray on the right
                /* step 2: complete the work unit */
                qt_wavefront_regionworker(left, below, R, arg->func);
                /* step 3: copy data from temporary local memory to lattice */
                memcpy(qarray_elem_nomigrate(above, 0),
                       qarray_elem_nomigrate(left, left->count - 1),
                       L->unit_size);
                for (size_t col = 0; col < (below->count - 1); col++) {
                    memcpy(qarray_elem_nomigrate(above, col + 1),
                           R[col] + ((left->count - 1) * L->unit_size),
                           L->unit_size);
                }
                assert(L->slats.strips[wu->row + 1][wu->col] == (void *)1);
                assert(L->struts.strips[wu->col + 1][wu->row] == NULL);
                L->slats.strips[wu->row + 1][wu->col]  = above;
                L->struts.strips[wu->col + 1][wu->row] = right;
                /* -- don't have to copy {right}, because data was placed in there directly */
                /* step 4: enqueue work unit for right (maybe) */
                if ((wu->col < L->slats.segs - 1) && /* there is a next column */
                    ((intptr_t)L->slats.strips[wu->row][wu->col + 1] > 1)) {
                    /* data is ready! yay! */
                    struct qt_wavefront_workunit *wu2 = qpool_alloc(workunit_pool);

                    wu2->row = wu->row;
                    wu2->col = wu->col + 1;
                    qdqueue_enqueue_there(arg->work_queue, wu2,
                                          qarray_shepof(L->slats.strips[wu2->row][wu2->col], 0));
                }
                /* step 5: enqueue work unit for next up (maybe) */
                if ((wu->row < L->struts.segs - 1) &&
                    (L->struts.strips[wu->col][wu->row + 1] != NULL)) {
                    struct qt_wavefront_workunit *wu2 = qpool_alloc(workunit_pool);

                    wu2->row = wu->row + 1;
                    wu2->col = wu->col;
                    qdqueue_enqueue_there(arg->work_queue, wu2,
                                          qarray_shepof(L->slats.strips[wu2->row][wu2->col], 0));
                }
                if ((wu->col == L->slats.segs - 1) &&
                    (wu->row == L->struts.segs - 1)) {
                    MACHINE_FENCE;
                    assert(arg->no_more_work);
                    *arg->no_more_work = 1;
                }
                // } else {
                // qthread_incr(&requeued, 1);
            }
            qpool_free(workunit_pool, wu);
        }
    }
    if (R) {
        qt_free(R);
    }
    if (local) {
        qarray_destroy(local);
    }
}

/* coming at this from a new direction. So, {vertical} contains the left-hand
 * edge, and {horizontal} contains the bottom edge. The underlying assumption
 * is that we are attempting to comput R[max][max], essentially. The input
 * function, {func}, is expected to be able to handle wave-front computation
 * over a subrange specified by the library. The last time the user's function
 * is called, the R[submax][submax] will be R[max][max], if that makes sense.
 * Thus, it's the user's responsibility to save this data somewhere useful.
 *
 * This library will do all necessary resource management for the internal
 * temporary data. It is NOT persistent, which means calculating R[max][max] is
 * relatively efficient, but means that calculating arbitrary R[x][y] values is
 * not very convenient.
 */
/* note that the bottom-left corner is item 0 of the below array */
qt_wavefront_lattice *qt_wavefront(qarray *restrict const vertical,
                                   qarray *restrict const horizontal,
                                   wave_comp_f            func)
{
    assert(vertical);
    assert(horizontal);
    assert(func);
    {
        qt_wavefront_lattice         *L;
        aligned_t                     no_more_work = 0;
        aligned_t                     donecount    = 0;
        qthread_shepherd_id_t         maxsheps     = qthread_num_shepherds();
        struct qt_wavefront_wargs     wargs        = {
            qdqueue_create(), &no_more_work, &donecount, func, NULL /*L*/
        };
        struct qt_wavefront_workunit *wu;

        if (workunit_pool == NULL) {
            workunit_pool = qpool_create(sizeof(struct qt_wavefront_workunit));
        }

        /* step 1: create the lattice */
        L = qt_calloc(1, sizeof(qt_wavefront_lattice));
        assert(L);
        wargs.L      = L;
        L->unit_size = vertical->unit_size;
        if (vertical->count > vertical->segment_size) {
            L->slats.num = QT_CEIL_RATIO(vertical->count, vertical->segment_size) + 1;
        } else {
            L->slats.num = 2;
        }
        if (horizontal->count > horizontal->segment_size) {
            L->struts.num = 2 + QT_CEIL_RATIO((horizontal->count - horizontal->segment_size),
                                              (horizontal->segment_size - 1));
        } else {
            L->struts.num = 2;
        }
        L->slats.seg_len  = horizontal->segment_size;
        L->struts.seg_len = vertical->segment_size;
        L->struts.segs    = L->slats.num - 1;
        L->slats.segs     = L->struts.num - 1;
        L->slats.strips   = qt_calloc(L->slats.num, sizeof(qarray * *));
        L->struts.strips  = qt_calloc(L->struts.num, sizeof(qarray * *));
        for (size_t i = 0; i < L->struts.num; i++) {
            L->struts.strips[i] = qt_calloc(L->struts.segs, sizeof(qarray *));
        }
        assert(L->slats.segs > 0);
        for (size_t i = 0; i < L->slats.num; i++) {
            L->slats.strips[i] = qt_calloc(L->slats.segs, sizeof(qarray *));
        }
        /* -- now that memory is initialized, populate the first qarrays
         * by copying the input data into the lattice
         * (maybe this is wasteful, but right now I don't care) */
        for (size_t i = 0; i < L->struts.segs; i++) {
            const size_t array_len = (vertical->count - (i * L->struts.seg_len) > L->struts.seg_len) ?
                                     L->struts.seg_len :
                                     (vertical->count - (i * L->struts.seg_len));
            qarray *tmp = qarray_create_configured(array_len,
                                                   L->unit_size, ALL_RAND, 1,
                                                   1);

            assert(tmp);
            assert(vertical);
            /* allocated... now copy */
            memcpy(qarray_elem_nomigrate(tmp, 0),
                   qarray_elem_nomigrate(vertical, i * (L->struts.seg_len)),
                   L->unit_size * array_len);
            assert(L->struts.strips);
            assert(L->struts.strips[0]);
            L->struts.strips[0][i] = tmp;
        }
        for (size_t i = 0; i < L->slats.segs; i++) {
            const size_t array_len = (horizontal->count - (i * L->slats.seg_len) > L->slats.seg_len) ?
                                     L->slats.seg_len :
                                     (horizontal->count - (i * L->slats.seg_len) + 1);
            qarray *tmp = qarray_create_configured(array_len,
                                                   L->unit_size, ALL_RAND, 1,
                                                   1);

            assert(tmp);
            /* allocated... now copy */
            memcpy(qarray_elem_nomigrate(tmp, 0),
                   qarray_elem_nomigrate(horizontal,
                                         i * (L->slats.seg_len - 1)),
                   L->unit_size * array_len);
            L->slats.strips[0][i] = tmp;
        }
        /* step 2: create the qdqueue for coordinating workers */
        /* -- work queue set up as part of initialization stuff, above */

        /* step 3: spawn workers */
        for (qthread_shepherd_id_t shep = 0; shep < maxsheps; shep++) {
            qthread_fork_to((qthread_f)qt_wavefront_worker, &wargs, NULL,
                            shep);
        }
        /* step 4: queue a job for the lower-left corner */
        wu       = qpool_alloc(workunit_pool);
        wu->row  = 0;
        wu->col  = 0;
        wu->next = NULL;
        qdqueue_enqueue(wargs.work_queue, wu);
        /* step 5: wait for the workers to get done */
        while (donecount < maxsheps) {
            qthread_yield();
        }
        qdqueue_destroy(wargs.work_queue);
        // printf("duplicates: %lu\n", (unsigned long)requeued);
        return L;
    }
}

void qt_wavefront_destroy_lattice(qt_wavefront_lattice *const L)
{
    const size_t slatCount     = L->slats.num;
    const size_t slatSegCount  = L->slats.segs;
    const size_t strutCount    = L->struts.num;
    const size_t strutSegCount = L->struts.segs;

    for (size_t i = 0; i < slatCount; i++) {
        for (size_t seg = 0; seg < slatSegCount; seg++) {
            qarray_destroy(L->slats.strips[i][seg]);
        }
        qt_free((void *)(L->slats.strips[i]));
    }
    qt_free((void *)(L->slats.strips));
    for (size_t i = 0; i < strutCount; i++) {
        for (size_t seg = 0; seg < strutSegCount; seg++) {
            qarray_destroy(L->struts.strips[i][seg]);
        }
        qt_free((void *)(L->struts.strips[i]));
    }
    qt_free((void *)(L->struts.strips));
    qt_free(L);
}

void qt_wavefront_print_lattice(const qt_wavefront_lattice *const L)
{
    for (ssize_t slat = L->slats.num - 1; slat >= 0; slat--) {
        assert(L->slats.strips[slat]);
        /* print this slat */
        for (size_t stripSeg = 0; stripSeg < L->slats.segs; stripSeg++) {
            const qarray *const thisSeg = L->slats.strips[slat][stripSeg];
            if (thisSeg) {
                for (size_t segCol = 0; segCol < thisSeg->count; segCol++) {
                    printf("%9.3f ",
                           *(double *)qarray_elem_nomigrate(thisSeg, segCol));
                }
            } else {
                printf("---NULL SLAT---");
            }
            printf("|");
        }
        printf("\n");
        /* now print the strut segments under the slat */
        // strut 0
        if (slat == 0) {
            continue;
        }
        const size_t         strutSeg = slat - 1;
        qarray *const *const strut0   = (qarray **)(L->struts.strips[0]);
        assert(strut0);
        const qarray *thisSeg = strut0[strutSeg];
        assert(thisSeg);
        for (ssize_t segRow = thisSeg->count - 1; segRow >= 0; segRow--) {
            for (size_t strut = 0; strut < L->struts.num; strut++) {
                qarray *const *const thisStrut = (qarray **)(L->struts.strips[strut]);
                assert(thisStrut);
                const qarray *curSeg = thisStrut[strutSeg];
                if (strut > 0) {
                    printf("|");
                }
                if (curSeg) {
                    printf("%9.3f ",
                           *(double *)qarray_elem_nomigrate(curSeg, segRow));
                } else {
                    printf("(NULL ) ");
                }
                if (strut < L->slats.num - 1) {
                    const size_t spacecount = L->slats.strips[0][strut]->count - 1;
                    for (size_t space = 0; space < spacecount; space++) {
                        printf("          ");
                    }
                }
            }
            printf("\n");
        }
    }
}

void qt_basic_wavefront(int *restrict const *const R,
                        size_t                     cols,
                        size_t                     rows,
                        wave_comp_f                func)
{
    /* assuming R is properly initialized. */
    for (size_t col = 1; col < cols; col++) {
        for (size_t row = 1; row < rows; row++) {
            func(&(R[col - 1][row]), &(R[col - 1][row - 1]),
                 &(R[col][row - 1]), &(R[col][row]));
        }
    }
}

/* vim:set expandtab: */
