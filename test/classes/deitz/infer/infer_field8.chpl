record R {
  var x;
}

var r1 = R(1);
var r2 = R(2);
var r3 = R(3.0);
var r4 = R(4.0);

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
