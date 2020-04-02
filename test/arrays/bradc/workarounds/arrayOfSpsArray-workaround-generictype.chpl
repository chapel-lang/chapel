config const n = 8;

const ProbDom = {1..n, 1..n},
      BigDom = {0..n+1, 0..n+1},
      DnsStencDom = {-1..1, -1..1};

record sps33 {
  type indexType = int;
  var irv = 0.0;

  var data:[-1..1, 0..1] real;

  proc this(i, j) ref {
    if (i == j) {
      return irv;
    } else if (i==-1) {
      return data(i, j==1);
    } else {
      return data(i, j!=-1);
    }
  }

  proc this(ij: 2*indexType) ref {
    return this(ij(0), ij(1));
  }
}

iter SpsStencDom() {
  for (i,j) in {-1..1, -1..1} do
    if (i != j) then
      yield (i,j);
}

iter SpsStencDom(param tag: iterKind) where tag == iterKind.leader {
  yield true;
}

iter SpsStencDom(param tag: iterKind, followThis) where tag == iterKind.follower {
  for (i,j) in {-1..1, -1..1} do
    if (i != j) then
      yield (i,j);
}

var A: [BigDom] real,
    B: [ProbDom] real,
    W: [ProbDom] sps33;  // sps33(int) works, but seems unnecessary

forall (i,j) in ProbDom {
  A(i,j) = (i-1)*n + j;
  forall (x,y) in SpsStencDom() {
    W(i,j)(x,y) = 1.0;
  }
}


writeln("A is:\n", A, "\n");


forall ij in ProbDom do
  B(ij) = (+ reduce [off in SpsStencDom()] W(ij)(off)*A(ij+off)) / 6;

writeln("B is:\n", B, "\n");
