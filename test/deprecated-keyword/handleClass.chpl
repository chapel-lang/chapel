deprecated "Class C is deprecated, please use D"
class C {
  var x: int;

  proc foo() {
    writeln("In C.foo()");
  }
}

class D {
  var x: int;

  proc foo() {
    writeln("In D.foo()");
  }
}

proc main() {
  var c1 = new C(1);                // Should trigger the warning
  var c2: C = new C(2);             // Should trigger the warning
  var c3 = new owned C(3);          // Should trigger the warning
  var c4 = new shared C(4);         // Should trigger the warning
  var c5: borrowed C = c3.borrow(); // Should trigger the warning
  var c6 = new unmanaged C(6);      // Should trigger the warning

  writeln(c1);   // Shouldn't trigger the warning
  writeln(c2.x); // Shouldn't trigger the warning
  c3.foo();      // Shouldn't trigger the warning

  delete c6; // Shouldn't trigger the warning
}
