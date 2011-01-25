proc fibonacci(n: int): int {
  if n == 1 {
    return 1;
  } else if n == 2 {
    return 1;
  } else {
    var s: sync int;
    begin { s = fibonacci(n-1); }
    return fibonacci(n-2) + s;
  }
}

writeln(fibonacci(8));
