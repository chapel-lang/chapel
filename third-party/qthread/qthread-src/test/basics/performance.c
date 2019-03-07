#include <qthread/qthread.h>
#include <qthread/performance.h>
#include <qthread/logging.h>

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