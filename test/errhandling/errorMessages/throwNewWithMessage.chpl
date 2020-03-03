class C {
  proc f() throws {
    throw new owned
      Error("This is an error message.");
  }
}

var c = new C();

c.f();
