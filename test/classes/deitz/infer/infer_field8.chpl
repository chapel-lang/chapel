record R {
  var x;
}

var r1 = new R(1);
var r2 = new R(2);
var r3 = new R(3.0);
var r4 = new R(4.0);

writeln(r1);
writeln(r2);
writeln(r3);
writeln(r4);

r1 = r2;
r3 = r4;

writeln(r1);
writeln(r2);
writeln(r3);
writeln(r4);
