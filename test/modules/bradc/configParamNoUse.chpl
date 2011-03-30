module M1 {
  config param debug: bool = false;

  proc foo() {
    if debug then
      writeln("Debug on!");
    writeln("In M1.foo()");
  }
}

module M2 {
  proc main() {
    M1.foo();
  }
}
