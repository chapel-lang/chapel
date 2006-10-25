// Test sync field in a record

record D {
  var s: sync int;
}

var d: D;
var i: int;

d.s = 4;
i = d.s;
writeln( "i is ", i);


record E {
  var sf: sync real;
}

var e: E;
var f: real;

e.sf = 1.2;
f = e.sf;
writeln( "f is ", f);
