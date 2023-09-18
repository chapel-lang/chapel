record R { var x: int; }

var x: R(); // bad
var y: R; // ok

var t: string(); // bad
var u: string; // ok

type tt1 = R(); // bad
type tt2 = R; // ok

proc foo(type t) { }
foo(R()); // bad
foo(R); // ok

class C { var x: int; }
foo(C()); // bad
foo(C); // ok
foo(borrowed (C())); // bad
foo((borrowed C)()); // bad
foo(borrowed C); // ok

var uc1: unmanaged C()?; // bad
var uc2: unmanaged C?; // ok

var oc1: owned C() = new C(); // bad
var oc2: owned C = new C(); // ok
