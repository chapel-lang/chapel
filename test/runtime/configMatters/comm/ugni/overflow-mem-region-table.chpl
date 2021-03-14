//
// This test (with the help of its .execenv) checks the error message
// printed by the comm=ugni memory allocator when we run out of memory
// region table entries.
//
config const arraySize = 2**26;  // > 2 * hugepage size, for separate alloc
config const arraysPerTask = 10;

coforall 1..here.maxTaskPar {  // per thread
  var x: [1..arraysPerTask] [1..arraySize] int(8);
}
