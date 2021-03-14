// This test was split off of
// test/classes/initializers/generic-uninit-field-initializers.chpl, to allow me
// to verify the individual type case worked appropriately (instead of making
// progress gated on all generic cases working)

record A {
  type t;
  var x:t;
  proc init(type t) {
    this.t = t;
  }
  // initializer for record 'A' requires a generic argument called 't'
}

var a: A(int);

writeln(a);
