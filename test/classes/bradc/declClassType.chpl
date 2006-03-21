class C {
  var x = 10;
}

var globc: C;

class D {
  var y = 20;
  var locc: C;

  fun doit {
    locc = globc;
  }
}

fun main() {
  var d: D = D();
  d.doit;
  writeln(d);
}

