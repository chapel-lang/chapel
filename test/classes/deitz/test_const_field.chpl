class C {
  var x: int = 1;
  const y: int = 2;
}

var c = new C();
c.x = 3;
writeln(c);
delete c;
