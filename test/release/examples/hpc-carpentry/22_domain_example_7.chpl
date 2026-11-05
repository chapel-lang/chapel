use CyclicDist; // Elements are sent to locales in a round-robin pattern
config const n = 8;
const mesh: domain(2) = {1..n, 1..n};  // A 2D domain defined in shared memory
                                       // on a single locale
const m2: domain(2) dmapped Cyclic(startIdx=mesh.low) = mesh; // mesh.low is
                                                              // the first
                                                              // index (1,1)
var A2: [m2] string;
forall a in A2 {
  a = a.locale.id:string + '-' + here.name + '-' + here.maxTaskPar:string + ' ';
}
writeln(A2);
