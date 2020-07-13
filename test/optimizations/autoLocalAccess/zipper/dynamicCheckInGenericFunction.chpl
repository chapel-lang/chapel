use common;

var dom = createDom({1..10});

var arr: [dom] real;

class MyClass {
  proc this(i) {
    return i*2;
  }
}

proc foo(A: arr.type, B) {
  // A is a static candidate that should be statically confirmed,
  // B is a dynamic candidate that should be statically reverted
  forall (i, loopIdx) in zip(A.domain, 1..) {
    A[i] = B[i]*loopIdx;
  }
}

foo(arr, new MyClass());
