class C {
  var x = 10;
}

var globc: C = C();

class D {
  var y = 20;
  var locc: C;

  function doit {
    locc = globc;
  }
}

function main() {
  var d: D = D();
  d.doit;
  writeln(d);
}
