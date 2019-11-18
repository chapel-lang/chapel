# Table of contents
1. [Introduction](#introduction)
2. [Quick note on memory management](#quick-note-on-memory-management)
3. [Tutorial](#tutorial)
    1. [Quickstart](#quickstart)
    2. [Setup Functions](#setup-functions)
        1. [Internal measurements](#internal-measurements)
        2. [State group management](#state-group-management)
    3. [Runtime Functions](#runtime-functions)
        1. `void qtperf_enter_state(qtperfdata_t* data, qtperfid_t state_id)`
        2. `const char* qtperf_state_name(qtstategroup_t* group, qtperfid_t state_id)`
        3. `void qtperf_start()`
        4. `void qtperf_stop()`
    4. [Reporting and data access functions](#report-and-data-access-functions)
        1. `void qtperf_print_results()`
        2. `void qtperf_print_delimited(qtstategroup_t* group, const char* delim, bool print_headers, const char* prefix)`
        3. `void qtperf_print_perfdata(qtperfdata_t* perfdata, bool show_states_with_zero_time)`
        4. `void qtperf_print_group(qtstategroup_t* group)`
        5. `void qtperf_print_perfdata(qtperfdata_t* data, bool show_states_with_zero_time)`
        6. `void qtperf_print_group(qtstategroup_t* group)`
        7. `qtperfcounter_t qtperf_total_group_time(qtstategroup_t* group)`
        8. `qtperfcounter_t qtperf_total_time(qtperfdata_t* data)`
    5. [Iterators](#iterators)
        1. `void qtperf_iter_begin(qtperf_iterator** iter)`
        2. `qtperfdata_t* qtperf_iter_next(qtperf_iterator_t** iter)`
        3. `qtperfdata_t* qtperf_iter_deref(qtperf_iterator_t * iter)`
        4. `qtperf_iterator_t* qtperf_iter_end()`
    6. [Teardown](#teardown)

# Introduction

The Qthreads performance measurement API (qtperf) is a set of
functions that make it easier to keep track of time-related data
during a run. It supports tracking of internal library timings for
measuring the actual overhead imposed by the library itself, and it
also supports arbitrary user-defined timing measurements.

Qtperf is modeled on the idea of a state machine. If you can model the
performance data you want to collect as a set of discrete states with
associated time spent in each state, you will be able to use this set
of functions to track it. The basic unit of data collection is a
state, which is just an arbitrary identifier. These are bundled into
state groups, which represent a state machine with all of its valid
states. After state groups are defined, you can instantiate them as
many times as necessary to track same-type data (for example, if you
want to measure overhead of communications in each thread, you could
define a communication state group and then instantiate it for each
thread that will be communicating). While the program is running, your
code can signal that a new state is being entered for the currently
active state group, and the library will add the elapsed time in the
previous state to its total, update the state variable, and return
control.

Instantiating a state group for every thread is often going to be too
much detail and overhead, so the library also supports
aggregation. When you have a large number of threads that will be
doing basically the same thing, you can define a state group to
represent the task, then instantiate it *once* and have all of the
threads post their updates to that shared instance. This requires the
limited use of mutual exclusion, so there will be a tradeoff in terms
of memory savings versus performance costs.

# Quick note on memory management

This library uses dynamically allocated memory to track the
performance data it captures. The only functions that perform
allocations are the functions with `create` in their names. You
should **not** manually deallocate any structures provided by this
library. Instead, call `qtperf_free_data()` at the end of your run,
and all memory will be safely deallocated.

# Tutorial

Qtperf is designed to be used with three distinct phases of
operation: setup, run, and teardown. This is necessary because of
memory allocation and cleanup requirements, and also because
instrumentation must be initialized for internal library states if
they are to be tracked.

## Quickstart

Here is a quick example of a tiny program that makes use of the
basic internal logging features of qtperf:
```
    #include<qthread/qthread.h>
    #include<qthread/performance.h>
    #include<qthread/logging.h>
    
    aligned_t spin(){
      size_t i=0;
      aligned_t result=2;
      for(i=0; i<1000000; i++){
        result = result * result + i;
      }
      return result;
    }
    
    size_t num_threads=5;
    int main(){
      size_t i=0;
      aligned_t ret=0;
    
      // Enable monitoring of qthread internal workers
      qtperf_set_instrument_workers(1);
    
      // Enable monitoring of internal qthreads (jobs)
      qtperf_set_instrument_qthreads(1);
    
      // Initialize and allocate data, enable collection
      qtperf_start();
    
      // Call this *after* qtperf_start()
      qthread_initialize();
    
      for(i=0; i<num_threads; i++){
        qthread_fork(spin, NULL,&ret);
      }
      for(i=0; i<num_threads; i++){
        qthread_readFE(NULL, &ret);
      }
      // Disable collection, you can switch on and off at will during a run
      qtperf_stop();
    
      // Print the results in a human readable format
      qtperf_print_results();
    
      // Deallocate everything. No more calls to qtperf_* after this!
      qtperf_free_data();
    
      return 0;
    }
```
Here is a program to demonstrate how to set up and use a custom state group:
```
    #include<qthread/qthread.h>
    #include<qthread/performance.h>
    #include<qthread/logging.h>
    
    static inline int spin_lock(volatile uint32_t* busy);
    
    static inline int spin_lock(volatile uint32_t* busy){
      int ret=0;
      while(qthread_cas32(busy, 0, 1) != 0){
        ret=1;
      }
      return ret;
    }
    
    typedef enum {
      SPIN_WORKING,
      SPIN_WAITING,
      SPIN_COMPLETE,
      SPIN_NUM_STATES
    } spinstate_t;
    
    const char* spin_names[] = {
      "SPIN_WORKING",
      "SPIN_WAITING",
      "SPIN_COMPLETE",
      "SPIN_NUM_STATES"
    };
    
    volatile uint32_t busy=0;
    int strct=0;
    aligned_t bit=0;
    
    #define SPIN 1
    aligned_t struct_edit(void*data){
      int start =0;
      size_t i=0;
      aligned_t ret=0;
      int interruptions=0;
      qtperfdata_t* mydata = (qtperfdata_t*)data;
    
      // Upon entry, timing is recorded for previous state
      qtperf_enter_state(mydata, SPIN_WORKING);
    
      for(i=0; i<100; i++){
        size_t j=0;
    
        // Multiple state transitions are expected, timing will not be
        // affected by transitioning to the same state multiple times.
        qtperf_enter_state(mydata, SPIN_WAITING);
    
        interruptions += spin_lock(&busy);
        qtperf_enter_state(mydata, SPIN_WORKING);
        start = strct;
        for(j=0; j<1000000; j++){
          strct = strct+1;
        }
        start = strct;
        busy = 0;
      }
    
      // Entering the QTPERF_INVALID_STATE state temporarily switches off
      // data collection for this instance. In this case, this thread is
      // now done so I want to switch it off.
      qtperf_enter_state(mydata, QTPERF_INVALID_STATE);
    
      qtlogargs(SPIN, "%d interruptions", interruptions);
      ret = strct;
      return ret;
    }
    
    typedef enum {
      RUNNING,
      DONE,
      TOTAL_NUM_STATES
    } total_t;
    const char* total_names[]={
      "RUNNING",
      "DONE"
    };
    
    int main() {
      aligned_t ret=0;
      size_t i=0;
      qtstategroup_t* spingroup=NULL;
      qtstategroup_t* totalgroup=NULL;
      qtperfdata_t* totaldata=NULL;
    
      // Enable collection and setup data structures. Library internals
      // will NOT be logged during this run, because
      // qtperf_set_instrument_* were not called.
      qtperf_start();
    
      qthread_initialize();
    
      // Create a state group for the spinstate_t state group. The names
      // array can be NULL, in which case the library will just report
      // numeric identifiers in the final output.
      spingroup=qtperf_create_state_group(SPIN_NUM_STATES, "Spin Testing", spin_names);
    
      // Create a state group for the total elapsed time.
      totalgroup=qtperf_create_state_group(TOTAL_NUM_STATES, "Total Time", total_names);
    
      // Instantiate the total elapsed time state group into a perfdata_t
      // structure to collect data for the run.
      totaldata=qtperf_create_perfdata(totalgroup);
    
      // Initially, perfdata_t are in the QTPERF_INVALID_STATE state, so
      // you must make at least one transition into your start state in
      // order to have logging enabled for the instance.
      qtperf_enter_state(totaldata,RUNNING);
    
      for(i=0; i<10; i++){
        // Create a new instance of the spinstate_t state group for each
        // thread.
        qtperfdata_t* spindata = qtperf_create_perfdata(spingroup);
    
        // The perfdata_t struct is passed through to the threads. If you
        // want to aggregate this, you can make use a global variable
        // because the perfdata will be shared between threads anyway.
        qthread_fork(struct_edit, (void*)spindata, &ret);
      }
      for(i=0; i<10; i++){
        qthread_readFE(NULL,&ret);
      }
      // Signal that the run is complete for the total elapsed time state
      // group.
      qtperf_enter_state(totaldata,DONE);
    
      // Stop collection. This is not final - collection could be
      // restarted if desired by a call to qtperf_start()
      qtperf_stop();
    
      // Display the results in a human readable format.
      qtperf_print_results();
    
      // signal the library to clean itself up.
      qtperf_free_data();
    
      return 0;
    }
```
API

## Setup Functions

Setting up qtperf varies a bit depending on what you want to
measure and how you want to track it. In the most minimal form, all
you have to do is call `qtperf_start()`, then allocate your groups
and you're off to the races. You can also tell the library to
record data from the qthreads internally, and from the library's
workers threads.

### Internal measurements

There are two ways to get internal data out of qthreads using
qtperf. You can measure either the workers (typically a smaller
number of actual operating system threads that implement the work
stealing behavior of qthreads), or you can instrument the qthreads
themselves (which represent tasks visible to the user of the
library). These measurements can also be made together during the
same run.

In all cases, you must inform the library that you want to record
this data **before** you call `qthread_initialize()`.

1.  `qtperf_set_instrument_workers(int yes_or_no)`

    This function will create a state group that allows you to
    measure internal timing data for the qthreads library's
    workers. Qthreads implements a system of work stealing, in which
    a (relatively) small number of system threads aggressively switch
    between a large number of task threads so as to maximize the time
    spent actually doing productive work. This instrumentation flag
    allows you to see how the workers themselves are doing by
    measuring their idle versus busy time. Using this data you can
    determine how well the processors are being utilized.

2.  `qtperf_set_instrument_qthreads(int yes_or_no)`

    This function allows you to measure the overhead of the qthreads
    scheduling systems in sending task work to the worker
    threads. Each qthread you create will be monitored to measure how
    much time is spent on task versus stalled in various parts of the
    qthreads system.

### State group management

The main abstraction that this API relies upon is a state
group. As the user, you will define a set of states that your code
can be in, tell the library when the code makes a transition, and
the library will track the amount of time spent in each state.

A state group is a connected set of states - these states are
allowed to have transitions to each other. Transitions can **only**
be between states that are in the same state group. This is a
constraint that the library has only limited power to enforce. If
a transition is requested to a state that is out of bounds, the
library will flag the error, but if the state number is valid for
the group it will just complete the transition, even if you
mistakenly used the wrong enum variant in the source code. 

A state group represents an abstract set of states. In order to
actually attach those states to a thread and measure something,
you need to make a `qtperfdata_t` struct.

1.  `qtstategroup_t* qtperf_create_state_group(size_t num_states, const char* group_name, const char** state_names)`

    This function creates a new state group. You must provide a name,
    but the array of state names is optional. If no state names are
    provided, the library will simply assign numeric identifiers to
    the states when output is requested.
    
    The value returned is a newly-malloc'd qtstategroup<sub>t</sub>
    structure. This structure is meant to be mostly opaque to users,
    though you can access its fields if you need to as with any other
    struct. For details of its contents, see `qthreads/performance.h`.

2.  `qtperfdata_t* qtperf_create_perfdata(qtstategroup_t* group)`

    This creates a new performance counters structure
    (`qtperfdata_t`) that will use the given state group to measure
    transitions. This function should be called for each thread that
    you want to measure. Data logged here will be kept separate from
    other threads. If you want to log data from a group of similar
    threads into the same performance counters structure, use
    `qtperf_create_aggregated_perfdata()` (see below).
    
    The value returned from this function should be treated as an
    opaque identifier unless you are very sure of what you need to do
    to it. In order to cause a state transition for this counter,
    call `qtperf_enter_state` with it.

3.  `qtperfdata_t* qtperf_create_aggregated_perfdata(qtstategroup_t* group)`

    This function is similar to `qtperf_create_perfdata` except that
    it creates a performance counter that is intended to be shared by
    many threads during a run. In some cases, large numbers of
    threads are doing essentially the same task, and the data you
    really want is how the overall task performed for the whole
    system. This function is intended to support that use case.
    
    If you need to measure data for each thread separately, use
    `qtperf_create_perfdata` instead.

4.  `void qtperf_piggyback_state(...)`

    Arguments:
    
    1.  `qtperfdata_t* source_data` - The perfdata you want to
        piggyback onto
    
    2.  `qtperfid_t trigger_state` - Trigger piggyback when the source
        data enters this state
    
    3.  `qtperfdata_t* piggyback_data` - The counter you want to
        attach to the source
    
    4.  `qtperfid_t piggyback_state` - The state that the piggyback
        should enter when the trigger condition is met
    
    The performance library allows you to add "piggybacks" onto other
    states. This means that you can set it up so that a state
    transition in one performance counter triggers a state transition
    in another performance counter automatically. The primary use of
    this is to allow you to pull data out of the internal
    instrumentation states and into your own state groups. For
    example, you might want to record the time a thread spends
    communicating via an external library, processing the data, and
    waiting for the qthreads library to schedule it. You would
    piggyback your performance counter onto the qthreads internal
    performance counters so that the library's internal state changes
    are recorded in your own structure. That gives you an easy way to
    monitor the internal overhead of the library compared with the
    task-related states in a given thread.
    
    1.  Performance impacts! Caution!
    
        There are two things to watch out for if you start using
        piggybacks heavily. First, it is possible to create a cycle, in
        which case your thread will enter an infinite loop and
        hang. This is easy to detect in most cases and you'll find the
        bug quickly. The second issue to look out for is that each
        piggyback you add to a `perfdata_t` adds a small constant amount
        of execution time to each state transition it makes. If you add
        a large number of piggybacks, or if you have a bunch of
        piggybacks attached in a chain, you may see a performance impact
        from processing all of the state transitions. Use piggybacks
        sparingly.

## Runtime Functions

These functions are intended for use while the experiment is
running. They are oriented toward making measurements as painlessly
as possible, and also support getting output at various points.

### `void qtperf_enter_state(qtperfdata_t* data, qtperfid_t state_id)`

This is the function that you should call each time you want to
record that something has changed. The data argument is the
pointer returned by the `qtperf_create_*_perfdata` functions, and
the state<sub>id</sub> is the identifier of the state you are entering. The
library will sample the current time, subtract from that the time
that the current state was entered, and add the difference to the
current state's total before entering the new state. If the
perfdata<sub>t</sub> is an aggregated collector, it will also ensure that
only one thread is updating at a time using a CAS-based spin lock.

### `const char* qtperf_state_name(qtstategroup_t* group, qtperfid_t state_id)`

This just returns the state name you provided when the state group
was created, for convenience. If you did not define names, this
function returns `NULL`.

### `void qtperf_start()`

This function enables data recording. It and its opposite function
(`qtperf_stop`) can be called any number of times while the
program is running to turn collection on and off as needed.

### `void qtperf_stop()`

This function halts collection globally. You can restart it with
`qtperf_start`, and both can be called at any time during program
execution without danger.

## Reporting and data access functions

These functions allow you to get the recorded data out of the
library in various forms. Currently, you can have data reported as
human-readable text or comma-separated values, and you can get
access directly to the data structures themselves via an iterator
for low-level or programmatic access.

### `void qtperf_print_results()`

Print out all of the data for all of the counters, using the
human-readable format provided by `qtperf_print_delimited`. States
with zero time will be omitted for brevity.

### `void qtperf_print_delimited(qtstategroup_t* group, const char* delim, bool print_headers, const char* prefix)`

This function is intended to make it easy to export data from a
run into other tools by printing it in tabular, delimited
format. It prints out by state *group*, so all instances of that
group will be printed, one in each row of the table. You can
specify what delimiter to use within rows with `delim`, and you
can also specify an optional prefix to be printed at column zero
of each row (including the optional header). If `print_headers` is
true, the name of each state will be printed in a header row so
that the columns of the table are identifiable.

The prefix allows you to easily split out a number of different
tables froma single run and divert them to their own files with a
simple command line. For example, if you use bash for your shell
and have two tables you want to keep separate, you can set one
delimiter to '\*' and the other to "+", then use this command to
put the data from a single run into two separate files:

`./program | tee >(egrep '^\*' > stars.csv) >(egrep '^\+' > pluses.csv)`

This will create a file called `stars.csv` that has all lines that
begin with '\*', and a separate file called `pluses.csv` that has
all lines that begin with '+'.

### `void qtperf_print_perfdata(qtperfdata_t* perfdata, bool show_states_with_zero_time)`

This prints the states in a human-readable format along with their
recorded times. If `show_states_with_zero_time` is non-zero, it
will display all states regardless of their time data. Otherwise,
it will only display states that have non-zero tick counts. This
is mostly useful if you have a number of states that aren't used
yet but might be in the future.

### `void qtperf_print_group(qtstategroup_t* group)`

Print the performance data for a state group in human-readable
list format. This will print a list of all instances of this
group, along with some summary information for the group as a
whole.

### `void qtperf_print_perfdata(qtperfdata_t* data, bool show_states_with_zero_time)`

This prints a single entry in the human-readable list format. If
`show_states_with_zero_time` is true, it will print an entry for
all states, otherwise it will only print if the state actually has
some time recorded.

### `qtperfcounter_t qtperf_total_group_time(qtstategroup_t* group)`

This function adds up all of the time for all of the instances of
the given state group and returns the result.A

### `qtperfcounter_t qtperf_total_time(qtperfdata_t* data)`

This function returns the total time elapsed in all states of the
given perfdata instance. If you put your perfdata into the
`QTHREAD_INVALID_STATE` state when it finishes or is otherwise not
executing, you can use this function to get a simple measurement
of how long the thread was actually running (because time is not
recorded when the thread is in `QTHREAD_INVALID_STATE`).

## Iterators

A simple iterator is provided for traversing through the
performance data. The iterator traverses stategroups, hitting all
perfcounters within a stategroup before moving to the next
stategroup.  In order to avoid allocation, the iterator interface
expects you to provide it with an iterator struct to use. This is
typically done by declaring a `qtperf_iterator_t` as a local
variable, then declaring **another** local variable that points to
the iterator itself, and passing a **pointer** to that pointer for
the iterator functions. This allows the API to initialize and
update the iterator struct, and to indicate when the end of the
chain has been reached by returning NULL. A basic example of use
would look like this:

    // ... 
      qtperfdata_t* iterdata = NULL;
      qtperf_iterator_t iterator;
      qtperf_iterator_t* iter=&iterator;
      qtperf_iter_begin(&iter);
    
      for(iterdata = qtperf_iter_next(&iter);
          iterdata != NULL;
          iterdata = qtperf_iter_next(&iter)){
        // .. do something with the data
      }
    // ...

That snippet of code would loop through all of the perfdata<sub>t</sub>
instances, ordered by state group, and execute the body of the
`for` loop on each one.

The primary use for this is to provide access to the raw data in a
way that will remain consistent when internal changes occur, so
that you can do custom data processing.  If you find you need to
access to the internal data structures, try to do it using this API
so that future changes to the library will be less likely to break
your code.

### `void qtperf_iter_begin(qtperf_iterator** iter)`

This function initializes a new iterator. `iter` should be a
pointer to a pointer that has been initialized to point at a
`qtperf_iterator_t` that you allocated (usually on the stack, by
simply declaring it as a local variable). This function **will not**
allocate a new iterator, so don't pass it `NULL`, or a pointer to
`NULL`.

### `qtperfdata_t* qtperf_iter_next(qtperf_iterator_t** iter)`

This function advances the iterator one slot, and returns the
`qtperfdata_t` it was pointing at before it advanced. You can use
this in the termination condition of a loop as described at the
top of this section.

### `qtperfdata_t* qtperf_iter_deref(qtperf_iterator_t * iter)`

This function returns the `qtperfdata_t` that the iterator is
currently pointing at. If the iterator is exhausted, it will
return NULL.

### `qtperf_iterator_t* qtperf_iter_end()`

This function returns a termination value that you can use to
compare with your iterator to see if you're done. Currently, this
is just NULL, but in the future it may be a value that points to
something else. The correct way to check to see if your iterator
is done is with the comparison `myiter == qtperf_iterator_end()`.

## Teardown

Stopping the performance system and cleaning up is done with a
single function call to `qtperf_free_data()`. It has no arguments
and returns `void`, just call it at the end of your run to free up
resources, particularly if your program will continue running after
performance measurement is no longer needed.
