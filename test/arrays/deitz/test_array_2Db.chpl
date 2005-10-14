var d : domain(2) = [1..3, 1..3];
var a : [d] integer;

forall ij:(integer,integer) in d {
  a(ij(1),ij(2)) = (ij(1)-1)*3 + ij(2);
}

forall ij:(integer,integer) in d {
  writeln(a(ij(1),ij(2)));
}
