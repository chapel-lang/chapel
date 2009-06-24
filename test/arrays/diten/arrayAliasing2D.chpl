var aliasDom: domain(2, alias=true) = [1..5, 1..5];
var notAliasDom: domain(2) = [1..9, 1..9];

var A: [notAliasDom] int;
var B: [aliasDom] int;

reset(A);

def reset(A: [] int) {
  for (i, j) in A.domain {
    A(i,j) = i*10 + j;
  }
}

B => A[1..5, 1..5];
B = 1;
writeln(A);
reset(A);

writeln("------");
B => A[1..9 by 2, 1..9 by 2];
B = 1;
writeln(A);
reset(A);

writeln("------");
B => A[5..9, 5..9];
B = 1;
writeln(A);
reset(A);
