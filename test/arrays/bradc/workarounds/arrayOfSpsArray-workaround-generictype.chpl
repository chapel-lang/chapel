config const n = 8;

const ProbDom = [1..n, 1..n],
      BigDom = [0..n+1, 0..n+1],
      DnsStencDom = [-1..1, -1..1];

record sps33 {
  type indexType = int;
  var irv = 0.0;

  var data:[-1..1, 0..1] real;

  def this(i, j) var {
    if (i == j) {
      return irv;
    } else if (i==-1) {
      return data(i, j==1);
    } else {
      return data(i, j!=-1);
    }
  }

  def this(ij: 2*indexType) var {
    return this(ij(1), ij(2));
  }
}

def SpsStencDom() {
  for (i,j) in [-1..1, -1..1] do
    if (i != j) then
      yield (i,j);
}

def SpsStencDom(param tag: iterator) where tag == iterator.leader {
  yield true;
}

def SpsStencDom(param tag: iterator, follower) where tag == iterator.follower {
  for (i,j) in [-1..1, -1..1] do
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
