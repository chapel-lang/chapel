// bug report from issue #12187
// compiler was reporting
// error: control reaches end of function that returns a value

proc g(x: int): int {
  try {
    return 1;
  } catch e {
    exit(1);
  }
}

writeln(g(1));
