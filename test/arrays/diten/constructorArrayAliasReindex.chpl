class C {
  param rank: int;
  param stridable: bool;
  type idxType;
  type eltType;
  var D: domain(rank=rank, stridable=stridable, idxType=idxType);
  var A: [D] eltType;
}

proc main {
  var D = {1..10 by 2};
  var A: [1..5] int;
  var c = new C(1, true, int, int, D, A=>A);
  writeln(c.A.domain);
  writeln(c.A);
}
