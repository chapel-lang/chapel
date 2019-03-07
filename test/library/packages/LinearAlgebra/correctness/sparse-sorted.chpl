
use LayoutCS;
use LinearAlgebra;
use LinearAlgebra.Sparse;

config const n = 1000;
config const debug = false;

//
// BHARSH: Leading up to the 1.16 release, 'dot' with sparse arrays would
// result in a sparse array with unsorted 'idx' values. This would fail
// in different ways, most obviously when checking for domain membership.
//
// I'm not sure what it is about this particular set of indices that triggers
// the failure. Simple patterns like this don't trigger the failure:
// - identity matrix
// - matrix with the same indices in each row
//

proc main() {
  const R = 1..n;
  const parent = {R,R};
  var D : sparse subdomain(parent) dmapped CS(sortedIndices=false);

  for i in R {
    const s = 1 + i % 10;
    for j in 1..n by s # 5 do D += (i,j);
  }

  var A : [D] int;

  var B = A.dot(A);

  var fails = 0;
  forall bb in B.domain with (+ reduce fails) {
    if B.domain.contains(bb) == false {
      fails += 1;
      if debug then writeln("NOT FOUND: ", bb);
    }
  }

  if fails == 0 then writeln("SUCCESS");
  else writeln("Found ", fails, " failures out of ", D.size, " indices.");
}
