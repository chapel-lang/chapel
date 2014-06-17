
record EndPoints {
  var start : int;
  var end : int;
}

proc f((s,e) : EndPoints) {
 return e - s;
}

var x : EndPoints;

x.start = 1;
x.end = 5;

writeln("expect 4 ", f(x));
