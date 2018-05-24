class C {
  var x = 10;
}

var globc: C;

class D {
  var y = 20;
  var locc: C;

  proc doit {
    locc = globc;
  }
}

proc main() {
  var d: D = new borrowed D();

  d.doit;
  writeln(d);
}

