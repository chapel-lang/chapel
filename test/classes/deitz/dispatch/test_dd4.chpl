class C {
  proc goo() {
    writeln("C.goo");
  }
}

class D : C {
  proc foo() {
    writeln("D.foo");
    goo();
  }
}

class E : D {
  override proc goo() {
    writeln("E.goo");
  }
}

((new owned C()).goo()).borrow();
((new owned D()).goo()).borrow();
((new owned E()).goo()).borrow();

((new owned D()).foo()).borrow();
((new owned E()).foo()).borrow();
