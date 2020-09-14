// A wrapper module that contains the config vars and modules that invoke the
// various iters. Having the iterator uses in separate modules, places their
// generated code in separate files. This makes it easier to ensure the
// vectorization hint is placed on the correct loops

module ItersConfig {
  config const n: int = 10;
  config const ntasks: int = 2;
  var A: [0..#ntasks*n] int;
}

module InvokeLeaderFollower {
  use ItersConfig;
  use iters;
  forall (i,j) in zip(myiter(n, ntasks), myiter(n, ntasks)) {
    A[i] = j;
  }
}

module InvokeStandalone {
  use ItersConfig;
  use iters;
  forall i in myiter(n, ntasks) {
    A[i] += i;
  }
}

module InvokeSerial {
  use ItersConfig;
  use iters;
  for a in A do writeln(a);
}

module InvokeIters {
  use InvokeLeaderFollower;
  use InvokeStandalone;
  use InvokeSerial;
}

module main {
  proc main() {
    use InvokeIters;
  }
}
