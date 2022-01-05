module C {
  proc test() {
    use D;
    writeln("In module C");
    D.foo();
  }
}
