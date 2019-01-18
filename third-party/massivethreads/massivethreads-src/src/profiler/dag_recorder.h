/*
 * dag recorder 2.0
 */

#pragma once

/* this file defines an API that is (or must be)
   visible from clients. */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* if set, define all instrumentation APIs in
   dag_recorder_inl.h included from client code,
   so as to inline all performance-critical 
   instrumentations. define it to zero when 
   you do not want to do it; in that case,
   client does not include dag_recorder_inl.h,
   dag_recorder_no_inl.c defines all instrumentation
   code as externally-visible functions called
   by clients */
#if !defined(DAG_RECORDER_INLINE_INSTRUMENTATION)
#define DAG_RECORDER_INLINE_INSTRUMENTATION 1
#endif

/* static_if_inline macro :
   we make all instrumentation functions static 
   when directly included by clients. otherwise
   they are external. static_if_inline helps
   us switch between them easily */
#if DAG_RECORDER_INLINE_INSTRUMENTATION
#define static_if_inline static
#else
#define static_if_inline
#endif

  /* experimental backtrace sampling */

  /* data type for clocks (64 bits) */
  typedef unsigned long long dr_clock_t;
  /* dag node */
  typedef struct dr_dag_node dr_dag_node;
  /* user-defined hooks called on actions */
  typedef int (*dr_hook)(dr_dag_node * node);

  typedef struct {
    dr_hook start_task;
    dr_hook begin_section;
    dr_hook enter_create_task;
    dr_hook return_from_create_task;
    dr_hook enter_wait_tasks;
    dr_hook return_from_wait_tasks;
    dr_hook enter_other;
    dr_hook return_from_other;
    dr_hook end_task;
  } dr_hooks;

  /* runtime-settable options */
  typedef struct dr_options {
    const char * dag_file_prefix; /* filename prefix of all files */
    char dag_file_yes;	          /* if set, save the dag */
    char stat_file_yes;	          /* if set, save the stat summary */
    char gpl_file_yes;	          /* if set, save parallelism (gnuplot) */
    char dot_file_yes;	          /* if set, save dot file */
    char sqlite_file_yes;	  /* if set, save sqlite3 db */
    char text_file_yes;	          /* if set, save the text version of dag */
    char nodes_file_yes;	  /* if set, save nodes separately */
    char edges_file_yes;	  /* if set, save edges separately */
    char strings_file_yes;	  /* if set, save strings separately */
    const char * text_file_sep;	  /* separator for text file */

    /* span-based contraction */
    dr_clock_t uncollapse_min;	/* minimum length that can be uncollapsed */
    dr_clock_t collapse_max;	/* maximum length that can be collpased */

    /* contraction to target a given number of nodes */
    long node_count_target;	/* desired number of nodes */
    long prune_threshold;	/* prune nodes larger than node_count_target * prune_threshold */

    /* max count based contraction */
    long collapse_max_count;	/* if logical node < this value, collapse */

    long alloc_unit_mb;	        /* node allocation unit in bytes */
    long pre_alloc_per_worker;  /* pre-allocated units per worker */
    long pre_alloc;	        /* pre-allocated units */
    dr_hooks hooks;		/* hooks */
    int gpl_sz;			/* size of gpl file */
    char worker_specific_state_array; 
                                /* set 1 to use array of worker-specific states */
    char on;
    char dbg_level;		/* level of debugging features */
    char verbose_level;		/* level of verbosity */
    char chk_level;		/* level of checks during run */
    char record_cpu;		/* 1 if we record cpu */
    char shrink;		/* 1 if we shrink dag (effective only in dag2any) */
  } dr_options;

  /* default values for runtime options. 
     written here for documentation purpose.
     there are two ways to overwrite them.
     (1) explicitly pass options to dr_start().
     e.g.,
     dr_options opts; 
     dr_options_default(&opts);
     opts.dbg_level = 2; 
     dr_start(&opts);
     (2) set corresponding environment variables when
     you run the program. e.g.,
     DAG_RECORDER_DBG_LEVEL=2 ./a.out
  */
  static dr_options dr_options_default_values 
  __attribute__ ((unused)) = {
    (const char *)"00dr",       /* dag_file_prefix */
    1,				/* dag_file_yes */
    1,				/* stat_file */
    1,				/* gpl_file */
    0,				/* dot_file */
    0,				/* sqlite_file */
    0,				/* text_file */
    0,				/* nodes_file */	
    0,				/* edges_file */	
    0,			 /* strings_file */	
    (const char *)"|",	       /* text_file_sep */
    0,			       /* uncollapse_min */
    (1L << 60),	/* collapse_max used to be (1L << 60), */
    0,		/* node_count_target */
    100000,	/* prune_threshold */

    0,		/* collapse_max_count */

    1,		/* alloc unit in MB */
    0,	       /* the number of pre-allocations */
    0,	       /* the number of pre-allocations per worker */
    {
      0,	     /* start_task */
      0,	     /* begin_section */
      0,	     /* enter_create_task */
      0,	     /* return_from_create_task */
      0,	     /* enter_wait_tasks */
      0,	     /* return_from_wait_tasks */
      0,	     /* dr_hook enter_other */
      0,	     /* return_from_other */
      0,	     /* end_task */
    },
    4000,      /* gpl_sz */
    0,	       /* worker_specific_state_array */

    1,	       /* on */
    0,	       /* dbg_level */
    0,	       /* verbose_level */
    0,	       /* chk_level */
    0,	       /* record_cpu */
    0,	       /* shrink */
  };
  
  /* all instrumentation functions are defined as
     macros below, as calling corresponding functions
     taking extra parameters (caller's location, and
     worker id).  a typical pattern is
     
     void some_instrumentation_func__(..., file, line, worker);
     #define some_instrumentation_func_(..., file, line) \
          some_instrumentation_func_(..., file, line, dr_get_worker())
     #define some_instrumentation_func(...) \
          some_instrumentation_func_(..., __FILE__, __LINE__)
  */

#if DAG_RECORDER == 2
  /* set default options in opts */
#define dr_options_default(opts) \
  dr_options_default_(opts)

  /* call this at the beginning of a task */
#define dr_start_task(parent)			\
  dr_start_task_(parent, __FILE__, __LINE__)

  /* call this at the beginning of a cilk procedure (task) */
#define dr_start_cilk_proc(parent) \
  dr_start_cilk_proc_(__FILE__, __LINE__)

  /* call this at the beginning of a section. a section begins
     when you create a task right after taskwait */
#define dr_begin_section() \
  dr_begin_section__(dr_get_worker())

  /* call this before you create a task */
#define dr_enter_create_task(create) \
  dr_enter_create_task_(create, __FILE__, __LINE__)

  /* call this before you spawn a cilk procedure task */
#define dr_enter_create_cilk_proc_task() \
  dr_enter_create_cilk_proc_task__(__FILE__, __LINE__, dr_get_worker())

  /* call this after you return from a create_task */
#define dr_return_from_create_task(task) \
  dr_return_from_create_task_(task, __FILE__, __LINE__)

  /* call this before you wait for child tasks */
#define dr_enter_wait_tasks() \
  dr_enter_wait_tasks_(__FILE__, __LINE__)

  /* call this after you return from waiting child tasks */
#define dr_return_from_wait_tasks(task) \
  dr_return_from_wait_tasks_(task, __FILE__, __LINE__)

  /* call this before you call any function that may 
     migrate the calling task */
#define dr_enter_other() \
  dr_enter_other_(__FILE__, __LINE__)

  /* call this after you return from any function that may
     migrate the calling task */
#define dr_return_from_other(task) \
  dr_return_from_other_(task, __FILE__, __LINE__)

  /* call this immediately before you end a task */
#define dr_end_task() \
  dr_end_task_(__FILE__, __LINE__)

  /* start recording dag (must be called by a root task) */
#define dr_start(opts) \
  dr_start__(opts, __FILE__, __LINE__, dr_get_worker(), dr_get_max_workers())

  /* stop recording dag */
#define dr_stop() \
  dr_stop__(__FILE__, __LINE__, dr_get_worker())

  /* clean up everything */
#define dr_cleanup() \
  dr_cleanup__(__FILE__, __LINE__, dr_get_worker(), dr_get_max_workers())

  /* dump trace results (between the last dr_start()/dr_stop() */
#define dr_dump() dr_dump_()

  /* read the dag file and convert it into various other formats */
#define dr_read_and_analyze_dag(filename) \
  dr_read_and_analyze_dag_(filename)

  /* declarations of functions (used in the above macro definitions) */
  void dr_options_default_(dr_options * opts);

  static_if_inline void
  dr_start_task__(dr_dag_node * parent, 
		  const char * file, int line, int worker);

  static_if_inline int
  dr_start_cilk_proc__(const char * file, int line, int worker);

  static_if_inline void
  dr_begin_section__(int worker);

  static_if_inline dr_dag_node * 
  dr_enter_create_task__(dr_dag_node ** create, 
			 const char * file, int line, int worker);

  static_if_inline dr_dag_node * 
  dr_enter_create_cilk_proc_task__(const char * file, int line, int worker);

  static_if_inline void 
  dr_return_from_create_task__(dr_dag_node * task, 
			       const char * file, int line, int worker);

  static_if_inline dr_dag_node * 
  dr_enter_wait_tasks__(const char * file, int line, int worker);

  static_if_inline void 
  dr_return_from_wait_tasks__(dr_dag_node * task, 
			      const char * file, int line, int worker);

  static_if_inline dr_dag_node * 
  dr_enter_other__(const char * file, int line, int worker);

  static_if_inline void 
  dr_return_from_other__(dr_dag_node * t, 
			 const char * file, int line, int worker);

  static_if_inline void 
  dr_end_task__(const char * file, int line, int worker);

  int dr_register_thread_start_hook(int (*f)(int worker));

  void dr_start__(dr_options * opts, const char * file, int line, 
		  int worker, int num_workers);

  void dr_stop__(const char * file, int line, int worker);

  void dr_cleanup__(const char * file, int line, 
		    int worker, int num_workers);

#define dr_start_task_(parent, file, line)			\
  dr_start_task__(parent, file, line, dr_get_worker())

#define dr_start_cilk_proc_(file, line)			\
  dr_start_cilk_proc__(file, line, dr_get_worker())

#define dr_enter_create_task_(create, file, line)		\
  dr_enter_create_task__(create, file, line, dr_get_worker())

#define dr_enter_create_cilk_proc_task_(file, line)		\
  dr_enter_create_cilk_proc_task__(file, line, dr_get_worker())

#define dr_return_from_create_task_(task, file, line)	\
  dr_return_from_create_task__(task, file, line, dr_get_worker())

#define dr_enter_wait_tasks_(file, line)			\
  dr_enter_wait_tasks__(file, line, dr_get_worker())

#define dr_return_from_wait_tasks_(task, file, line)		\
  dr_return_from_wait_tasks__(task, file, line, dr_get_worker())

#define dr_enter_other_(file, line)			\
  dr_enter_other__(file, line, dr_get_worker())

#define dr_return_from_other_(task, file, line)		\
  dr_return_from_other__(task, file, line, dr_get_worker())

#define dr_end_task_(file, line)			\
  dr_end_task__(file, line, dr_get_worker())

  /* dump the last dag */
  void dr_dump_();

  /* read dag from filename and produce other files
     (state, gnuplot, ...) */
  int dr_read_and_analyze_dag_(const char * filename);

#else

#define dr_options_default(opts)          (void)0
#define dr_start_task(parent)             do { } while (0)
#define dr_start_cilk_proc(parent)        0
#define dr_begin_section()                do { } while (0)
#define dr_enter_create_task(create)      ((dr_dag_node*)0)
#define dr_enter_create_cilk_proc_task()  ((dr_dag_node*)0)
#define dr_return_from_create_task(task)  do { } while (0)
#define dr_enter_wait_tasks()             ((dr_dag_node*)0)
#define dr_return_from_wait_tasks(task)   do { } while (0)
#define dr_enter_other()                  ((dr_dag_node*)0)
#define dr_return_from_other(task)        do { } while (0)
#define dr_end_task()                     do { } while (0)
#define dr_start(opts)                    do { } while (0)
#define dr_stop()                         do { } while (0)
#define dr_cleanup()                      do { } while (0)
#define dr_dump()                         do { } while (0)
#define dr_read_and_analyze_dag(filename) 0

#endif

#ifdef __cplusplus
}
#endif

/* all the above static functions are defined in 
   dag_recorder_inl.h.
   all the above external functions are defined in 
   dag_recorder.c
 */
#if DAG_RECORDER>=2
#if DAG_RECORDER_INLINE_INSTRUMENTATION
#include <dag_recorder_inl.h>
#endif
#endif



