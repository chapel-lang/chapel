class C {
  var x: int = 1;
  const y: int = 2;
}

var c = new borrowed C();
c.x = 3;
c.y = 4;
writeln(c);
