#include<qthread/performance.h>
#include<qthread/logging.h>
#include<qthread/qthread.h>
#include"qt_threadstate.h"
#include"qt_qthread_mgmt.h"
#include"qt_qthread_struct.h"
#include<string.h>
#include<strings.h>
#include<stdlib.h>
#include<time.h>

#ifdef QTHREAD_PERFORMANCE

qtperf_group_list_t* _groups=NULL;
qtperf_group_list_t** _next_group=NULL;
size_t _num_groups=0;
size_t _num_counters=0;
bool _collecting=0;
bool qtperf_should_instrument_workers = 0;
bool qtperf_should_instrument_qthreads = 0;
qtstategroup_t* qtperf_workers_group = NULL;
qtstategroup_t* qtperf_qthreads_group = NULL;

volatile uint32_t _group_busy=0;
volatile uint32_t _perf_busy=0;

bool incr_counter(qtperf_iterator_t**);
bool incr_group(qtperf_iterator_t**);
void qtperf_free_state_group_internals(qtstategroup_t*);
void qtperf_free_group_list(void);
void qtperf_free_perfdata_internals(qtperfdata_t*);
void qtperf_free_perf_list(qtstategroup_t*,qtperf_perf_list_t*);
static inline void spin_lock(volatile uint32_t* busy);

static inline void spin_lock(volatile uint32_t* busy){
  bool stopped = 0;
  while(qthread_cas32(busy, 0, 1) != 0){
    stopped = 1;
  }
  if(stopped){
    qtlog(LOGWARN, "Stopped in spinlock");
  }
}

qtstategroup_t* qtperf_create_state_group(size_t num_states, const char* name, const char** state_names) {
  qtperf_group_list_t* current = NULL;
  spin_lock(&_group_busy);
  if(_next_group == NULL){
    _next_group = &_groups;
  }
  *_next_group = malloc(sizeof(qtperf_group_list_t));
  qtlogargs(PERFDBG, "create group %p", *_next_group);
  current = *_next_group;
  _next_group = &current->next;
  current->next = NULL;
  current->group.num_states = num_states;
  if(name != NULL){
    current->group.name = strdup(name);
  } else {
    current->group.name = strdup("Unnamed group");
  }
  current->group.state_names = NULL;
  current->group.num_counters = 0;
  current->group.next_counter = NULL;
  current->group.counters = NULL;
  if(state_names != NULL){
    size_t i=0; 
    current->group.state_names = calloc(num_states,sizeof(char*));
    for(i=0; i<num_states; i++){
      current->group.state_names[i] = strdup(state_names[i]);
    }
  }
  _num_groups++;
  _group_busy=0;
  return &current->group;
}

/* INTERNAL - free allocated memory from a stategroup structure */
void qtperf_free_state_group_internals(qtstategroup_t* group) {
  free(group->name);
  qtperf_free_perf_list(group,group->counters);
  if(group->state_names != NULL){
    size_t i=0;
    for(i=0; i<group->num_states; i++){
      free(group->state_names[i]);
    }
    memset(group->state_names, 0, sizeof(char*)*group->num_states);
    free(group->state_names);
    group->name = NULL;
    group->state_names = NULL;
    group->counters = NULL;
    group->next_counter = NULL;
    QTPERF_ASSERT(group->num_counters == 0);
  }
}
/* Free the entire list of state groups */
void qtperf_free_group_list(){
  qtperf_group_list_t* next=NULL;
  qtlogargs(PERFDBG, "num_groups = %lu", _num_groups);
  while(_groups != NULL){
    next = _groups->next;
    qtperf_free_state_group_internals(&_groups->group);
    _groups->next = NULL;
    qtlogargs(PERFDBG, "free group %p", _groups);
    free(_groups);
    _groups = next;
    _num_groups--;
  }
  QTPERF_ASSERT(_num_groups == 0);
}

/** Create a qtperfdata_t struct that points to the given aggregate
    counter data. If the counter provided is null, a new one will be
    created for this struct. You can then use the returned qtperfctr_t
    (inside the returned qtperdata_t) in future calls to this function
    and all additional threads will log their data to the first
    qtperfctr_t.
 */
qtperfdata_t* qtperf_create_aggregated_perfdata(qtstategroup_t* state_group, qtperfctr_t* aggregate){
  qtperf_perf_list_t* current=NULL;
  spin_lock(&_perf_busy);
  if(state_group->next_counter == NULL){
    state_group->next_counter = &state_group->counters;
  }
  *state_group->next_counter = malloc(sizeof(qtperf_perf_list_t));
  QTPERF_ASSERT(*state_group->next_counter != NULL && "out of memory?!");
  current = *state_group->next_counter;
  memset(current, 0, sizeof(qtperf_perf_list_t));
  state_group->next_counter = &current->next;
  current->next = NULL;
  current->performance_data.current_state=QTPERF_INVALID_STATE;
  current->performance_data.time_entered=0;
  current->performance_data.piggybacks = NULL;
  state_group->num_counters++;
  if(aggregate != NULL) {
    current->performance_data.counters = aggregate;
    spin_lock(&current->performance_data.counters->busy);
    current->performance_data.counters->num_contributors++;
    current->performance_data.counters->busy = 0;
  } else {
    qtperfctr_t* ctr = malloc(sizeof(qtperfctr_t));
    memset(ctr, 0, sizeof(qtperfctr_t));
    ctr->data = calloc(state_group->num_states,sizeof(qtperfcounter_t));
    memset(ctr->data, 0, sizeof(qtperfcounter_t)*state_group->num_states);
    ctr->busy = 1;
    current->performance_data.ctr_owner = 1;
    ctr->state_group = state_group;
    ctr->num_states = state_group->num_states;
    ctr->num_contributors = 1;
    current->performance_data.counters = ctr;
    ctr->busy = 0;
  }
  _perf_busy = 0;
  return &current->performance_data;
}

qtperfdata_t* qtperf_create_perfdata(qtstategroup_t* state_group) {
  return qtperf_create_aggregated_perfdata(state_group, NULL);
}

void qtperf_free_perfdata_internals(qtperfdata_t* perfdata) {
  // don't spin lock here, it will deadlock from qtperf_free_perf_list
  if(perfdata->piggybacks != NULL) {
    size_t index=0;
    for(index=0; index < perfdata->counters->num_states; index++){
      qtperf_piggyback_list_t* list = perfdata->piggybacks[index];
      while(list != NULL){
        qtperf_piggyback_list_t* next = list->next;
        qt_free(list);
        list = next;
      }
    }
    free(perfdata->piggybacks);
    perfdata->piggybacks=NULL;
  }
  spin_lock(&perfdata->counters->busy);
  perfdata->counters->num_contributors--;
  if(perfdata->counters->num_contributors < 1){
    qt_free(perfdata->counters->data);
    qt_free(perfdata->counters);
  } else {
    perfdata->counters->busy = 0;
  }
  perfdata->counters = NULL;
}

void qtperf_free_perf_list(qtstategroup_t* group, qtperf_perf_list_t* counters){
  qtperf_perf_list_t* next = NULL;
  while(counters != NULL){
    spin_lock(&counters->performance_data.busy);
    next = counters->next;
    qtperf_free_perfdata_internals(&counters->performance_data);
    counters->next = NULL;
    free(counters);
    counters = next;
    group->num_counters--;
  }
  group->next_counter = &group->counters;
}

qttimestamp_t qtperf_now(){
  qttimestamp_t time=0;
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  time = ts.tv_sec * 1000000 + ts.tv_nsec/1000;
  return time;
}

void qtperf_start(){
  qtperfcounter_t now = qtperf_now();
  qtperf_iterator_t iter_box;
  qtperf_iterator_t* iter=&iter_box;
  qtperfdata_t* data = NULL;
  volatile uint32_t* last_busy=NULL;
  if(_collecting == 1){
    return; // already collecting
  }
  _collecting = 1;
  // Now I need to resume the counters, but pretend that any elapsed
  // time did not actually happen (set time_entered to now)
  qtperf_iter_begin(&iter);
  for(data = qtperf_iter_next(&iter); data != NULL; data = qtperf_iter_next(&iter)){
    if(last_busy != NULL)
      *last_busy=0; // release previous 
    last_busy=&data->busy;
    spin_lock(&data->busy);
    if(data->current_state != QTPERF_INVALID_STATE){
      data->time_entered = now;
    }
  }
  if(last_busy != NULL){
    *last_busy = 0;// release last 
  }
}

void qtperf_stop() {
  qtperfcounter_t now = qtperf_now();
  qtperf_iterator_t iter_box;
  qtperf_iterator_t* iter=&iter_box;
  qtperfdata_t* data = NULL;
  volatile uint32_t* last_busy=NULL;
  if(_collecting == 0){
    return; // already stopped
  }
  _collecting = 0;
  // Now I need to record the time spent in the current state for all
  // active records
  qtperf_iter_begin(&iter);
  for(data = qtperf_iter_next(&iter); data != NULL; data = qtperf_iter_next(&iter)){
    if(last_busy != NULL){
      *last_busy = 0; // release previous
    }
    last_busy = &data->busy;
    spin_lock(&data->busy);
    // need the time_entered check because the thread may have entered
    // its first valid state between when _collecting was zeroed and
    // this check. In that case, the thread would be in a valid state
    // but the time_entered would not be set, and therefore we should
    // have a zero elapsed time for the state.
    if(data->current_state != QTPERF_INVALID_STATE && data->time_entered != 0){
      spin_lock(&data->counters->busy);
      data->counters->data[data->current_state] += now - data->time_entered;
      data->time_entered = now;
      data->counters->busy = 0;
    }
  }
  if(last_busy != NULL){
    *last_busy = 0; // release last
  }
}

void qtperf_free_data(){
  // Stop all workers, otherwise we could have a crash
  qtperf_set_instrument_workers(0);
  qtperf_set_instrument_qthreads(0);
  qtperf_free_group_list();
  _groups = NULL;
  _next_group = NULL;
  _collecting = 0;
}

const char* qtperf_state_name(qtstategroup_t* group, qtperfid_t state){
  if(state >= group->num_states){
    qtlogargs(LOGERR, "ERROR - STATE NUMBER %lu OUT OF BOUNDS FOR GROUP", state);
    return "ERROR";
  } else if(group->state_names == NULL){
    qtlog(LOGERR, "ERROR - GROUP DOES NOT HAVE STATE NAMES DEFINED");
    return "ERROR";
  }
  return group->state_names[state];
}

// This function will still change states even if _collecting is
// false, but the timing data will not be recorded.
void qtperf_enter_state(qtperfdata_t* data, qtperfid_t state){
  qttimestamp_t now = qtperf_now();
  qtperfid_t from_state = data->current_state;
  spin_lock(&data->busy);
  if(state != QTPERF_INVALID_STATE && state >= data->counters->state_group->num_states) {
    qtlogargs(LOGERR,"State number %lu is out of bounds!", state);
    data->busy = 0;
    return;
  }
  if(_collecting && from_state != QTPERF_INVALID_STATE) {
    if(data->time_entered < now/2){
      qtlogargs(LOGWARN, "Warning: entering state with invalid time_entered value %lu", data->time_entered);
    }
    spin_lock(&data->counters->busy);
    data->counters->data[from_state] += now - data->time_entered;
    data->counters->busy = 0;
  }
  data->current_state = state;
  // threads in QTPERF_INVALID_STATE should not log data. This can be
  // used to switch off logging for particular threads in lieu of
  // having to use the qtperf_start() and qtperf_stop() methods, which
  // affect all threads.
  if(state == QTPERF_INVALID_STATE){
    data->time_entered = 0;
  } else {
    data->time_entered = now;
    if(data->piggybacks != NULL && data->piggybacks[data->current_state] != NULL){
      qtperf_piggyback_list_t* current = data->piggybacks[data->current_state];
      for(; current != NULL; current = current->next){
        // WARNING! recursive call, make sure the length of this chain
        // is very short.
        qtperf_enter_state(current->target_data, current->target_state);
      }
    }
  }
  data->busy = 0;
}

void qtperf_iter_begin(qtperf_iterator_t**iter){
  (*iter)->current_group = _groups;
  if((*iter)->current_group){
    (*iter)->current = _groups->group.counters;
  } else {
    (*iter)->current = NULL;
  }
}

qtperfdata_t* qtperf_iter_deref(qtperf_iterator_t* iter){
  if(iter == qtperf_iter_end()){
    return NULL;
  }
  return &iter->current->performance_data;
}

// First cycle through the counters within a group. This function
// takes the iterator to the next counter.
bool incr_counter(qtperf_iterator_t** iter){
  if(*iter != qtperf_iter_end() && (*iter)->current != NULL){
    (*iter)->current = (*iter)->current->next;
  }
  return (*iter)->current != NULL;
}

// Cycle through groups when the counters in a group are
// exhausted. This function takes the iterator to the next group.
bool incr_group(qtperf_iterator_t** iter){
  if(*iter != qtperf_iter_end() && (*iter)->current_group != NULL){
    (*iter)->current_group = (*iter)->current_group->next;
    if((*iter)->current_group != NULL){
      (*iter)->current = (*iter)->current_group->group.counters;
    }
  }
  return (*iter)->current_group != NULL;
}

qtperfdata_t* qtperf_iter_next(qtperf_iterator_t** iter){
  qtperfdata_t* data = qtperf_iter_deref(*iter);
  // try to increment the counter. If that fails, try to increment the
  // group. If that also fails, the iterator is finished.
  if(!incr_counter(iter) && !incr_group(iter)){
    (*iter) = qtperf_iter_end();
    return NULL;
  }
  return data;
}

qtperf_iterator_t* qtperf_iter_end(){
  return NULL;
}


/* This section is for the instrumentation of workers */

static const char* worker_state_names[]={
  "WKR_INIT",
  "WKR_QTHREAD_ACTIVE",
  "WKR_SHEPHERD",
  "WKR_IDLE",
  "WKR_BLOCKED"
};

// need to set a flag that's visible to other files that can be used
// to control whether samples are recorded for worker state
// transitions. For now I'll see if I can use an inline function
// definition from performance.h
void qtperf_set_instrument_workers(bool yes_no){
  qtperf_should_instrument_workers = yes_no;
  // TODO: add code to instrument existing workers. As it stands, this
  // only affects workers spawned *after* this call, which means that
  // you have to call this function before qthread_initialize();

  // initialize the state group for worker data collection
  if(yes_no && qtperf_workers_group == NULL){
    qtperf_workers_group = qtperf_create_state_group(WKR_NUM_STATES, "Workers Internal", worker_state_names);
  }
  if(qtperf_should_instrument_workers){
    QTPERF_ASSERT(qtperf_should_instrument_workers && (qtperf_workers_group != NULL));
  }
}


/* This section is for instrumentation of qthreads */
static const char* qthread_state_names[] ={
    "QTHREAD_STATE_NASCENT",              /* awaiting preconds */
    "QTHREAD_STATE_NEW",                  /* first ready-to-run state */
    "QTHREAD_STATE_RUNNING",              /* ready-to-run */
    "QTHREAD_STATE_YIELDED",              /* reschedule, otherwise ready-to-run */
    "QTHREAD_STATE_YIELDED_NEAR",         /* reschedule, otherwise ready-to-run */
    "QTHREAD_STATE_QUEUE",                /* insert me into a qthread_queue_t */
    "QTHREAD_STATE_FEB_BLOCKED",          /* waiting for feb */
    "QTHREAD_STATE_PARENT_YIELD",         /* parent is moving into QTHREAD_STATE_PARENT_BLOCKED */
    "QTHREAD_STATE_PARENT_BLOCKED",       /* waiting for child to take this execution */
    "QTHREAD_STATE_PARENT_UNBLOCKED",     /* child is picking up parent execution */
    "QTHREAD_STATE_ASSASSINATED",         /* thread destroyed via signal; needs cleanup */
    "QTHREAD_STATE_TERMINATED",           /* thread function returned */
    "QTHREAD_STATE_MIGRATING",            /* thread needs to be moved, otherwise ready-to-run */
    "QTHREAD_STATE_SYSCALL",              /* thread performing external blocking operation */
    "QTHREAD_STATE_ILLEGAL",              /* illegal state */
    "QTHREAD_STATE_TERM_SHEP"             /* special flag to terminate the shepherd */
};

void qtperf_set_instrument_qthreads(bool yes_no) {
  QTPERF_ASSERT(QTHREAD_STATE_NUM_STATES == 16
                && "threadstate_t has changed, check to make sure all states are represented in qthread_state_names in performance.c" );// make sure we're still current with our names array.
  qtperf_should_instrument_qthreads = yes_no;

  if(yes_no && qtperf_qthreads_group == NULL){
    qtperf_qthreads_group = qtperf_create_state_group(QTHREAD_STATE_NUM_STATES, "Qthreads Internal", qthread_state_names);
  }
  if(qtperf_should_instrument_qthreads){
    QTPERF_ASSERT(qtperf_should_instrument_qthreads && (qtperf_qthreads_group != NULL));
  }
}


void qtperf_print_results(){
  qtperf_group_list_t* current = NULL;
  for(current = _groups; current != NULL; current = current->next){
    qtperf_print_group(&current->group);
  }
}
/* PRINTING RESULTS */

qtperfcounter_t qtperf_total_group_time(qtstategroup_t* group){
  qtperf_perf_list_t* current = NULL;
  qtperfcounter_t total=0;
  for(current = group->counters; current != NULL; current = current->next){
    total += qtperf_total_time(&current->performance_data);
  }
  return total;
}

qtperfcounter_t qtperf_total_time(qtperfdata_t* data){
  size_t i=0;
  qtperfcounter_t result=0;
  for(i=0; i<data->counters->num_states; i++){
    result += data->counters->data[i];
  }
  return result;
}

void qtperf_piggyback_state(qtperfdata_t* source_data, qtperfid_t trigger_state,
                            qtperfdata_t* piggyback_data, qtperfid_t piggyback_state){
  qtperf_piggyback_list_t* next = NULL;
  QTPERF_ASSERT(source_data != NULL);
  spin_lock(&source_data->busy);
  if(source_data->piggybacks==NULL){
    source_data->piggybacks = calloc(source_data->counters->num_states, sizeof(qtperfdata_t*));
    memset(source_data->piggybacks, 0, sizeof(qtperfdata_t*) * source_data->counters->num_states);
  }
  next = malloc(sizeof(qtperf_piggyback_list_t));
  next->next = source_data->piggybacks[trigger_state];
  next->target_data = piggyback_data;
  next->target_state = piggyback_state;
  source_data->piggybacks[trigger_state] = next;
  source_data->busy = 0;
}

qtperfdata_t* qtperf_get_qthread_data(void){
  qthread_t* me = qthread_internal_self();
  if(me->rdata->performance_data == NULL){
    qtlog(LOGWARN, "qtperf_get_qthread_data() called, but qtperf_set_instrument_qthreads() has not been called");
    return NULL;
  }
  return me->rdata->performance_data;
}
/**
 * qtperf_print_delimited prints the data for a state group as a
 * table, one row per instance of the state group, with columns
 * separated by the string given in sep. The first column is the
 * instance number, then each subsequent column gives the time spent
 * in the corresponding state starting with zero and going up from
 * there. You can use this function to get data that's easy to import
 * into a spreadsheet (e.g CSV). Provide a row_prefix if you want to
 * be able to print multiple tables at a time and use grep to
 * separate them in the output. row_prefix can be NULL. If you do
 * choose to output multiple tables, you can split them out to
 * separate files using output redirection in bash like this
 * (assuming you have two tables, one with the output prefix '-' and
 * the other with '+'):
 *
 * myprog | tee >(egrep '^-' > table1.csv) >(egrep '^\+' > table2.csv)
 *
 * You can also use the program 'q' (https://github.com/harelba/q) to
 * make queries against the csv output as if it was a sqlite3
 * database:
 *
 * myprog | egrep '^-' | q -d, 'select c2,1.0*c1/c2 as ratio from - order by ratio'
 */

void qtperf_print_delimited(qtstategroup_t* group, const char* sep, bool print_headers, const char* row_prefix){
  qtperf_perf_list_t* current = NULL;
  size_t column=0;
  size_t i=0;
  const char* pfx="";
  if(row_prefix != NULL)
    pfx=row_prefix;
  if(print_headers && group->state_names != NULL){
    printf("%sIndex%s", pfx, sep);
    for(column=0; column<group->num_states; column++){
      if(column+1 < group->num_states){
        printf("%s%s", group->state_names[column], sep);
      } else{
        printf("%s\n", group->state_names[column]);
      } 
    }
  }
  for(current = group->counters, i=0; current != NULL; current = current->next, i++){
    printf("%s%lu%s", pfx,i, sep);
    for(column=0; column < group->num_states; column++){
      if(column+1 < group->num_states){
        printf("%llu%s",  current->performance_data.counters->data[column], sep);
      }else{
        printf("%llu\n",  current->performance_data.counters->data[column]);
      }
    }
  }
}

#ifndef PERF_SHOW_STATES_WITH_ZERO_TIME
#define PERF_SHOW_STATES_WITH_ZERO_TIME 1
#endif

void qtperf_print_group(qtstategroup_t* group){
  qtperf_perf_list_t* current = NULL;
  size_t i=0;
  printf("%s (%lu instances, %lu total states, total time=%llu)\n", group->name, group->num_counters, group->num_states, qtperf_total_group_time(group));
  for(current = group->counters,i=0; current != NULL; current = current->next,i++){
    printf("  Instance %lu, total time %llu:\n", i, qtperf_total_time(&current->performance_data));
    qtperf_print_perfdata(&current->performance_data, PERF_SHOW_STATES_WITH_ZERO_TIME);
  }
  printf("------------------------------------------------\n");
}

void qtperf_print_perfdata(qtperfdata_t* perfdata, bool show_zeros){
  size_t i=0;
  const char** names = (const char**)perfdata->counters->state_group->state_names;
  qtperfcounter_t* data = perfdata->counters->data;
  if(!perfdata->ctr_owner) {// don't print individual records for aggregated groups
    return;
  }
  spin_lock(&perfdata->busy);
  spin_lock(&perfdata->counters->busy);
  for(i=0; i<perfdata->counters->state_group->num_states; i++){
    if(show_zeros || (data[i] != 0)){
      if(names != NULL){
        printf("    %s: %llu\n", names[i], data[i]);
      }else{
        printf("    state %lu: %llu\n", i, data[i]);
      }
    }
  }
  perfdata->counters->busy = 0;
  perfdata->busy = 0;
}

/* Testing related functionality */

#ifdef QTPERF_TESTING

#define MAX_NAME_LENGTH 128
bool qtp_validate_names(const char** names, size_t count){
  size_t i=0;
  bool valid = 1;
  for(i=0; names != NULL && i<count; i++){
    size_t len=0;
    bool printable=1;
    for(len=0; names[i][len] != '\0' && len < MAX_NAME_LENGTH; len++){
      printable = printable && isprint(names[i][len]);
    }
    valid = valid && printable && len < MAX_NAME_LENGTH && len > 0 && names[i][len] == '\0';
    assert_true(printable && len < MAX_NAME_LENGTH && len > 0 && names[i][len] == '\0');
  }
  assert_true(names == NULL || valid);
  return valid;
}

bool qtp_validate_piggyback_list(qtperf_piggyback_list_t* list){
  bool valid = 1;
  qtperf_piggyback_list_t* current = list;
  while(current != NULL) {
    assert_true(valid = valid && (current->target_data != NULL));
    assert_true(valid = valid && (current->target_state < current->target_data->counters->num_states));
    current = current->next;
  }
  assert_true(current == NULL);
  return valid && current == NULL;
}

bool qtp_validate_piggybacks(qtperf_piggyback_list_t** piggybacks, size_t num_states){
  size_t counter=0;
  bool valid = 1;
  if(piggybacks == NULL){
    return valid;
  }
  for(counter=0; counter < num_states; counter++){
    valid = valid && qtp_validate_piggyback_list(piggybacks[counter]);
    assert_true(valid && "Piggyback list is valid");
  }
  return valid;
}

bool qtp_validate_perfctr(qtperfctr_t* ctr){
  assert_true(ctr != NULL);
  assert_true(ctr->num_contributors >= 1);
  assert_true(ctr->state_group != NULL);
  assert_true(ctr->num_states > 0 && ctr->num_states != QTPERF_INVALID_STATE);
  assert_true(ctr->data != NULL);
  return 1;
}

bool qtp_validate_perfdata(qtperfdata_t* data){
  assert_true(data != NULL);
  assert_true(data->counters != NULL);
  assert_true(data->ctr_owner || data->counters->num_contributors > 1);
  qtp_validate_perfctr(data->counters);
  qtp_validate_piggybacks(data->piggybacks, data->counters->num_states);
  qtlogargs(PERFDBG,"data->current_state=%lu, data->time_entered=%lu", data->current_state, data->time_entered);
  assert_true(((data->current_state == QTPERF_INVALID_STATE && data->time_entered==0)
               || (data->current_state != QTPERF_INVALID_STATE && data->time_entered > 0)));
  return 1;
}

bool qtp_validate_perf_list(qtperf_perf_list_t* counters,
                            qtperf_perf_list_t** next_counter,
                            size_t num_counters){
  qtperf_perf_list_t* current = counters;
  bool valid=1;
  size_t found_counters=0;
  if(current == NULL){
    assert_true(next_counter == &counters || next_counter == NULL);
    valid = valid && (next_counter == &counters || next_counter == NULL);
  }
  while(current != NULL) {
    found_counters++;
    valid = valid && qtp_validate_perfdata(&current->performance_data);
    if(current->next == NULL){
      assert_true(next_counter == &current->next);
      valid = valid && next_counter == &current->next;
    }
    current = current->next;
  }
  assert_true(found_counters == num_counters);
  valid = valid && (found_counters == num_counters);
  return valid;
}

bool qtp_validate_state_group(qtstategroup_t* group){
  bool valid = group->num_states > 0;
  bool valid_names = 0;
  bool valid_counters = 0;
  valid_names = qtp_validate_names((const char**)group->state_names, group->num_states);
  valid_counters = qtp_validate_perf_list(group->counters, group->next_counter, group->num_counters);
  assert_true(valid_counters);
  assert_true(group->num_states > 0);
  valid = valid && ((group->state_names == NULL) || valid_names);
  assert_true((group->state_names == NULL) || valid_names);
  return valid;
}

bool qtp_validate_group_list(void){
  qtperf_group_list_t* current = _groups;
  size_t found_groups=0;
  bool valid=1;
  if(current == NULL){
    assert_true(_next_group == &_groups || _next_group == NULL);
    valid = valid && (_next_group==&_groups || _next_group == NULL);
  }
  while(current != NULL){
    found_groups++;
    valid = valid & qtp_validate_state_group(&current->group);
    if(current->next == NULL){
      assert_true(_next_group == &current->next);
      valid = valid && _next_group== &current->next;
    }
    current = current->next;
  }
  assert_true(found_groups == _num_groups);
  valid = valid && (found_groups == _num_groups);
  return valid;
}

bool qtperf_check_invariants(void) {
  assert_true(_collecting == 0 || _collecting == 1);
  return (_collecting == 0 || _collecting == 1) && qtp_validate_group_list();
}

#endif

#endif // ifdef QTHREAD_PERFORMANCE
