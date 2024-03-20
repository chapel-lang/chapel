record Outer {
  record Inner : contextManager {
    var x: int;

    proc ref enterContext() ref {
      writeln("entering, x = ", x);
      return x;
    }
    proc exitContext(in err: owned Error?) {
      writeln("exiting, x = ", x);
    }
  }

  proc getInner() {
    return new Inner();
  }
}

var outer: Outer;
manage outer.getInner() as someResource {
  someResource = 42;
}
