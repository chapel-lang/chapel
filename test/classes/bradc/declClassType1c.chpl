class C {
  var x = 10;
}

var globc: borrowed C? = nil;

class D {
  var y = 20;
  var locc: borrowed C?;

  proc doit {
    locc = globc;
  }
}

proc main() {
  var d: borrowed D = new borrowed D();

  d.doit;
  writeln(d);
}
