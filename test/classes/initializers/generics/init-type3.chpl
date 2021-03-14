// This test is a copy of
// test/classes/initializers/generics/init-type.chpl, to allow me
// to verify the individual type case worked appropriately (instead of making
// progress gated on all generic cases working)

// This test failed in a previous version of the initializers implementation.

class A {
  type t = int;
  var x:t;
  proc init(xVal: int) {
    // I removed the argument and explicit setting of field t, to show that
    // omitted type fields work just fine.
    this.x = xVal;
  }
}

var a = new unmanaged A(2);

writeln(a);

delete a;
