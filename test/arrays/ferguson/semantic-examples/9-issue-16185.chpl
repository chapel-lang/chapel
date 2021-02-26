proc run() {
  var A:[1..10] int;

  foo(A[1..3]); // accepts argument with `in` and sets element 1
  writeln("after foo, A[1] is ", A[1]); // is A[1] still 0?
}

proc foo(in A) {
  writeln("in foo, chpl__isArrayView(A) is ", chpl__isArrayView(A));
  writeln("in foo, A[1] is ", A[1]);
  writeln("in foo, setting A[1] = 1");
  A[1] = 1;
}

run();
