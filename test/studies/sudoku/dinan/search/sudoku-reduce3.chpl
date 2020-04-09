use IO;

var S = {0..8,0..8}, A: [S] int = for S do read(int);

proc R {
  proc !=(A: [], k) return && reduce [i in A] i != k;
  for (i,j) in S { if A(i,j) == 0 {
    for k in 1..9 {
      if A(i,..) != k && A(..,j) != k && A(i/3*3..i/3*3+2,j/3*3..j/3*3+2) != k {
        A(i,j) = k;
        R;
      }
    }
    A(i,j) = 0;
    return;
  }}
  writeln(A);
  exit(0);
}
R;
