deprecated "Record C is deprecated, please use D"
record C {
  var x: int;

  proc foo() {
    writeln("In C.foo()");
  }
}

record D {
  var x: int;

  proc foo() {
    writeln("In D.foo()");
  }
}

proc main() {
  var c1 = new C(1);                // Should trigger the warning
  var c2: C = new C(2);             // Should trigger the warning
  var c3: C;

  writeln(c1);   // Shouldn't trigger the warning
  writeln(c2.x); // Shouldn't trigger the warning
  c3.foo();      // Shouldn't trigger the warning
}
