config const setting = false;

proc main() {
  const ref a;

  if setting {
    a = 9.0;
  } else {
    a = 100;
  }
  writeln(a);
}
