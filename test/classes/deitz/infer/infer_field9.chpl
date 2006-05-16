record R1 {
  var x;
}

record R2 {
  var y;
}

var r1 = R1(2);
var r2 = R2(r1);

writeln(r2);
