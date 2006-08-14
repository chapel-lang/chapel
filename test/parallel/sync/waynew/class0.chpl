// Test sync field in a class

class D {
  var s: sync int;
}

var d: D = D();
var i: int;

d.s = 4;
i = d.s;
writeln( "i is ", i);


class E {
  var sf: sync float;
}

var e: E = E(); 
var f: float;

e.sf = 1.2;
f = e.sf;
writeln( "f is ", f);
