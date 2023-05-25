class C {
  proc f() throws {
    throw Error();
  }
}

var c = (new owned C()).borrow();

c.f();
