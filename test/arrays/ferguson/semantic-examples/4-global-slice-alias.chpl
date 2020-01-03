module OuterModule {
  module bar {
    var dom = {1..10, 1..10};
    var A : [dom] int;

    ref R = A[1..4, 1..4]; // call_tmp = slice
    // slice freed here
  }

  use bar;

  proc main() {
    writeln("Setting A[1,1] to 1");
    A[1,1] = 1;
    writeln("R[1,1] is now     ", R[1,1]);

    for r in R do write(r);
    writeln();
  }
}
