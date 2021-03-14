use IO;

var S = {0..8,0..8}, A: [S] int = for i in S do read(int);
proc R {
  iter I(i...?k) { for param a in 0..k-1 { for b in i(a) { yield b; }}}
  for (i,j) in S { if A(i,j) == 0 {
    for k in 1..9 {
      if && reduce (I(A(i,..),A(..,j),A(i/3*3..i/3*3+2,j/3*3..j/3*3+2)) != k) {
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
