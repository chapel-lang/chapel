record R1 {
  var x;
}

record R2 {
  var y;
}

var r1 = new R1(2);
var r2 = new R2(r1);

writeln(r2);
