class C {
  proc +(x) {writeln("In +, x is ", x);}
}

var c: borrowed C = new borrowed C();
