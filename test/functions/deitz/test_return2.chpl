function foo() : integer {
  var i : integer;
  while i < 10 {
    return i * 4;
  }
  return;
}

writeln(foo());
