class C {
  var x: int;
  def x {
    writeln("getter invoked");
    return x;
  }
  def =x(val: int) {
    writeln("setter invoked");
    x = val;
  }
}

var c = C();
c.x = 2;
writeln(c.x);
