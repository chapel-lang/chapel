
record B {
  proc foo() throws {
    throw new owned Error();
  }
}

record A {
  var down : B;
  forwarding down;
}

proc main() {
  try {
    var a = new A();
    a.foo();
  } catch e : Error {
    writeln("Got error from proc.");
  }
}
