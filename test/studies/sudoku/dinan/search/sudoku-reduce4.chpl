var S = [0..8,0..8], A: [S] int = [S] read(int);

def R {
  for (i,j,k) in [0..8,0..8,1..9] {
    if A(i,j) == 0 && & reduce (A(i,..)!=k)&(A(..,j)!=k)&(A(i/3*3..i/3*3+2,j/3*3..j/3*3+2)!=k) {
      A(i,j) = k;
      R;
      A(i,j) = 0;
    }
    if A(i,j)-k == -9 return;
  }
  writeln(A);
  exit(0);
}
R;
