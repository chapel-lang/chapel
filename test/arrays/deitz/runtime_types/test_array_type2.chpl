class C {
  type t;
  var x: t;
}

var c = (new owned C([1..3] int)).borrow();

c.x[2] = 4;
writeln(c);
