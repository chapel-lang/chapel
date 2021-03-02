
#include<stdarg.h>
#include<stddef.h>
#include<setjmp.h>
#include<string.h>
#include<cmocka.h>
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

#define TEST 0
size_t num_threads=5;
void test_spinners(void** state){
  size_t i=0;
  aligned_t ret=0;
  qtperf_set_instrument_workers(1);
  qtperf_start();
  qthread_initialize();
  for(i=0; i<num_threads; i++){
    qthread_fork(spin, NULL,&ret);
  }
  for(i=0; i<num_threads; i++){
    qthread_readFE(NULL, &ret);
  }
  qtperf_stop();
  qtlog(TEST, "Printing results...");
  qtperf_print_results();
  qtlog(TEST, "done printing results");
  assert_true(qtperf_check_invariants());
}

void test_teardown(void** state){
  qtperf_free_data();
  assert_true(qtperf_check_invariants());
}

int main(int argc, char** argv){
  const struct CMUnitTest test[] ={
    cmocka_unit_test(test_spinners),
    cmocka_unit_test(test_teardown)
  };
  return cmocka_run_group_tests(test,NULL,NULL);
}
