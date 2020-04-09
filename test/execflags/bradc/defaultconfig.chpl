
config const m = foo();

writeln("Value of m is: ", m);

proc foo() {
  writeln("Did I get called?");
  return true;
}
