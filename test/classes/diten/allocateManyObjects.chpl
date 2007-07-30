class letterclass {
  var x: int;
  var y: int;
  var z: int;
  def total () {
    return x + y + z;
  }
}
config const iterations = 10000000;
def main() {
  var cc: letterclass();
  var n = 0;
  var i = 0;

  while i < iterations {
    cc = letterclass(i, i+1);
    n += cc.total();
    i += 1;
  }
  writeln(n);
}
