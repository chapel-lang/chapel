class C {
  var x = 10;
}

var globc: C = new C();

class D {
  var y = 20;
  var locc: C;

  def doit {
    locc = globc;
  }
}

def main() {
  var d: D = new D();
  d.doit;
  writeln(d);
}
