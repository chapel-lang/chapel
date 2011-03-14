var x: int;

proc test: int {
  x += 1;
  return x;
}

proc main() {
  var s, t: int;
  atomic on 1 do t = test;
  s = t;
  writeln(s);
}