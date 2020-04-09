// Would like to allow index variable to be modifiable,
// e.g. using the 'var' keyword.

iter myIter() {
  yield 555;
}

proc main() {
  for var myVar in myIter() {
    writeln("before: ", myVar);
    myVar += 333;
    writeln("after: ", 888);
  }
}
