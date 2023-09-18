
extern type pthread_t;
extern type pthread_attr_t;
extern proc pthread_create(thread:c_ptr(pthread_t),
                           const attr:c_ptr(pthread_attr_t),
                           start_routine:c_fn_ptr, arg:c_ptr(void)): c_int;
extern proc pthread_join(thread:pthread_t, retval:c_ptr(c_ptr(void))): c_int;

use UnorderedAtomics, CTypes;

config const numThreadsPerLocale = here.maxTaskPar;
config const numTrials = 10;

var a: atomic int;
proc addIt(p: c_ptr(void)): c_ptr(void) {
  a.unorderedAdd(1);
  unorderedAtomicTaskFence();
  return nil;
}

proc threadsAddIt(nthreads) {
  var pthread_arr = allocate(pthread_t, nthreads);
  for i in 0..#nthreads do
    pthread_create(c_ptrTo(pthread_arr[i]), nil:c_ptr(pthread_attr_t), c_ptrTo(addIt), nil);

  var res: c_ptr(void);
  for i in 0..#nthreads do
    pthread_join(pthread_arr[i], c_ptrTo(res));
  deallocate(pthread_arr);
}

coforall loc in Locales do on loc do
  for 1..numTrials do
    threadsAddIt(numThreadsPerLocale);

assert(a.read() == numLocales * numTrials * numThreadsPerLocale);
