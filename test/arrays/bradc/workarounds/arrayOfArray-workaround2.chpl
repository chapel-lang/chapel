config const n = 8;

const ProbDom = {1..n, 1..n},
      BigDom = {0..n+1, 0..n+1},
      StencDom = {-1..1, -1..1};

record arr33 {
  var data: [-1..1, -1..1] real;

  proc this(x,y) ref {
    return data(x,y);
  }

  proc this(xy:2*int) ref {
    return data(xy(0), xy(1));
  }
}

var A: [BigDom] real,
    B: [ProbDom] real,
    W: [ProbDom] arr33;

forall (i,j) in ProbDom {
  A(i,j) = (i-1)*n + j;
  forall (x,y) in StencDom {
    W(i,j)(x,y) = if (x == y) then 0.0 else 1.0;
  }
}

writeln("A is:\n", A, "\n");

forall ij in ProbDom do
  B(ij) = (+ reduce [off in StencDom] W(ij)(off)*A(ij+off)) / 6;

writeln("B is:\n", B, "\n");
