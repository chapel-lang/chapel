class C {
  var x: [1..3] int;
}

var c = new borrowed C();
c.x(2) = 1;

writeln(c);

var a: c.x.type;

writeln(a);
