record C {
  param rank : int;
  var x;
  var y;
}

var c1 = new C(2, 12, 14);
var c2 = new C(2, 12.0, 14);
var tmp1 = c1.x;
var tmp2 = c2.x;

writeln(c1);
writeln(c2);
writeln(tmp1);
writeln(tmp2);
