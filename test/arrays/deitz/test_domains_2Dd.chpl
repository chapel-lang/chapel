var x = (0, 0);

var D : domain(2) = [1..4, 1..4];

forall ij in D {
  var i = ij(1);
  var j = ij(2);
  writeln(4 * i + j - 4);
}
