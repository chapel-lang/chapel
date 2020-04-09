use IO;

var S = {0..8,0..8}, A: [S] int = for S do read(int);

proc R {
  for (i,j) in S { if A(i,j) == 0 {
    for k in 1..9 {
      if (&& reduce [l in A(i,..)] l != k)
      && (&& reduce [l in A(..,j)] l != k)
      && (&& reduce [l in A(i/3*3..i/3*3+2,j/3*3..j/3*3+2)] l != k) {
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
