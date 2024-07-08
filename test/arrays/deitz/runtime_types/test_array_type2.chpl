class C {
  type t;
  var x: t;
}

var cOwn = new owned C([1..3] int);
var c = cOwn.borrow();

c.x[2] = 4;
writeln(c);
