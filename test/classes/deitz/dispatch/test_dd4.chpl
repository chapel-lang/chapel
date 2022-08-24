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

(new owned C()).borrow().goo();
(new owned D()).borrow().goo();
(new owned E()).borrow().goo();

(new owned D()).borrow().foo();
(new owned E()).borrow().foo();
