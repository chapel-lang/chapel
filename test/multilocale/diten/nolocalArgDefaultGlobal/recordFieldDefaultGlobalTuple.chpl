var t = (1, 2);
record R {
  var a = t(1);
}

var r: R();
t(1) = 2;
var s: R();

writeln(r);
writeln(s);
