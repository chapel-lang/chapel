/** \file performance.h
 * 
 * A performance measuring API for QThreads. This file contains the
 * declarations for a set of functions that can be used to implement
 * fine-grained timing measurement for the QThreads library. You can
 * measure the internal performance of QThreads itself, or you can
 * implement custom timing measurements for your own code. You can
 * also piggyback your measurements onto selected state transitions
 * within the library, so that you can measure the overhead of
 * QThreads with respect to your code in various circumstances.
 *
 * This library uses a state machine abstraction to provide a simple
 * way to track events in the code as it executes. You can define a
 * state machine using a "state group," which is the set of states
 * that the machine can enter. You can define multiple state
 * groups. Timing data is recorded by tracking the enter and leave
 * time for each state (you supply these events by calling
 * qtperf_enter_state). For each transition, the time delta is
 * calculated and added to the total for the state that was current
 * just before the transition. You can globally pause and resume data
 * collection as often as you like.
 *
 * At any point in the execution of the program, you can extract the
 * timing data for the states to inspect your data. The API also
 * includes functions for printing the data out in either a labeled
 * list format or a CSV tabular format suitable for importing into
 * other analytical software. 
 *
 * Future work: 
 *  - Implement aggregated state groups (groups that aggregate data 
 *    from many contributing threads)
 *  - Add additional qthreads internal state tracking (currently only
 *    thread state and worker state transitions are tracked).
 *
 * \author Erik Lee (ejlee@sandia.gov)
 * \version 0.1
 */

#ifdef __cplusplus 
extern "C" {
#endif

#ifndef QT_PERFORMANCE_H
#define QT_PERFORMANCE_H
#include<stddef.h>
#include<stdint.h>

#ifndef PERFDBG
#  define PERFDBG 0
#endif

#ifndef __cplusplus
typedef unsigned char bool;
#endif
typedef size_t qtperfid_t;
typedef unsigned long long qtperfcounter_t;
typedef unsigned long qttimestamp_t;
static const qtperfid_t QTPERF_INVALID_STATE = (qtperfid_t)(-1);


struct qtperf_perf_list_s;

/**  qtstategroup_t represents a group of states that can make
 *   transistions between each other.
 */
typedef struct qtstategroup_s {
  /// Number of states in this group
  size_t num_states;
  /// Human-readable name for this state group, used in output to make
  /// it clear what is being reported.
  char* name;
  /// Array of human-readable names for the states. This is
  /// optional. If not provided, the library will just report
  /// numerical names for the states.
  char** state_names;
  /// The number of instances of this state group (length of the
  /// counters list)
  size_t num_counters;
  /// Address of the tail end of the list, so that new counters can be
  /// inserted at the end.
  struct qtperf_perf_list_s** next_counter;
  /// A list of all performance counters that use this group for their
  /// state definitions.
  struct qtperf_perf_list_s* counters;
} qtstategroup_t;

/** qtperf_group_list_t is the linked list that keeps track of all of
 * the state groups that have been defined. This is used internally
 * for memory management in the library, user code should not need to
 * touch it.
 */
typedef struct qtperf_group_list_s {
  qtstategroup_t group;
  struct qtperf_group_list_s* next;
} qtperf_group_list_t;

/** qtperf_piggyback_list_t is the linked list of all piggyback
 *  definitions for a given performance data counter. Piggyback
 *  definitions allow you to have the library automatically make extra
 *  state transitions in other performance counters when the
 *  piggybacked counter makes a transition (for example, you can use
 *  this to be notified when the internal QThreads system reschedules
 *  a thread or assigns a worker to a new task). This list is internal
 *  to the QThreads library and should not be touched by user code.
 */
typedef struct qtperf_piggyback_list_s {
  struct qtperfdata_s* target_data;
  qtperfid_t target_state;
  struct qtperf_piggyback_list_s* next;
} qtperf_piggyback_list_t;

/** qtperfdata_counter_t records the actual timing information. This
    is split off from qtperfdata_t in order to support aggregates. In
    the non-aggregate case, there will be exactly one of these for
    each qtperfdata_t.
 */
typedef struct qtperfctr_s {
  // 1 unless it's an aggregate target.
  size_t num_contributors; 
  /// A reference to the qtstategroup_t that defines the states for
  /// this counter.
  qtstategroup_t* state_group;
  /// Number of states in this counter's state group. This is here to
  /// speed up state entry by avoiding a dereference of the state
  /// group pointer.
  size_t num_states;
  /// represent the total time spent in each state.
  qtperfcounter_t* data;
  /// A spin lock gate to ensure exclusive access for updates
  volatile uint32_t busy;// 1 if somebody is using this structure, else 0
} qtperfctr_t;

/** qtperfdata_t holds the current state data for each thread. Every
    thread has to be able to track it's own current state, along with
    the time that state was entered and the list of piggybacks for
    that thread. If the thread is storing its results in an aggregated
    counter, the counter pointer will be shared between all of the
    sharing threads.
 */
typedef struct qtperfdata_s{
  qtperfid_t current_state;
  qttimestamp_t time_entered;
  // This is set to true if this struct is the owner of the
  // qtperfctr_t pointer it holds (for safe deallocation)
  bool ctr_owner;
  // only used if num_contributors > 1
  volatile uint32_t busy; 
  /// the actual timing data. May be shared if it's aggregated
  qtperfctr_t* counters;
  /// An array of lists of piggyback relationships - when a
  /// piggybacked state is entered, all of its piggybackers will be
  /// entered as well
  qtperf_piggyback_list_t** piggybacks;
} qtperfdata_t;

/** qtperf_perf_list_t is a linked list of performance trackers. This
 * is used internally by the library and should not be accessed by
 * other code.
 */
typedef struct qtperf_perf_list_s {
  qtperfdata_t performance_data;
  struct qtperf_perf_list_s* next;
} qtperf_perf_list_t;


/** qtperf_iterator_t is a structure that supports iteration through
 *  the performance data structures. The iterator will start at the
 *  beginning of the first state group, and work through all counters
 *  grouped by their common state group. This allows processing to be
 *  clustered in tables of same-type counters. User code should treat
 *  this structure as opaque so that interface changes don't break
 *  things.
 */
typedef struct qtperf_iterator_s {
  /// Proceed by groups, all counters will be traversed for a group
  /// before the next group is entered.
  qtperf_group_list_t* current_group;
  /// Counters within the current group. This pointer stores the
  /// current location within that list.
  qtperf_perf_list_t* current;
} qtperf_iterator_t;


//--------------- WORKER INSTRUMENTATION ----------------------------
/** worker_state_t defines the set of states that a qthreads worker
 *  can transition between.
*/
typedef enum {
  /// Worker thread being created
  WKR_INIT,
  /// Worker thread is executing code for a qthread task
  WKR_QTHREAD_ACTIVE,
  /// Worker thread is executing shepherd overhead 
  WKR_SHEPHERD,
  /// Worker thread is idle waiting for something to do.
  WKR_IDLE,
  /// Worker thread is blocked (not currently used)
  WKR_BLOCKED,
  /// Total number of states for the worker_state_t state group
  WKR_NUM_STATES
} worker_state_t;

/** qtperf_should_instrument_workers is a flag to tell the library
 * whether or not to add instrumentation to the worker threads as they
 * are created 
*/
extern bool qtperf_should_instrument_workers;

/** qtperf_workers_group is the state group that contains the state
 *  transition information for instrumenting workers.
 *  \see [qtperf_set_instrument_workers]
 */
extern qtstategroup_t* qtperf_workers_group;

//--------------- QTHREAD INSTRUMENTATION ---------------------------- 
/** qtperf_should_instrument_qthreads is a flag to tell the library to
 * add instrumentation data to qthreads as they are created.
 * @see qtperf_set_instrument_qthreads
 */
extern bool qtperf_should_instrument_qthreads;
/** qtperf_qthreads_group is the state group for tracking internal
 *  state changes for qthreads 
 */
extern qtstategroup_t* qtperf_qthreads_group;


//---------------- PERFORMANCE API -----------------------------------
/** @brief Create a new state group with given names and states.
 *
 * This function creates a new state group. 
 * @param num_states The number of states in this state group. Should be the largest state id plus 1
 * @param group_name The human-readable name for this group. Required.
 * @param state_names An array of human-readable strings for state names. Optional (NULL to skip)
 */
qtstategroup_t* qtperf_create_state_group(size_t num_states, const char* group_name, const char** state_names);

/** @brief Create performance data to track state transitions in a group.
 *
 * This function creates a new performance data structure and
 * associates it with the given group
 *
 * @param state_group A pointer to the state group that this counter should implement.
 * @see qtperf_create_state_group
 */ 
qtperfdata_t* qtperf_create_perfdata(qtstategroup_t* state_group);

/** @brief Create performance data to track state transitions in an
 * aggregated group.
 *
 * This function creates a new performance data structure and
 * associates it with the given group. If aggregate is non-NULL, it is
 * assumed to be a pointer to a valid qtperfctr_t struct that is to be
 * used as the logging target for this perfdata struct. If it is NULL,
 * a new perfctr_t will be allocated and assigned to this struct. If
 * you want to then add more threads to the aggregation group, simply
 * specify the counters member of the qtperfdata_t* returned by this
 * function to the aggregate argument of future calls of this
 * function.
 *
 * @param state_group A pointer to the state group that this counter should implement.
 * @param aggregate A pointer to a qtperfctr_t struct that is shared between multiple loggers
 * @see qtperf_create_state_group
 */ 
qtperfdata_t* qtperf_create_aggregated_perfdata(qtstategroup_t* state_group, qtperfctr_t* aggregate);

/** @brief Return the current time
 * 
 * This function returns the current time stamp. Time stamps are
 * calculated using clock_gettime(CLOCK_MONOTONIC_RAW), and are
 * limited to microsecond precision currently.
 */ 
qttimestamp_t qtperf_now(void);

/** @brief Turn data collection on.
 *
 * This function tells the library to record data about state
 * transitions. The library defaults to being off, so this must be
 * called before the first data will be collected. This function and
 * its dual qtperf_stop() can be called at any time and any number of
 * times, so you can use them to limit data collection to only parts
 * of execution that interest you.
 *
 * @see qtperf_stop
 */
void qtperf_start(void);

/** @brief Turn data collection off.
 *
 * This function tells the library to stop collecting data. State
 * transitions that occur while the library is stopped will track the
 * state changes but will not affect the total time elapsed for any
 * state.
 *
 * @see qtperf_start
 */
void qtperf_stop(void);

/** @brief Deallocate all data used by the performance library
 *
 * This function frees all data from the performance counting
 * library. Do not call this function until you are completely
 * finished with the data, as any further access will be invalid and
 * likely will cause a crash.
 a*/
void qtperf_free_data(void);

/** @brief Return the name for the given state id
 *
 * This function is used to look up the name associated with a state
 * id for a particular state group. If the state group doesn't have
 * names defined, this will return NULL so be careful.
 * 
 * @param group the group that defines the state id in question
 * @param state_id the id number of the state
 * @see qtperf_create_state_group
 * @see qtperf_print_results
 * @see qtperf_print_delimited
 */
const char* qtperf_state_name(qtstategroup_t* group, qtperfid_t state_id);

/** @brief Signal a state transition
 *
 * This is the workhorse of the performance data collection
 * library. Performance data is associated with states for each
 * collection unit (qtperdata_t). Each unit can be in exactly one
 * state at any given time. At each transition, the library will
 * record how long the previous state was active before switching to
 * the new state. Data for each state is retained as a running total,
 * so you have only aggregate visibility (you can't see how long it
 * was in a state for each time it was in it, only how long it was
 * there overall for the collection period). To collect meaningful
 * data, first determine what intervals you want to know about, then
 * define state ids for them using qtperf_create_state_group. When you
 * have a task that will make those transitions, define a qtperfdata_t
 * struct for it using qtperf_create_perfdata. Then, each time your
 * thread switches between states, simply call qtperf_enter_state with
 * the new state id and the library will track the total time spent in
 * each state. 
 *
 * Tracking can be temporarily disabled and enabled globally using
 * qtperf_stop and qtperf_start.
 *
 * @param data The performance data struct returned by qtperf_create_perfdata
 * @param state_id The id number of the state being entered. Time will be recorded 
 *    for the old state at the instant the new state is entered
 * @see qtperf_create_state_group
 * @see qtperf_create_perfdata
 * @see qtperf_start
 * @see qtperf_stop
 */
void qtperf_enter_state(qtperfdata_t* data, qtperfid_t state_id);


/** qtperf_enter_state_from does the same thing as qtperf_enter_state,
    except that you can explicitly set the state you're coming
    from. This is used by the aggregate collection system so that
    transitions can be recorded for state groups that share perfdata.
 */
/* void qtperf_enter_state_from(qtperfdata_t* data, qtperfid_t from_state_id, qtperfid_t to_state_id);*/

/** @brief Get an iterator for inspecting performance data
 *
 * The performance data from the library is accessible through
 * iterators that are provided and manipulated with API
 * functions. This allows you to access all collected performance data
 * without having to know the internals of how that data is stored and
 * tracked. You can also access the lists directly (their data
 * pointers are visible to user code), but be aware that the structure
 * of these lists may change in the future!
 *
 * You can enter the special state QTPERF_INVALID_STATE if you want to
 * turn off time tracking for this thread. This is useful if you want
 * to measure parallelism - you can enter QTPERF_INVALID_STATE at the
 * end of your computation and the thread will then have only the time
 * that it was actually executing recorded. You can of course do the
 * same thing by just entering a "finished" state, but if you use
 * QTPERF_INVALID_STATE then the time totaling functions will report
 * your data without further intervention on your part.
 *
 * Iterators progress through the performance data by group then by
 * instance. For example, if you have defined two state groups and
 * have a number of instances for each group, this iterator will go
 * through all of the instances of the first group, followed by all of
 * the instances of the second group. This order allows for clustered
 * processing, which makes it easy to do things like output organized
 * reports on findings. If you need to use a different orderint for
 * your traversal, you can access the lists directly (avoid this if
 * possible though).
 * 
 * In order to use the iterators without requiring any allcations,
 * this function expects you to create a qtperf_iterator_t *on the
 * stack* in the function that calls this function, and then also
 * create a pointer to that struct *on the stack*. You then pass the
 * *address* of that pointer to this function. Each time the iterator
 * is updated, the pointer that you have saved will be updated. When
 * that pointer is NULL, you have reached the end of the data.
 * 
 * An example of how to use the iterator API:
 * @code
   void process_my_data(){
     qtperf_iterator_t iterator;
     qtperf_iterator_t* iter;
     qtperf_iter_begin(&iter);
     qtperfdata_t* data = qtperf_iter_next(&iter);
     while(data != NULL){
         // do something with the data
         data = qtperf_iter_next(&iter);
     } 
   }
 * @endcode
 *
 * @param iter The address of a pointer to a struct that you control
 * @see qtperf_iter_next
 * @see qtperf_iter_end
 * @see qtperf_iter_deref
 */
void qtperf_iter_begin(qtperf_iterator_t** iter);

/** @brief Increment the iterator and retrieve the current data pointer
 *
 * This function increments the iterator. It also returns the data
 * pointer associated with the current item *before*
 * incrementing. This structure makes it easier to use the iterator in
 * a loop because you can just test the return value of the
 * qtperf_iter_next instead of checking to see if the iterator is
 * qtperf_iter_end. If this function returns NULL, the end has been
 * reached.
 *
 * @param iter The address of a pointer to a qtperf_iterator_t that you control
 * @see qtperf_iter_begin
 * @see qtperf_iter_end
 * @see qtperf_iter_deref
 */
qtperfdata_t* qtperf_iter_next(qtperf_iterator_t** iter);

/** @brief Return the data pointer referenced by the iterator
 *
 * This function retrieves the current data pointer, or returns NULL
 * if you're already at the end. See the documentation for
 * qtperf_iter_begin for an example of typical iterator usage.
 * 
 * @param iter The iterator pointer (not the address of it this time)
 * @see qtperf_iter_begin
 * @see qtperf_iter_next
 * @see qtperf_iter_end
 */
qtperfdata_t* qtperf_iter_deref(qtperf_iterator_t* iter);

/** @brief Returns a value that can be used to test whether the iterator is finished.
 *
 * This function is an alternative method of detecting when the
 * iterator is done. You can check to see if your iterator is equal to
 * qtperf_iter_end(), and if it is, you stop. Currently, this function
 * just returns NULL, but at some point it's possible that may change. 
 *
 * @see qtperf_iter_next
 * @see qtperf_iter_begin
 * @see qtperf_iter_deref
 */
qtperf_iterator_t* qtperf_iter_end(void);

/** @brief Enable or disable worker data collection
 *
 * The library can track the performance characteristics of its
 * internal workers. If you want that data to be collected, call this
 * function with 1 as the argument *before* calling
 * qthread_initialize. This should not be changed after
 * qthread_initialized has been called.
 * 
 * @param yesno Use 1 if you want to collect data, zero otherwise
 */
void qtperf_set_instrument_workers(bool yesno);

/** @brief Enable or disable qthread data collection
 *
 * This function controls whether or not the internal states of
 * qthread tasks are tracked. Set the parameter to 1 *before* calling
 * qthread_initialize if you want to track qthread performance. This
 * should not be changed after qthread_initialize has been called.
 *
 * @param yesno Use 1 if you want tracking, zero otherwise
 */
void qtperf_set_instrument_qthreads(bool yesno);

/** @brief Display results to stdout
 *
 * This function prints a list of all performance data numbers, with
 * totals. The list includes state names and group names. States
 * within each data instance are printed one per line, with an extra
 * line between data instances. This function is mostly useful for
 * human interaction, use qtperf_print_delimited if you need to import
 * the data to another application for processing.
 *
 * @see qtperf_print_delimited
 */
void qtperf_print_results(void);

/** @brief Print results in comma (or other char) delimited tables
 *
 * This function allows you to print data out in a format that's easy
 * to import into other applications. You can choose an arbitrary
 * string to use for delimiters between items. Each data instance is
 * printed as a row in the table, with each state separated by a
 * delimiter. You can optionally define a prefix to use for each row
 * so that multiple tables can be printed and separated later using
 * grep and tee. If you want something more human readable, try
 * qtperf_print_results.
 *
 * Here's an example of a command line to split multiple tables into
 * separate files based on the row_prefix (in this case, '*' and "+'):
 * @code
   ./myprogram | tee >(egrep '^*" > stars.csv) >(egrep '^+' > pluses.csv)
 * @endcode
 * 
 * @param group The state group containing the instances you want to print
 * @param delim A string that should be printed between each performance counter in a row
 * @param print_headers Use 1 if you want to print the state names as table headers, zero to skip
 * @param row_prefix This string will be printed as the first column of each row to make grepping easier
 * @see qtperf_print_results
 */
void qtperf_print_delimited(qtstategroup_t* group, const char* delim, bool print_headers,const char* row_prefix);

/** @brief Return total time spent by instances of this state group
 * 
 * This function adds up all of the time for all of the instances of
 * the given state group and returns the result. This is used by
 * qtperf_print_results to indicate the amount of processor time used
 * by a group. This can be used to give an approximation of the
 * efficiency of parallel execution, by dividing the total by the
 * actual elapsed time (the total will be some multiple of the elapsed
 * time for parallel execution). The total time will only be
 * meaningful in this way if you have your threads enter
 * QTPERF_INVALID_STATE when they have finished their computations
 * (otherwise your own quiescent state will be added to the total
 * time, and you'll get *exactly* the number of threads as your
 * parallelism estimate).
 * 
 * @param group The state group to sum over.
 */
qtperfcounter_t qtperf_total_group_time(qtstategroup_t* group);

/** @brief Return the total time recorded in this perfdata struct
 *
 * This function returns the total time elapsed in all states of the
 * given perfdata instance. This will just be the total time that the
 * thread existed unless you use QTPERF_INVALID_STATE to indicate when
 * the thread should not be recording time data.
 *
 * @param data The pointer to the qtperfdata_t struct to sum over
 */
qtperfcounter_t qtperf_total_time(qtperfdata_t* data);

/** @brief Print the performance data for a single group in list format
 *
 * This function prints a group name, total time, and time for each
 * state for all qtperfdata_t instances of the given state group.
 *
 * @param group The group to print data for.
 * @see qtperf_print_perfdata
 * @see qtperf_print_results
 * @see qtperf_enter_state
 */
void qtperf_print_group(qtstategroup_t* group);

/** @brief Print the performance data for a single instance in list format
 *
 * This function prints the state names, total elapsed time, and group
 * name for the given qtperfdata_t structure. One line per state is
 * printed. If state names aren't defined for the state group
 * containing this instance, numeric identifiers will be printed
 * instead. You can choose to skip printing states that have no
 * recorded data (i.e. were never entered) using the second parameter.
 *
 * @param perfdata The data to print
 * @param show_states_with_zero_time One if you want to print states with no elapsed time, zero otherwise
 * @see qtperf_enter_state
 * @see qtperf_print_group
 * @see qtperf_print_results
 */
void qtperf_print_perfdata(qtperfdata_t* perfdata, bool show_states_with_zero_time);

/** @brief Make one state transition cause other state transitions as a side effect
 *
 * This function allows you to have a state transition for one
 * instance trigger state transitions for other instances
 * automatically. This is primarily useful for keeping tabs on
 * internal state transitions within the qthreads library from user
 * code. With this function, you can register to be notified when
 * workers or qthreads change state inside the scheduler. If you need
 * to measure the overhead imposed by various parts of the qthreads
 * library, and also need to correlate it with other events in your
 * own code, this is probably the easiest way to proceed.
 * 
 * \warning Note that you have *very* limited stack space in qthread
 * tasks, so having a large number of piggyback states will probably
 * cause trouble for you because each piggyback calls
 * qtperf_enter_state again. The typical number of piggybacks should
 * be zero, use this only in the case where you can't get the data to
 * work in another way.
 *  
 * @param source_data The qtperfdata_t structure that should cause other transitions
 * @param trigger_state The specific state that should cause other transitions
 * @param piggyback_data The qtperfdata_t that wants to be notified of a transition
 * @param piggyback_state The state that should be entered on the piggyback_data
 * @see qtperf_enter_state
 */
void qtperf_piggyback_state(qtperfdata_t* source_data, qtperfid_t trigger_state,
                            qtperfdata_t* piggyback_data, qtperfid_t piggyback_state);
/** @brief return the data counters for the current thread
 * 
 * This function returns the internal qtperfdata_t for the currently
 * executing qthread. You must call qtperf_set_instrument_qthreads(1)
 * before qthread_initialize or this function will simply return NULL.
 *
 * @see qtperf_set_instrument_qthreads
 */
qtperfdata_t* qtperf_get_qthread_data(void);

#ifdef QTPERF_TESTING
#include<stdarg.h>
#include<stddef.h>
#include<setjmp.h>
#include<string.h>
#include<cmocka.h>
#include<ctype.h>
#define QTPERF_ASSERT(...) assert_true(__VA_ARGS__)

/// Internal - verify that the state names array is well-formed
bool qtp_validate_names(const char** names, size_t count);
/// Internal - verify that the qtstategroup_t is well-formed
bool qtp_validate_state_group(qtstategroup_t*);
/// Internal - verify that the piggyback list is well-formed
bool qtp_validate_piggyback_list(qtperf_piggyback_list_t*);
/// Internal - verify that the piggyback lists are all well-formed
bool qtp_validate_piggybacks(qtperf_piggyback_list_t**, size_t);
/// Internal - verify that the perfctr struct is well-formed
bool qtp_validate_perfctr(qtperfctr_t* ctr);
/// Internal - verify that the perfdata struct is well-formed
bool qtp_validate_perfdata(qtperfdata_t*);
/// Internal - ensure qtperf_perf_list_t is well-formed
bool qtp_validate_perf_list(qtperf_perf_list_t* list, qtperf_perf_list_t** next, size_t expected_items);
/// Internal - verify that the group list is well-formed
bool qtp_validate_group_list(void);

/** @brief Verify that the performance data is internally consistent.
 * 
 * This function checks the data structures used by qtperf to ensure
 * that they are consistent. You should be able to call it at any
 * time, as long as another call into qtperf is not running
 * concurrently. You must #define QTPERF_TESTING when compiling
 * qthreads in order to use this function. If errors are found, this
 * function returns false (0). If you are using the cmocka test
 * framework, the particular failure will be indicated by the cmocka
 * test failure.
 */
bool qtperf_check_invariants(void);

#else // ifdef QTPERF_TESTING

// define away unless QTPERF_TESTING is defined, otherwise it's an
// alias for cmocka's assert_true
#define QTPERF_ASSERT(...)

#endif // ifdef QTPERF_TESTING


// Convenience macros to limit the prevalence of ifndef/endif blocks
// in the main code
#ifdef QTHREAD_PERFORMANCE
#define QTPERF_ENTER_STATE(...) qtperf_enter_state(__VA_ARGS__)
#define QTPERF_WORKER_ENTER_STATE(pdata,state) do{      \
    if(qtperf_should_instrument_workers){               \
      QTPERF_ASSERT(pdata != NULL);                     \
      qtperf_enter_state(pdata, state);                 \
    } } while(0)
#define QTPERF_QTHREAD_ENTER_STATE(pdata,state) do{     \
    if(qtperf_should_instrument_qthreads){              \
        QTPERF_ASSERT(pdata != NULL);                   \
        qtperf_enter_state(pdata, state);               \
    }} while(0)

#else
# define QTPERF_ENTER_STATE(...)
# define QTPERF_QTHREAD_ENTER_STATE(...)
# define QTPERF_WORKER_ENTER_STATE(...)
#endif // ifdef QTHREAD_PERFORMANCE


#endif

    /* Declarations of this file */
#ifdef __cplusplus
}
#endif
