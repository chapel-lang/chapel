module M {
  proc bar() {
    writeln("Computing bar()");
  }
  module N {
    proc foo() {
      writeln("Hijacked!");
    }
  }
}

module N {
  proc foo() {
    writeln("Computing foo()");
  } 
}

module Main {
  proc main() {
    use M;
    use N;
    foo();  // this now calls the hijacked version
    bar();
  }
}
