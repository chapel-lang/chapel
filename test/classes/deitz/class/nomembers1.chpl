class addition {
  proc identity(x : int) : int {
    return x + 2;
  }
}

var a : borrowed addition = new borrowed addition();

writeln(a.identity(2));
