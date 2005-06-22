function f() {
  var s : seq of integer;
  var i : integer;
  while i < 10 {
    s._append(i*4);
    i += 1;
  }
  return s;
}

writeln(f());

