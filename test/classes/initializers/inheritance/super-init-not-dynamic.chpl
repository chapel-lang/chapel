//
// Calls to super.<method>() should not be dynamically dispatched.
//
// This is essential to ensure that the implicit call to super.init()
// in Derived.init() will directly invoke Base.init().
//
// There was a bug in insertDynamicDispatchCalls() that triggered an
// internal error during inlining for this case.  This test locks in
// the fix for this.
//

class Base {
  var b = 10;

  proc init() {
    writeln('Base.init');
  }
}

class Derived : Base {
  var d = 20;

  // The implied call to super.init() must be to Base.init()
  // Dynamic dispatching would cause an infinite loop
  proc init() {
    writeln('Derived.init');
  }
}

var b = new owned Base();

writeln();

var d = new owned Derived();

writeln();
writeln(b);
writeln(d);
