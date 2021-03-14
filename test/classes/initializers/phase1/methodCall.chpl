class MethodTooEarly {
  var i : int;

  proc init(iVal : int) {
    myMethod();   // *** Invalid.  Still in phase1
    myMethod(20); // *** Invalid.  Still in phase1

    super.init();

    i = iVal;

  }

  // This method demonstrates why it would be bad to access a method
  // before the instance had finished Phase 1 of initialization
  proc myMethod() {
    writeln("My i value is: ", i);
  }


  // This method demonstrates why it would be bad to access a method
  // before the instance had finished Phase 1 of initialization
  proc myMethod(x : int) {
    writeln("My i value is: ", i + x);
  }
}

proc main() {
  var c : unmanaged MethodTooEarly = new unmanaged MethodTooEarly(3);

  delete c;
}
