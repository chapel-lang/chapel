//
// If a subclass overloads a non-throwing method of the base class as throws,
// the compiler should emit an error.
//

class A {
  var x: int = 0;

  proc writeThis(ch: channel) {
    ch <~> x;
  }
}

class A1 : A {
  var y: int = 0;

  proc writeThis(ch: channel) throws {
    ch.write(y);
  }
}


