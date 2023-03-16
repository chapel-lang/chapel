class C {
  var x = 10;
}

var globc: borrowed C?;

class D {
  var y = 20;
  var locc: borrowed C?;

  proc doit {
    locc = globc;
  }
}

proc main() {
  var ownD = new owned D();
  var d: borrowed D = ownD.borrow();

  d.doit;
  writeln(d);
}

