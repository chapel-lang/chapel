var d : domain(2) = {1..3, 1..3};
var a : [d] int;

var k = 1;
for ij in d {
  a(ij) = k;
  k += 1;
}

writeln(a);

writeln([ij in d] a(ij));
