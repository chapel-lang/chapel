record myr {
  var x:int;
}

proc +(a: myr, b: myr) {
  writeln("Found my plus");
  return new myr(a.x + b.x);
}

proc ==(a: myr, b: myr) {
  writeln("Found my ==, returning !=");
  return a != b;
}

var a = new myr(1);
var b = new myr(2);


writeln(a + b);
writeln(a == b);


