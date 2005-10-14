var d : domain(2) = [1..3, 1..3];
var a : [d] integer;

forall ij:(integer,integer) in d {
  var i : integer = ij(1);
  var j : integer = ij(2);
  a(i,j) = (i-1)*3 + j;
}

forall ij:(integer,integer) in d {
  var i : integer = ij(1);
  var j : integer = ij(2);
  writeln(a(i,j));
}
