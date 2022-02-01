module D {
  require "subdir2/E.chpl";
  proc foo() {
    use E;
    writeln("In module D");
    E.bar();
  }
}
