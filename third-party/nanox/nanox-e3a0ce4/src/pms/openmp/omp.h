/*************************************************************************************/
/*      Copyright 2010 Barcelona Supercomputing Center                               */
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

#ifndef _OMP_H_DEF
#define _OMP_H_DEF

/* OpenMP API interface */

/*
* define the lock data types
*/
typedef void * omp_lock_t;
typedef void * omp_nest_lock_t;

/*
* define the schedule kinds
*/
typedef enum omp_sched_t {
   omp_sched_static = 1,
   omp_sched_dynamic = 2,
   omp_sched_guided = 3,
   omp_sched_auto = 4
} omp_sched_t;

/*
* exported OpenMP functions
*/
#ifdef __cplusplus
extern "C"
{
#endif

extern void omp_set_num_threads(int num_threads);
extern int  omp_get_num_threads(void);
extern int  omp_get_max_threads(void);
extern int  omp_get_thread_num(void);
extern int  omp_get_num_procs(void);
extern int  omp_in_parallel(void);
extern void omp_set_dynamic(int dynamic_threads);
extern int  omp_get_dynamic(void);
extern void omp_set_nested(int nested);
extern int  omp_get_nested(void);
extern int  omp_get_thread_limit(void);
extern void omp_set_max_active_levels(int max_active_levels);
extern int  omp_get_max_active_levels(void);
extern void omp_set_schedule(omp_sched_t kind, int modifier);
extern void omp_get_schedule(omp_sched_t *kind, int *modifier);

extern int  omp_get_level(void);
extern int  omp_get_ancestor_thread_num(int level);
extern int  omp_get_team_size(int level);
extern int  omp_get_active_level(void);

extern void omp_init_lock(omp_lock_t *lock);
extern void omp_destroy_lock(omp_lock_t *lock);
extern void omp_set_lock(omp_lock_t *lock);
extern void omp_unset_lock(omp_lock_t *lock);
extern int  omp_test_lock(omp_lock_t *lock);

extern void omp_init_nest_lock(omp_nest_lock_t *lock);
extern void omp_destroy_nest_lock(omp_nest_lock_t *lock);
extern void omp_set_nest_lock(omp_nest_lock_t *lock);
extern void omp_unset_nest_lock(omp_nest_lock_t *lock);
extern int  omp_test_nest_lock(omp_nest_lock_t *lock);

extern double omp_get_wtime(void);
extern double omp_get_wtick(void);

extern int omp_in_final(void);


#ifdef __cplusplus
}

#endif

#endif

