#include<stdarg.h>
#include<stddef.h>
#include<setjmp.h>
#include<string.h>
#include<cmocka.h>
#include<qthread/qthread.h>
#include<qthread/performance.h>
#include<qthread/logging.h>

typedef enum{
  SS_RUNNING,
  SS_DONE,
  SS_NUM_STATES
} startstop_t;

const char* names[] = {
  "SS_RUNNING",
  "SS_DONE",
  "SS_NUM_STATES"
};

aligned_t spin(void* d){
  size_t i=0;
  qtperfdata_t* data = (qtperfdata_t*)d;
  aligned_t result=2;
  qtperf_enter_state(data, SS_RUNNING);
  for(i=0; i<1000000; i++){
    result = result * result + i;
  }
  qtperf_enter_state(data, QTPERF_INVALID_STATE);
  return result;
}

#define TEST 1
size_t num_threads=50;
void test_startstop(void** state){
  size_t i=0;
  aligned_t ret=0;
  qtstategroup_t* group=NULL;
  qtperfdata_t* data=NULL;
  qtperfcounter_t total_time=0;
  qttimestamp_t now=0;
  qtperf_set_instrument_workers(1);
  group = qtperf_create_state_group(SS_NUM_STATES, "start stop counter", names);
  qtperf_start();
  qthread_initialize();
  for(i=0; i<num_threads; i++){
    data = qtperf_create_perfdata(group);
    qthread_fork(spin, (void*)data,&ret);
  }
  for(i=0; i<num_threads; i++){
    qthread_readFE(NULL, &ret);
    if(i%3 == 0)
      qtperf_stop();
    if(i%4==0)
      qtperf_start();
  }
  qtperf_stop();
  qtperf_print_results();
  now = qtperf_now();
  total_time = qtperf_total_group_time(group);
  if(total_time >= now/2){
    qtlogargs(LOGERR, "Invalid total time: %llu", total_time);
  }
  assert_true(total_time < now/2);
  assert_true(qtperf_check_invariants());
  qtperf_free_data();
  assert_true(qtperf_check_invariants());
}

int main(int argc, char** argv){
  const struct CMUnitTest test[] ={
    cmocka_unit_test(test_startstop)
  };
  return cmocka_run_group_tests(test,NULL,NULL);
}
