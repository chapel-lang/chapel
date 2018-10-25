export proc foo() {
  writeln("In foo()");
}

proc main() {
  writeln("In Chapel main()");
  foo();
}
