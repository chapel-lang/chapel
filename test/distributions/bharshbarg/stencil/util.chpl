
proc verifyStencil(A : [?dom], debug = false) {
  // Domain where each dim is "-1..1"
  param rank = dom.rank;
  const halo = dom._value.fluff;
  assert(dom._value.periodic);
  var Neighs : domain(rank);
  {
    var n : rank*range;
    for i in 1..rank do n(i) = -1..1;
    Neighs = {(...n)};
  }

  var abstr : rank*int;
  for i in 1..rank do
    abstr(i) = abs(dom.dim(i).stride);
  const max = dom.expand(halo*abstr);

  // Build each ghost domain and check it against the actual data to confirm
  // the update occurred correctly
  for n in Neighs {
    const neigh = if isTuple(n) then n else (n,);
    const base : rank*int;
    if neigh == base then continue; // skip when neigh is all 0s

    var ghost : rank*dom.dim(1).type;
    var actual : rank*dom.dim(1).type;
    for i in 1..rank {
      const h = halo(i);
      const str = dom.dim(i).stride;
      if neigh(i) < 0 {
        ghost(i) = dom.dim(i).exterior(-h*str);
      }
      else if neigh(i) == 0 {
        ghost(i) = dom.dim(i);
      }
      else {
        ghost(i) = dom.dim(i).exterior(h*str);
      }
      actual(i) = ghost(i).translate(-neigh(i) * dom.dim(i).size * str);
    }
    const gdom = {(...ghost)};
    const adom = {(...actual)};

    if debug then writeln("\tComparing ", gdom, " vs. ", adom);
    for (g, a) in zip(gdom, adom) {
      if A[g] != A[a] {
        writeln("Failed when domain is: ", dom, ". Ghost = ", gdom, "; actual = ", adom);
        halt();
      }
    }
  }
}
