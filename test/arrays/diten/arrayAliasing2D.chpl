var aliasDom: domain(2) = {1..5, 1..5};
var notAliasDom: domain(2) = {1..9, 1..9};

var A: [notAliasDom] int;

reset(A);

proc reset(A: [] int) {
  for (i, j) in A.domain {
    A(i,j) = i*10 + j;
  }
}

{
ref B = A[1..5, 1..5].reindex(aliasDom);
B = 1;
writeln(A);
reset(A);
}

writeln("------");
{
ref B = A[1..9 by 2, 1..9 by 2].reindex(aliasDom);
B = 1;
writeln(A);
reset(A);
}

writeln("------");
{
ref B = A[5..9, 5..9].reindex(aliasDom);
B = 1;
writeln(A);
reset(A);
}
