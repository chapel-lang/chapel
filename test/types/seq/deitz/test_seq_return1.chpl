use List;

proc f() {
  var s : list(int);
  var i : int;
  while i < 10 {
    s.append(i*4);
    i += 1;
  }
  return s;
}

writeln(f());

