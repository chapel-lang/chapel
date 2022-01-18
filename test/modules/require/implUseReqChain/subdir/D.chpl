module D {
  require "subdir2/E.chpl";
  proc foo() {
    use E;
    writeln("In module D in subdir");
    E.bar();
  }
}
