config const n = 1;

proc main() {
  var D = {1..3};

  foo(D);
}

proc foo(inout D: domain(1)) {

  forall i in 1..n { // task intent required?
    D = {2..4}; // race if n > 1
  }
}
