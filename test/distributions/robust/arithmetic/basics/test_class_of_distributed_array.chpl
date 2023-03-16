use driver;

class C {
  var dom: domain(2) dmapped Dist2D = {1..5, 1..5};
  var arr: [dom] int;
}

var ownC = new owned C();
var c = ownC.borrow();

for i in c.dom do
  c.arr(i) = next();

writeln(c);
