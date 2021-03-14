config const setting = false;

proc main() {
  param a;

  if setting {
    a = 9;
  } else {
    a = 100;
  }
  writeln(a);

  param b;
  if setting {
    b = 9: int(32);
  } else {
    b = 9;
  }
  writeln(b);
}
