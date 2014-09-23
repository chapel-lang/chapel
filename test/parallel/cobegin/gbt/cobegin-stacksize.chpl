//
// The recursive_sum() function currently requires 64 bytes of stack
// per activation.  The default stack size on linux systems seems to
// be 8 mb, which should be enough for a depth of almost 128k ("almost"
// because the stack actually has to hold all the recursive_sum()
// activations plus a few others).  The default depth here is such
// that with the default stack limit, doing recursion to the given
// depth overruns the stack, typically causing a segfault.  But when
// the task call stack size is increased enough (to about 10 mb for
// tasking==none and ? mb for tasking==fifo), the program works.
// 
config var depth: uint(64) = 128 * 1024;

var sum1$, sum2$: single uint(64);

proc recursive_sum(n: uint(64)): uint(64) {
  if n <= 1 then return n; else return n + recursive_sum(n - 1);
}

cobegin {
  sum2$ = recursive_sum(depth);
  sum1$ = recursive_sum(depth);
}

writeln("sums are ", sum1$.readFF(), ", ", sum2$.readFF());
