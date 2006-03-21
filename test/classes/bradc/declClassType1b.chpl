class C {
  var x = 10;
}

var globc: C = C();

class D {
  var y = 20;
  var locc: C;

  fun doit {
    locc = globc;
  }
}

fun main() {
  var d: D = D();
  globc = nil;
  d.doit;
  writeln(d);
}
