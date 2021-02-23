#include<stdarg.h>
#include<stddef.h>
#include<setjmp.h>
#include<string.h>
#include<cmocka.h>
#include<qthread/qthread.h>
#include<qthread/performance.h>
#include<qthread/logging.h>


// Test the spin_lock function to make sure it prevents simultaneous
// edits to a data structure. this function is copied from
// performance.c in order to test it as written. It's lame, but
// simple.
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
  qtperf_enter_state(mydata, SPIN_WORKING);
  for(i=0; i<100; i++){
    size_t j=0;
    qtperf_enter_state(mydata, SPIN_WAITING);
    interruptions += spin_lock(&busy);
    qtperf_enter_state(mydata, SPIN_WORKING);
    start = strct;
    for(j=0; j<100000; j++){
      strct = strct+1;
    }
    assert_true(strct == start+100000);
    start = strct;
    busy = 0;
  }
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

static void test_spinlock(void** state) {
  aligned_t ret=0;
  size_t i=0;
  qtstategroup_t* spingroup=NULL;
  qtstategroup_t* totalgroup=NULL;
  qtperfdata_t* totaldata=NULL;
  qtperf_start();
  qthread_initialize();
  spingroup=qtperf_create_state_group(SPIN_NUM_STATES, "Spin Testing", spin_names);
  totalgroup=qtperf_create_state_group(TOTAL_NUM_STATES, "Total Time", total_names);
  totaldata=qtperf_create_perfdata(totalgroup);
  qtperf_enter_state(totaldata,RUNNING);
  for(i=0; i<10; i++){
    qtperfdata_t* spindata = qtperf_create_perfdata(spingroup);
    qthread_fork(struct_edit, (void*)spindata, &ret);
  }
  for(i=0; i<10; i++){
    qthread_readFE(NULL,&ret);
  }
  qtperf_enter_state(totaldata,DONE);
  qtperf_stop();
  assert_true(qtperf_check_invariants());
  qtperf_print_results();
  qtperf_free_data();
  assert_true(qtperf_check_invariants());
}


int main(int argc, char** argv){
  const struct CMUnitTest test[] ={
    cmocka_unit_test(test_spinlock)
  };
  return cmocka_run_group_tests(test,NULL,NULL);
}
