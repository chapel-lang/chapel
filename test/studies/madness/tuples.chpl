def sum (pt = (0.0, 0.0, 0.0)) {
  const (x, y, z) = pt;
  return x + y + z;
}


writeln(sum((1.0, 2.0, 3.0)));
writeln(sum());

var pt: 3*int;

pt = (1, 2, 3);

const (m, _, _) = pt;

writeln(m);

enum bcasts {rg, rm};
var p: [1..2] bcasts;

p(1) = rg;
p(2) = rm;
writeln(p);
