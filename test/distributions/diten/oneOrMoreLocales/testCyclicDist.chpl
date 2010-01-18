use CyclicDist;

config const tpl = 2;

def test1d() {
  const r = 3.0;
  const Dist = new dist(new Cyclic(rank=1, idxType=int(64), low=tuple(min(int(64))), tasksPerLocale = tpl));
  const Dom: domain(1, int(64)) distributed Dist = [1..10:int(64)];
  var A, B: [Dom] real;

  forall i in Dom {
    B(i) = i;
  }

  forall (a, b) in (A, B) {
    a = b * r;
  }

  writeln(A);
  writeln(B);
}

def test2d() {
  var Dist = new dist(new Cyclic(rank=2, idxType=int, low=(min(int), min(int)), tasksPerLocale=tpl));
  var Dom: domain(2, int) distributed Dist = [1..5, 1..5];
  var A: [Dom] real;

  for (i,j) in Dom {
    writeln(A(i,j).locale, ": A(", i, ",", j, ") = ", A(i,j));
  }

  forall (i,j) in Dom { A(i,j) = i*100 + j; }
  forall a in A { a += 1; }
  writeln(A);
}

def main {
  test1d();
  test2d();
}
