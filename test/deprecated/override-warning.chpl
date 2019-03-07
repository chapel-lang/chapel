// This test is to remind us to convert
// the warning about missing override keyword
// into an error.
// See issue #11069.

class Parent {
  proc f() { }
}

class Child : Parent {
  proc f() { }
}

var x = new borrowed Child();
x.f();
