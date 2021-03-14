config var n = 9;

proc main() {
  const D = {1..n, 1..n};

  var A: [D] real;

  coforall t in 1..2 {
    for i in t..n by 2 {
      for j in 1..n {
        A(i,j) += i + j/10.0;
      }
    }
  }

  writeln("A is:\n", A);
}
