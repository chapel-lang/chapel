class pair {
  var x: int;
  var y: real;
}

def main() {
  var a: pair;
  a = new pair();
  a.x = 10;
  a.y = 2.3;
  writeln("a is: ", a.x, " ", a.y);
}
