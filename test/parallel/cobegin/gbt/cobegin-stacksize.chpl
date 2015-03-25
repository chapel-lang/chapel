//
// This test is intended to show that CHPL_RT_CALL_STACK_SIZE properly sets the
// call stack size. Other tests check that the runtime reports the correct
// value, but this will show if the value is actually changed based on whether
// or not program execution was successful or not. To do this we run the
// program twice, once with a really small stack size, and once with a really
// large stack size to avoid having to think about what the stack size is for
// different configurations and worrying about it changing in the future.
//
config var depth: uint(64) = 1024;

var sum1$, sum2$: single uint(64);

proc recursive_sum(n: uint(64)): uint(64) {
  if n <= 1 then return n; else return n + recursive_sum(n - 1);
}

cobegin {
  sum2$ = recursive_sum(depth);
  sum1$ = recursive_sum(depth);
}

writeln("sums are ", sum1$.readFF(), ", ", sum2$.readFF());
