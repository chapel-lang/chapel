use CyclicDist;

config const tpl = 2;

proc test1d() {
  const r = 3.0;
  const Dist = new dmap(new Cyclic(startIdx=min(int(64)), dataParTasksPerLocale=tpl));

  const Dom: domain(1, int(64)) dmapped Dist = {1..10:int(64)};
  var A, B: [Dom] real;

  forall i in Dom {
    B(i) = i;
  }

  forall (a, b) in zip(A, B) {
    a = b * r;
  }

  writeln(A);
  writeln(B);
}

proc test2d() {
  var Dist = new dmap(new Cyclic(startIdx=(min(int), min(int)), dataParTasksPerLocale=tpl));
  var Dom: domain(2, int) dmapped Dist = {1..5, 1..5};
  var A: [Dom] real;

  for (i,j) in Dom {
    writeln(A(i,j).locale, ": A(", i, ",", j, ") = ", A(i,j));
  }

  forall (i,j) in Dom { A(i,j) = i*100 + j; }
  forall a in A { a += 1; }
  writeln(A);
}

proc main {
  test1d();
  test2d();
}
