@unstable("Class TestUnstableClass is unstable, use C2")
class TestUnstableClass {
  var x: int;

  proc foo() {
    writeln("In TestUnstableClass.foo()");
  }
}

class C2 {
  var x: int;

  proc foo() {
    writeln("In C2.foo()");
  }
}

proc main() {
  var c1 = new TestUnstableClass(1);                    // Should trigger the warning
  var c2: TestUnstableClass = new TestUnstableClass(2); // Should trigger the warning
  var c3 = new owned TestUnstableClass(3);              // Should trigger the warning
  var c4 = new shared TestUnstableClass(4);             // Should trigger the warning
  var c5: borrowed TestUnstableClass = c3.borrow();     // Should trigger the warning
  var c6 = new unmanaged TestUnstableClass(6);          // Should trigger the warning

  writeln(c1);   // Shouldn't trigger the warning
  writeln(c2.x); // Shouldn't trigger the warning
  c3.foo();      // Shouldn't trigger the warning

  delete c6; // Shouldn't trigger the warning
}
