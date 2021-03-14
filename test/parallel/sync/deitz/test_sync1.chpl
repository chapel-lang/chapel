proc fibonacci(n: int): int {
  if n == 1 {
    return 1;
  } else if n == 2 {
    return 1;
  } else {
    var s: sync int;
    begin { s.writeEF(fibonacci(n-1)); }
    return fibonacci(n-2) + s.readFE();
  }
}

writeln(fibonacci(8));
