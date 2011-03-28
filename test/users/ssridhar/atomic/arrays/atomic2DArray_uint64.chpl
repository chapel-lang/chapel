config param n: int = 1000;
config param m: int = 10;

proc main() {
  const D = [1..m, 1..m];
  var A: [D] uint(64); 

  coforall i in 1..n do
    for j in 1..m do
      for k in 1..m do
	atomic A(j,k) += 1;

  writeln(A);
}
