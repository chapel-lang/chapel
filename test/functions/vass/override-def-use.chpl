// Issue #21073

config const flag = false;

class parent {
  proc plusone(ref n: int): void
  {
    n = n + 1;
  }
  proc plusthree(ref n: int): void
  {
    n = n + 3;
  }
}

class child : parent {
  override proc plusone(ref n: int): void
  {
    n = n + 2;
  }
  override proc plusthree(ref n: int): void
  {
    n = n + 4;
  }
}

proc main(): int
{
  var c: parent = if flag then new parent() else new child();
  var v: [1..5] int = 1;

  c.plusone(v[3]);
  writeln("first try:  ", v);

  ref k: int = v[3];
  c.plusone(k);
  writeln("second try: ", v);

  var c3: parent = if !flag then new parent() else new child();
  c3.plusthree(v[3]);
  writeln("third try:  ", v);

  return 0;
}
