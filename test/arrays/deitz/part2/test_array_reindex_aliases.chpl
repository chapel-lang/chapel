config const k: int = 3, n: int = 10;

def foo(A: [1..k]) {
  writeln(A);
}

def main {
  var A: [i in 1..n] int = i;

  for i in 1..n-k+1 do
    foo(A[i..i+k-1]);
}
