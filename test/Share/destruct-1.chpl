
record EndPoints {
  var start : integer;
  var end : integer;
}

function f((s,e) : EndPoints) {
 return e - s;
}

var x : EndPoints;

x.start = 1;
x.end = 5;

writeln("expect 4 ", f(x));
