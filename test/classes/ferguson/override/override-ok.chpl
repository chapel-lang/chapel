class Parent {
  proc f() {
    writeln("Parent.f()");
  }
}

class Child : Parent {
  override proc f() {
    writeln("Child.f()");
  }
}

proc test() {
  var x = new owned Child();
  var y = x.borrow():Parent;
  y.f();
}
test();
