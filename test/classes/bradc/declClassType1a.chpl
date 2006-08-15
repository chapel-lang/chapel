class C {
  var x = 10;
}

var globc: C = C();

class D {
  var y = 20;
  var locc: C;

  def doit {
    locc = globc;
  }
}

def main() {
  var d: D = D();
  d.doit;
  writeln(d);
}
