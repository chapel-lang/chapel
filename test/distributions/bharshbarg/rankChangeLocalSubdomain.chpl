use BlockDist;

proc display(msg, A: []) {
  writeln("----- ", msg, " -----");
  for loc in Locales {
    on loc {
      const D = A.localSubdomain();
      if D.size > 0 then
        writeln("\t", loc, ": ", D);
      else
        writeln("\t", loc, ": empty");
    }
  }
  writeln();
}

proc main() {
  var D = {1..10, 1..10};
  var Space = D dmapped Block(D);
  var A : [Space] int;

  display("normal array", A);

  display("center-sliced array", A[3..7, 3..7]);
  display("left-sliced array"  , A[1..10, 1..1]);
  display("right-sliced array" , A[1..1, 1..10]);
  display("quad-sliced array"  , A[1..5, 1..5]);

  display("rank-changed left"   , A[1..10, 1]);
  display("rank-changed right"  , A[1, 1..10]);
  display("rank-changed quad"   , A[1..5, 1]);
  display("rank-changed partial", A[1..3, 1]);

  // TODO: reindex
}
