// This code is from #11330.

class A {
  var data: real;
}
class B {
  param n: int;
  var data: real;
}

// Straightforward borrow.
var owned_a: owned A = new owned A();
var borr_a: borrowed A = owned_a.borrow();

var owned_b: owned B(42) = new owned B(42);
var borr_b: borrowed B(42) = owned_b.borrow();

// Redundant borrows.
var a = borr_a.borrow();
var b = borr_b.borrow();

if a.data == b.data then writeln("Equal");
