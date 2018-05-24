class C {
  var x: [1..3] int;
}

var a: [1..3] int;
var c = new borrowed C(a);
a(2) = 1;
writeln(c);
writeln(a);
