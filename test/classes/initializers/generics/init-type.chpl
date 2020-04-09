// This test is a copy of
// test/classes/initializers/generics/uninit-type.chpl, to allow me
// to verify the individual type case worked appropriately (instead of making
// progress gated on all generic cases working)

class A {
  type t = int; // This line differs, to show what happens with a default value
  var x:t;
  proc init(type t) {
    this.t = t;
  }
  // initializer for class 'A' requires a generic argument called 't'
}

var a = new unmanaged A(int);

writeln(a);

delete a;
