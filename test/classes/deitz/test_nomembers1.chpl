class addition {
  fun identity(x : int) : int {
    return x + 2;
  }
}

var a : addition = addition();

writeln(a.identity(2));
