#include<stdarg.h>
#include<stddef.h>
#include<setjmp.h>
#include<string.h>
#include<cmocka.h>
#include<qthread/performance.h>
#include<qthread/logging.h>

typedef enum {
  S1,S2,S3,S4,S5,NumStates1
} state1_t;

const char* state1_names[] = {
    "S1","S2","S3","S4","S5"
};

typedef enum {
  S21,S22,S23,S24,S25,NumStates2
} state2_t;

const char* state2_names[] = {
    "S21","S22","S23","S24","S25"
};

typedef enum {
  S31,S32,S33,S34,S35,NumStates3
} state3_t;

const char* state3_names[] = {
    "S31","S32","S33","S34","S35"
};


qtstategroup_t* group1=NULL;
qtstategroup_t* group2=NULL;
qtstategroup_t* group3=NULL;

#ifndef DBG_RESET
#define DBG_RESET 0
#endif
static int reset_qtperf(void** state){
  qtlog(DBG_RESET, "Freeing qtperf data");
  qtperf_free_data();
  group1 = NULL;
  group2 = NULL;
  group3 = NULL;
  return 0;
}

#ifndef DBG_GRP
#define DBG_GRP 0
#endif
static void test_create_group(void** state){
  group1 = qtperf_create_state_group(NumStates1, "group 1", state1_names);
  assert_true(group1 != NULL);
  assert_true(qtperf_check_invariants());
}

#ifndef DBG_PERF
#define DBG_PERF 0
#endif
static void test_create_perfdata(void**state) {
  qtperfdata_t* data1 = NULL;
  qtperfdata_t* data2 = NULL;
  qtperfdata_t* data3 = NULL;
  group1 = qtperf_create_state_group(NumStates1, "group 1", state1_names);
  assert_true(group1 != NULL);
  group2 = qtperf_create_state_group(NumStates2, "group 2", state2_names);
  assert_true(group2 != NULL);
  group3 = qtperf_create_state_group(NumStates3, NULL, NULL);
  assert_true(group3 != NULL);
  data1 = qtperf_create_perfdata(group1);
  assert_true(data1 != NULL);
  data2 = qtperf_create_perfdata(group2);
  assert_true(data2 != NULL);
  data3 = qtperf_create_perfdata(group3);
  assert_true(data3 != NULL);
  assert_true(qtperf_check_invariants());
  qtperf_free_data();
  assert_true(qtperf_check_invariants() && "after cleanup");
}


size_t spin(size_t amount){
  size_t i=0;
  size_t r=2;
  for(i=0; i<amount; i++){
    size_t j=0;
    for(j=0; j<100; j++){
      r = r * r+1;
    }
  }
  return r;
}

#ifndef DBG_STATES
#define DBG_STATES 1
#endif
void checked_transition(qtperfdata_t* data, qtperfid_t newstate){
  qtperfid_t last_state = data->current_state;
  qttimestamp_t entered = data->time_entered;
  qtperfcounter_t before = 0;//
  if(data->current_state != QTPERF_INVALID_STATE){
    before = data->counters->data[data->current_state];
  }
  qtperf_enter_state(data, newstate);
  assert_true(data->current_state == newstate);
  if(last_state != QTPERF_INVALID_STATE){
    bool valid_timing = data->counters->data[last_state] == (data->time_entered - entered) + before;
    if(!valid_timing){
      qtlog(LOGERR, "Invalid timing update");
      qtlogargs(LOGERR, "\tstates: %lu -> %lu", last_state, data->current_state);
      qtlogargs(LOGERR, "\tentered: %lu", entered);
      qtlogargs(LOGERR, "\tleft: %lu", data->time_entered);
      qtlogargs(LOGERR, "\tcounter before: %llu", before);
      qtlogargs(LOGERR, "\tcounter after: %llu", data->counters->data[last_state]);
      qtlogargs(LOGERR, "\texpected difference: %lu", data->time_entered-entered);
      qtlogargs(LOGERR, "\tactual difference: %llu", data->counters->data[last_state]-before);
    }
    assert_true(valid_timing);
  }
}
static void test_state_transitions(void** state){
  qtperfdata_t* data = NULL;
  size_t i=0;
  size_t spinval=0;
  group1 = qtperf_create_state_group(NumStates1, "group 1", state1_names);
  assert_true(group1 != NULL);
  data = qtperf_create_perfdata(group1);
  assert_true(data != NULL);
  qtperf_start();
  assert_true(data->current_state == QTPERF_INVALID_STATE);
  checked_transition(data, S1);
  assert_true(data->current_state == S1);
  spin(100);
  for(i=0; i<1000; i++){
    if(i%2 == 0){
      checked_transition(data, S2);
      spinval+=spin(1000);
      assert_true(data->current_state == S2);
    }
    if(i%2 == 1){
      checked_transition(data, S3);
      spinval+=spin(500);
      assert_true(data->current_state == S3);
    }
    checked_transition(data, S4);
    assert_true(data->current_state == S4);
    spinval+=spin(1000);
  }
  qtlogargs(DBG_STATES, "time[S1] = %llu", data->counters->data[S1]);
  qtlogargs(DBG_STATES, "time[S2] = %llu", data->counters->data[S2]);
  qtlogargs(DBG_STATES, "time[S3] = %llu", data->counters->data[S3]);
  qtlogargs(DBG_STATES, "time[S4] = %llu", data->counters->data[S4]);
  printf("print number to avoid optimizing out spin function: %lu\n", spinval);
  
  assert_true(data->counters->data[S2] > data->counters->data[S3]);
  assert_true(data->counters->data[S3] > data->counters->data[S1]);
  assert_true(data->counters->data[S4] > data->counters->data[S3]);
  qtperf_enter_state(data, S5);
  qtperf_stop();
  qtperf_free_data();
  assert_true(qtperf_check_invariants() && "state transistions done");
}

static const size_t NumThreads=100;
static void test_fake_concurrent(void** state){
  qtperfdata_t* threads[NumThreads];
  qtstategroup_t* group[3];
  qtperfid_t numstates[3];
  size_t i=0;
  group[0]=qtperf_create_state_group(NumStates1, "group 1", state1_names);
  group[1]=qtperf_create_state_group(NumStates2, "group 2", state2_names);
  group[2]=qtperf_create_state_group(NumStates3, "group 3", state3_names);
  numstates[0] = NumStates1;
  numstates[1] = NumStates2;
  numstates[2] = NumStates3;
  assert_true(group[0] != NULL && group[1] != NULL && group[2] != NULL);
  qtperf_start();
  for(i=0; i<NumThreads; i++){
    threads[i] = qtperf_create_perfdata(group[i%3]);
  }
  for(i=0; i<NumThreads*20; i++){
    checked_transition(threads[i%NumThreads], qtperf_now()%numstates[i%3]);
    spin(500);
  }
  for(i=0; i<NumThreads; i++){
    qtperfcounter_t total_time=0;
    size_t j=0;
    for(j=0; j<numstates[i%3]; j++){
      total_time += threads[i]->counters->data[j];
    }
    assert_true(total_time > 0);
  }
  qtperf_stop();
  assert_true(qtperf_check_invariants());
  qtperf_free_data();
  assert_true(qtperf_check_invariants());
}

#ifndef DBG_ITER
#define DBG_ITER 0
#endif
static void test_iterator(void** state){
  qtperfdata_t* threads[NumThreads];
  qtstategroup_t* group[3];
  qtperfid_t numstates[3];
  qtperf_iterator_t iterator;
  qtperf_iterator_t* iter=&iterator;
  qtperfdata_t* iterdata = NULL;
  size_t i=0;
  group[0]=qtperf_create_state_group(NumStates1, "group 1", state1_names);
  group[1]=qtperf_create_state_group(NumStates2, "group 2", state2_names);
  group[2]=qtperf_create_state_group(NumStates3, "group 3", state3_names);
  numstates[0] = NumStates1;
  numstates[1] = NumStates2;
  numstates[2] = NumStates3;
  assert_true(group[0] != NULL && group[1] != NULL && group[2] != NULL);
  qtperf_start();
  for(i=0; i<NumThreads; i++){
    threads[i] = qtperf_create_perfdata(group[i%3]);
  }
  for(i=0; i<NumThreads*20; i++){
    checked_transition(threads[i%NumThreads], qtperf_now()%numstates[i%3]);
  }
  qtperf_stop();
  qtperf_iter_begin(&iter);
  assert_true(iter != NULL);
  assert_true(qtperf_iter_deref(iter) != NULL);
  i=0;
  for(iterdata = qtperf_iter_next(&iter);
      iterdata != NULL;
      iterdata = qtperf_iter_next(&iter)){
    qtlogargs(DBG_ITER, "iter step %lu = %p", i, (void*)iter);
    i++;
  }
  assert_true(iter == NULL);
  qtlogargs(DBG_ITER, "i=%lu, NumThreads=%lu", i, NumThreads-1);
  assert_true(i == NumThreads-1);
  assert_true(qtperf_check_invariants());
  qtperf_free_data();
  assert_true(qtperf_check_invariants());
}

static void test_print(void** state){
  qtperfdata_t* threads[5];
  qtstategroup_t* group[3];
  qtperfid_t numstates[3];
  size_t i=0;
  group[0]=qtperf_create_state_group(NumStates1, "group 1", state1_names);
  group[1]=qtperf_create_state_group(NumStates2, "group 2", state2_names);
  group[2]=qtperf_create_state_group(NumStates3, "group 3", state3_names);
  numstates[0] = NumStates1;
  numstates[1] = NumStates2;
  numstates[2] = NumStates3;
  assert_true(group[0] != NULL && group[1] != NULL && group[2] != NULL);
  qtperf_start();
  for(i=0; i<5; i++){
    threads[i] = qtperf_create_perfdata(group[i%3]);
  }
  for(i=0; i<5*20; i++){
    checked_transition(threads[i%5], qtperf_now()%numstates[i%3]);
    spin(500);
  }
  for(i=0; i<5; i++){
    qtperfcounter_t total_time=0;
    size_t j=0;
    for(j=0; j<numstates[i%3]; j++){
      total_time += threads[i]->counters->data[j];
    }
    assert_true(total_time > 0);
  }
  qtperf_stop();
  qtperf_print_results();
  assert_true(qtperf_check_invariants());
  qtperf_free_data();
  assert_true(qtperf_check_invariants());
}

int main(int argc, char** argv){
  const struct CMUnitTest test[] ={
    cmocka_unit_test_teardown(test_create_group,reset_qtperf),
    cmocka_unit_test_teardown(test_state_transitions,reset_qtperf),
    cmocka_unit_test_teardown(test_create_perfdata,reset_qtperf),
    cmocka_unit_test_teardown(test_fake_concurrent,reset_qtperf),
    cmocka_unit_test_teardown(test_iterator,reset_qtperf),
    cmocka_unit_test_teardown(test_print,reset_qtperf)
  };
  return cmocka_run_group_tests(test,NULL,NULL);
}
