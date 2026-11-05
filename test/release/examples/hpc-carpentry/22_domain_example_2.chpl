config const n = 8;
const mesh: domain(2) = {1..n, 1..n}; // a 2D domain defined in shared memory
                                      // on a single locale
forall m in mesh { // go parallel through all n^2 mesh points
  writeln((m, m.locale.id, here.id, here.maxTaskPar));
}
