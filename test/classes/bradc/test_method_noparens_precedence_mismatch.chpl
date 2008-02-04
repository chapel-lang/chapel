class C {
  var y: 3*int;
  def x return y;
  def x(i: real) return 4;
}

var c = new C();
c.y = (1, 2, 3);
writeln(c);
writeln(c.x);
writeln(c.x(3.2));
