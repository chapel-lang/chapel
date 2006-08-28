class C {
  var x: int;
  var s: seq of C;
}

var c = C(x=1);
var d = C(x=2);

c.s #= d;

writeln(c);
