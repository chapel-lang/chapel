class addition {
  function identity(x : integer) : integer {
    return x + 2;
  }
}

var a : addition = addition();

writeln(a.identity(2));
