var x = (0, 0);

var D : domain(2) = {1..4, 1..4};

for ij in D {
  var i = ij(0);
  var j = ij(1);
  writeln(4 * i + j - 4);
}
