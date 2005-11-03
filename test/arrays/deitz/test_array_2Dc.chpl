var x = (1, 2);

var d : domain(2) = [1..3, 1..3];
var a : [d] integer;

forall i,j in d {
  a(i,j) = (i-1)*3 + j;
}

forall i,j in d {
  writeln(a(i,j));
}
