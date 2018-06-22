class C {
  proc f() throws {
    throw Error();
  }
}

var c = new borrowed C();

c.f();
