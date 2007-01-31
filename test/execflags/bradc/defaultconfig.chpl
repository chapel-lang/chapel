
config const m = foo();

writeln("Value of m is: ", m);

def foo() {
  writeln("Did I get called?");
  return true;
}
