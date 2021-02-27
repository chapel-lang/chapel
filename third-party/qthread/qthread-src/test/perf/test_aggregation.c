#include<stdarg.h>
#include<stddef.h>
#include<setjmp.h>
#include<string.h>
#include<cmocka.h>
#include<qthread/qthread.h>
#include<qthread/performance.h>
#include<qthread/logging.h>

typedef enum {
  FEB_WORKING,
  FEB_WAITING,
  FEB_COMPLETE,
  FEB_NUM_STATES
} febstate_t;

const char* feb_names[] = {
  "FEB_WORKING",
  "FEB_WAITING",
  "FEB_COMPLETE",
  "FEB_NUM_STATES"
};
aligned_t available=0;
int strct=0;
aligned_t bit=0;
#define SPIN 1
#define NITER 100000
#define NTHREAD 20
aligned_t struct_edit(void*data){
  size_t i=0;
  aligned_t ret=0;
  qtperfdata_t* mydata = (qtperfdata_t*)data;
  qtperf_enter_state(mydata, FEB_WORKING);
  for(i=0; i<100; i++){
    size_t j=0;
    qtperf_enter_state(mydata, FEB_WAITING);
    qthread_readFE(NULL, &available);
    qtperf_enter_state(mydata, FEB_WORKING);
    for(j=0; j<NITER; j++){
      strct = strct*(strct+1);
    }
    qthread_writeEF(&available, &ret);
  }
  qtperf_enter_state(mydata, QTPERF_INVALID_STATE);
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

static void test_feblock(void** state) {
  aligned_t ret=0;
  size_t i=0;
  qtperfctr_t* agg_target = NULL;
  qtstategroup_t* spingroup=NULL;
  qtstategroup_t* totalgroup=NULL;
  qtperfdata_t* totaldata=NULL;
  qtperfdata_t* all_data[NTHREAD];
  qtperf_start();
  qtperf_set_instrument_qthreads(1);
  qthread_initialize();
  qthread_fill(&available);
  qthread_empty(&ret);
  spingroup=qtperf_create_state_group(FEB_NUM_STATES, "Spin Testing", feb_names);
  totalgroup=qtperf_create_state_group(TOTAL_NUM_STATES, "Total Time", total_names);
  totaldata=qtperf_create_perfdata(totalgroup);
  qtperf_enter_state(totaldata,RUNNING);
  for(i=0; i<NTHREAD; i++){
    qtperfdata_t* spindata = qtperf_create_aggregated_perfdata(spingroup, agg_target);
    all_data[i]=spindata;
    if(agg_target == NULL) {
      agg_target = spindata->counters;
    }
    assert_true(qtperf_check_invariants());
    qthread_fork(struct_edit, (void*)spindata, &ret);
  }
  for(i=1; i<NTHREAD; i++){
    // These are all aggregated, so they should all push data to the
    // same logging struct
    assert_true(all_data[0]->counters == all_data[i]->counters);
    assert_true(all_data[0] != all_data[i]);
  }
  for(i=0; i<NTHREAD; i++){
    qthread_readFE(NULL,&ret);
  }
  qtperf_enter_state(totaldata,DONE);
  qtperf_stop();
  assert_true(qtperf_check_invariants());
  qtperf_print_delimited(qtperf_qthreads_group, ",", 1, "= ");
  qtperf_print_delimited(spingroup, ",", 1, "+ ");
  qtperf_free_data();
  assert_true(qtperf_check_invariants());
}


int main(int argc, char** argv){
  const struct CMUnitTest test[] ={
    cmocka_unit_test(test_feblock)
  };
  return cmocka_run_group_tests(test,NULL,NULL);
}
