var d : domain(2) = {1..3, 1..3};
var a : [d] int;

forall ij in d {
  var i : int = ij(0);
  var j : int = ij(1);
  a(i,j) = (i-1)*3 + j;
}

for ij in d {
  var i : int = ij(0);
  var j : int = ij(1);
  writeln(a(i,j));
}
