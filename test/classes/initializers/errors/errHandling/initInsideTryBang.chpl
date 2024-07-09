
record Signal {
  var x : int;

  proc init(x: int = 0) {
    writeln("Signal.init");
    this.x = x;
  }

  proc init=(other: Signal) {
    writeln("Signal.init=");
    this.x = other.x;
  }

  operator =(ref lhs: Signal, const rhs: Signal) {
    writeln("Signal.=");
    lhs.x = rhs.x;
  }
}

proc makeSignal(x: int) throws {
  if x < 0 then throw new Error("negative int!");

  return new Signal(x);
}

//
// This test exists to ensure that initializer normalization occurs within
// try! statements. Without such normalization, we'd see an assignment into
// an uninitialized 'sig' field, then see the field initialized such that
// 'x == 0'.
//
record Wrapper {
  var sig : Signal;

  proc init() throws {
    try! {
      this.sig = makeSignal(999);
    }
  }
}

proc main() {
  var w : Wrapper;
  writeln(w);
}
