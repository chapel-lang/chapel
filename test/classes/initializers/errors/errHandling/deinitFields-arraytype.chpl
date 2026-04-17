class C {
  type t = [1..3] real;

  proc helpThrow() throws {
    throw new Error();
  }
  
  proc init() throws {
    init this;
    helpThrow();
  }
}

class D {
  type t = [1..3] real;

  proc postinit() throws {
    throw new Error();
  }
}

try {
  var myC = new C();
} catch {
  writeln("Caught an error, as expected");
}

try {
  var myD = new D();
} catch {
  writeln("Caught an error, as expected");
}
