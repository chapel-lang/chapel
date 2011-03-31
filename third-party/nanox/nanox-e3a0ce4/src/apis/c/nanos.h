/*************************************************************************************/
/*      Copyright 2009 Barcelona Supercomputing Center                               */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             */
/*************************************************************************************/

#ifndef _NANOS_H_
#define _NANOS_H_

#include <unistd.h>
#include "nanos-int.h"

#ifdef GPU_DEV
#include "gpuprocessor.hpp"
#endif

#ifdef _MERCURIUM
// define API version
#pragma nanos interface family(master) version(5005)
#endif

// data types

// C++ types hidden as void *
typedef void * nanos_wg_t;
typedef void * nanos_team_t;
typedef void * nanos_sched_t;
typedef void * nanos_slicer_t;
typedef void * nanos_slicer_data_t;
typedef void * nanos_dd_t;
typedef void * nanos_sync_cond_t;
typedef unsigned int nanos_copy_id_t;

typedef struct {
   int nthreads;
   void *arch;
} nanos_constraint_t;

typedef enum { NANOS_OK=0,
               NANOS_UNKNOWN_ERR,          // generic error
               NANOS_UNIMPLEMENTED,        // service not implemented
} nanos_err_t;

// TODO: move smp to some dependent part
typedef struct {
   void (*outline) (void *);
} nanos_smp_args_t;

#ifdef __cplusplus

#define _Bool bool

extern "C" {
#endif

// Functions related to WD
nanos_wd_t nanos_current_wd (void);
int nanos_get_wd_id(nanos_wd_t wd);

nanos_slicer_t nanos_find_slicer ( const char * slicer );

nanos_err_t nanos_create_wd ( nanos_wd_t *wd, size_t num_devices, nanos_device_t *devices, size_t data_size, int data_align,
                              void ** data, nanos_wg_t wg, nanos_wd_props_t *props, size_t num_copies, nanos_copy_data_t **copies );

nanos_err_t nanos_set_translate_function ( nanos_wd_t wd, nanos_translate_args_t translate_args );

nanos_err_t nanos_create_sliced_wd ( nanos_wd_t *uwd, size_t num_devices, nanos_device_t *devices, size_t outline_data_size, int outline_data_align,
                               void **outline_data, nanos_wg_t uwg, nanos_slicer_t slicer, size_t slicer_data_size,
                               int slicer_data_align, void **slicer_data, nanos_wd_props_t *props, size_t num_copies, nanos_copy_data_t **copies );

nanos_err_t nanos_submit ( nanos_wd_t wd, size_t num_deps, nanos_dependence_t *deps, nanos_team_t team );

nanos_err_t nanos_create_wd_and_run ( size_t num_devices, nanos_device_t *devices, size_t data_size, int data_align, void * data,
                                      size_t num_deps, nanos_dependence_t *deps, nanos_wd_props_t *props,
                                      size_t num_copies, nanos_copy_data_t *copies, nanos_translate_args_t translate_args );

nanos_err_t nanos_create_for ( void );

nanos_err_t nanos_set_internal_wd_data ( nanos_wd_t wd, void *data );
nanos_err_t nanos_get_internal_wd_data ( nanos_wd_t wd, void **data );
nanos_err_t nanos_yield ( void );

nanos_err_t nanos_slicer_get_specific_data ( nanos_slicer_t slicer, void ** data );

// Team related functions

nanos_err_t nanos_create_team(nanos_team_t *team, nanos_sched_t sg, unsigned int *nthreads,
                              nanos_constraint_t * constraints, bool reuse, nanos_thread_t *info);

nanos_err_t nanos_create_team_mapped (nanos_team_t *team, nanos_sched_t sg, unsigned int *nthreads,                                                           unsigned int *mapping);

nanos_err_t nanos_leave_team ( );
nanos_err_t nanos_end_team ( nanos_team_t team );

nanos_err_t nanos_team_barrier ( void );

nanos_err_t nanos_single_guard ( bool *);

// sync

nanos_err_t nanos_wg_wait_completion ( nanos_wg_t wg );

nanos_err_t nanos_create_int_sync_cond ( nanos_sync_cond_t *sync_cond, volatile int *p, int condition );
nanos_err_t nanos_create_bool_sync_cond ( nanos_sync_cond_t *sync_cond, volatile bool *p, bool condition );
nanos_err_t nanos_sync_cond_wait ( nanos_sync_cond_t *sync_cond );
nanos_err_t nanos_sync_cond_signal ( nanos_sync_cond_t *sync_cond );
nanos_err_t nanos_destroy_sync_cond ( nanos_sync_cond_t *sync_cond );

nanos_err_t nanos_wait_on ( size_t num_deps, nanos_dependence_t *deps );

#define NANOS_INIT_LOCK_FREE { NANOS_LOCK_FREE }
#define NANOS_INIT_LOCK_BUSY { NANOS_LOCK_BUSY }
nanos_err_t nanos_init_lock ( nanos_lock_t **lock );
nanos_err_t nanos_set_lock (nanos_lock_t *lock);
nanos_err_t nanos_unset_lock (nanos_lock_t *lock);
nanos_err_t nanos_try_lock ( nanos_lock_t *lock, bool *result );
nanos_err_t nanos_destroy_lock ( nanos_lock_t *lock );

// Device copies
nanos_err_t nanos_get_addr ( nanos_copy_id_t copy_id, void **addr, nanos_wd_t cwd );

nanos_err_t nanos_copy_value ( void *dst, nanos_copy_id_t copy_id, nanos_wd_t cwd );

// system interface
nanos_err_t nanos_get_num_running_tasks ( int *num );

// error handling
void nanos_handle_error ( nanos_err_t err );

// factories
   // smp
void * nanos_smp_factory( void *prealloc ,void *args);
extern const size_t nanos_smp_dd_size;
#define NANOS_SMP_DESC( args ) { nanos_smp_factory, nanos_smp_dd_size, &( args ) }

   // gpu
void * nanos_gpu_factory( void *prealloc ,void *args);
extern const size_t nanos_gpu_dd_size;
#define NANOS_GPU_DESC( args ) { nanos_gpu_factory, nanos_gpu_dd_size, &( args ) }

#ifdef GPU_DEV
cudaStream_t nanos_get_kernel_execution_stream();
#else
#ifdef __CUDACC__
cudaStream_t nanos_get_kernel_execution_stream();
#endif
#endif

// instrumentation interface
nanos_err_t nanos_instrument_register_key ( nanos_event_key_t *event_key, const char *key, const char *description, bool abort_when_registered );
nanos_err_t nanos_instrument_register_value ( nanos_event_value_t *event_value, const char *key, const char *value, const char *description, bool abort_when_registered );

nanos_err_t nanos_instrument_register_value_with_val ( nanos_event_value_t val, const char *key, const char *value, const char *description, bool abort_when_registered );

nanos_err_t nanos_instrument_get_key (const char *key, nanos_event_key_t *event_key);
nanos_err_t nanos_instrument_get_value (const char *key, const char *value, nanos_event_value_t *event_value);


nanos_err_t nanos_instrument_events ( unsigned int num_events, nanos_event_t events[] );
nanos_err_t nanos_instrument_enter_state ( nanos_event_state_value_t state );
nanos_err_t nanos_instrument_leave_state ( void );
nanos_err_t nanos_instrument_enter_burst( nanos_event_key_t key, nanos_event_value_t value );
nanos_err_t nanos_instrument_leave_burst( nanos_event_key_t key );
nanos_err_t nanos_instrument_point_event ( unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values );
nanos_err_t nanos_instrument_ptp_start ( nanos_event_domain_t domain, nanos_event_id_t id,
                                         unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values );
nanos_err_t nanos_instrument_ptp_end ( nanos_event_domain_t domain, nanos_event_id_t id,
                                         unsigned int nkvs, nanos_event_key_t *keys, nanos_event_value_t *values );

nanos_err_t nanos_instrument_disable_state_events ( nanos_event_state_value_t state );
nanos_err_t nanos_instrument_enable_state_events ( void );

nanos_err_t nanos_instrument_close_user_fun_event();


// utility macros

#define NANOS_SAFE( call ) \
do {\
   nanos_err_t err = call;\
   if ( err != NANOS_OK ) nanos_handle_error( err );\
} while (0)

#ifdef __cplusplus
}
#endif

#endif
