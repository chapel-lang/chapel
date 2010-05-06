var x: int;

def test: int {
  x += 1;
  return x;
}

def main() {
  var s, t: int;
  atomic on 1 do t = test;
  s = t;
  writeln(s);
}