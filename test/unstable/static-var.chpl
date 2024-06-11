proc test() {
  @functionStatic
  ref theVar = 42;

  writeln("Variable: ", theVar);
  theVar += 1;
}

test();
test();
test();
