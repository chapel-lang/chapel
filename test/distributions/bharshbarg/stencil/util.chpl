
proc verifyStencil(A : [?dom], debug = false) {
  // Domain where each dim is "-1..1"
  param rank = dom.rank;
  const halo = dom._value.fluff;
  var Neighs : domain(rank);
  {
    var n : rank*range;
    for i in 1..rank do n(i) = -1..1;
    Neighs = {(...n)};
  }

  // Build each ghost domain and check it against the actual data to confirm
  // the update occurred correctly
  for neigh in Neighs {
    const base : rank*int;
    if neigh == base then continue; // skip when neigh is all 0s

    var ghost : rank*dom.dim(1).type;
    var actual : rank*dom.dim(1).type;
    for i in 1..rank {
      if neigh(i) < 0 {
        ghost(i) = dom.dim(i).low - halo(i)..#halo(i);
      }
      else if neigh(i) == 0 {
        ghost(i) = dom.dim(i);
      }
      else {
        ghost(i) = dom.dim(i).high+1..#halo(i);
      }
      actual(i) = ghost(i).translate(-neigh(i) * dom.dim(i).size);
    }
    const gdom = {(...ghost)};
    const adom = {(...actual)};

    if debug then writeln("\tComparing ", gdom, " vs. ", adom);
    for (g, a) in zip(gdom, adom) {
      if A[g] != A[a] {
        writeln("Failed when domain is: ", dom);
        halt();
      }
    }
  }
}
