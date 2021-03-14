class C {
  proc foo(A: []) {
  }
}

proc bar(A: [], x: borrowed C? = nil) {
  forall a in A {
    if x then x!.foo(A);
  }
}

// The internal error this test is about goes away if
// the declaration below is uncommented.
//const c: C;

var A: [1..10] int;

bar(A);

