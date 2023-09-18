class C {
  proc f() throws {
    throw Error();
  }
}

var c = new C();

c.f();
