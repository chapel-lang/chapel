var x = (1, 2);

var d : domain(2) = {1..3, 1..3};
var a : [d] int;

forall (i,j) in d {
  a(i,j) = (i-1)*3 + j;
}

for (i,j) in d {
  writeln(a(i,j));
}
