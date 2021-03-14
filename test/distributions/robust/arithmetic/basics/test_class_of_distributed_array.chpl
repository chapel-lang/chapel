use driver;

class C {
  var dom: domain(2) dmapped Dist2D = {1..5, 1..5};
  var arr: [dom] int;
}

var c = new borrowed C();

for i in c.dom do
  c.arr(i) = next();

writeln(c);
