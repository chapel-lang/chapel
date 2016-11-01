class MethodTooEarly {
  var i: int;

  proc init(iVal: int) {
    myMethod(); // Uh oh!
    i = iVal;
    super.init();
  }

  // This method demonstrates why it would be bad to access a method before the
  // instance had finished Phase 1 of initialization
  proc myMethod() {
    writeln("My i value is: ", i);
  }
}

proc main() {
  var c: MethodTooEarly = new MethodTooEarly(3);

  delete c;
}
