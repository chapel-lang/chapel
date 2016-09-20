#include <stdlib.h>

#include "qthread/qthread.h"
#include "qt_visibility.h"
#include "qt_shepherd_innards.h"
#include "qt_envariables.h"
#include "qt_affinity.h"
#include "qt_output_macros.h"

#ifndef QTHREAD_SHEPHERD_TYPEDEF
# define QTHREAD_SHEPHERD_TYPEDEF
typedef struct qthread_shepherd_s qthread_shepherd_t;
#endif

void INTERNAL qt_topology_init(qthread_shepherd_id_t * nshepherds,
                               qthread_worker_id_t   * nworkerspershep,
                               size_t                * hw_par) {
    qthread_shepherd_id_t num_sheps;
    qthread_worker_id_t   num_wps;
    size_t                num_workers;
    uint_fast8_t          print_info      = 0;

    print_info = qt_internal_get_env_num("INFO", 0, 1);

    /* Collect common environment variables */
    num_sheps   = qt_internal_get_env_num("NUM_SHEPHERDS", 0, 0);
    num_wps     = qt_internal_get_env_num("NUM_WORKERS_PER_SHEPHERD", 0, 0);
    num_workers = qt_internal_get_env_num("HWPAR", num_sheps * num_wps,
                                          num_sheps * num_wps);

    /* Process common environment variables */
    if (THREADQUEUE_POLICY_TRUE == qt_threadqueue_policy(SINGLE_WORKER)) {
        if (0 == num_wps) {
            num_wps = 1;
        } else if (1 < num_wps) {
            /* Disregard WPS hint for single-threaded schedulers. */
            print_warning("Disregarding request for %d workers per shepherd specified, scheduler only supports 1.\n", num_wps);
            num_wps = 1;
        }
    } else {
        if (0 < num_wps && 0 == num_sheps) {
            /* Late-bound shep count will have precedence over user-specified
             * WPS */
            print_warning("Number of shepherds not specified - number of workers may be ignored\n");
        }
    }

    qt_affinity_init(&num_sheps, &num_wps, &num_workers);

    /* Adjust logical topology */
    if (num_workers != 0) {
        if ((num_workers < num_sheps) || (num_workers == 1)) {
            num_wps = 1;
            num_sheps      = num_workers;
        } else if (num_workers > (num_sheps * num_wps)) {
            num_wps = (num_workers / num_sheps);
            if ((num_workers % num_sheps) != 0) {
                num_wps++;
            }
        } else {
            num_wps = num_workers / num_sheps;
            if (num_workers % num_sheps > 0) {
                num_wps++;
            }
        }
    } else {
        num_workers = num_sheps * num_wps;
    }

    if (print_info) {
        print_status("Using %i Shepherds\n", (int)num_sheps);
        print_status("Using %i Workers per Shepherd\n", (int)num_wps);
    }

    if (THREADQUEUE_POLICY_TRUE == qt_threadqueue_policy(SINGLE_WORKER)
        && 1 != num_wps) {
        print_error("attempted to use %i workers per shepherd with scheduler that only supports 1 worker per shepherd.\n", (int)num_wps);
        exit(EXIT_FAILURE);
    }

    *nshepherds      = num_sheps;
    *nworkerspershep = num_wps;
    *hw_par          = num_workers;
}
