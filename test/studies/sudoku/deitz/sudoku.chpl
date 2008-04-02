var S = [0..8,0..8], A: [S] int = [S] read(int);

def R {
  var T: [0..9] int;
  for (i,j) in S if A(i,j) == 0 {
    T = 0;
    for (m,n) in S if i == m || j == n || i / 3 == m / 3 && j / 3 == n / 3 {
      T(A(m,n)) = 1;
    }
    for k in 1..9 if T(k) == 0 {
      A(i,j) = k;
      R;
    }
    A(i,j) = 0;
    return;
  }
  writeln(A);
  exit(0);
}
R;
