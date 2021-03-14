proc foo() : int {
  var i : int;
  while i < 10 {
    return i * 4;
  }
  return;
}

writeln(foo());
