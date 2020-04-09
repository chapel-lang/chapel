var t = (1, 2);
record R {
  var a = t(0);
}

var r: R();
t(0) = 2;
var s: R();

writeln(r);
writeln(s);
