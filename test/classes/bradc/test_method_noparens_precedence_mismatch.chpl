class C {
  var y: 3*int;
  proc x do return y;
  proc x(i: real) do return 4;
}

var c = (new owned C()).borrow();
c.y = (1, 2, 3);
writeln(c);
writeln(c.x);
writeln(c.x(3.2));
