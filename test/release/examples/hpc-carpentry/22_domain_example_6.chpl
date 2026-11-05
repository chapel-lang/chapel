use BlockDist; // use block distribution module to partition the domain into
               // blocks
config const n = 8;
const mesh: domain(2) = {1..n, 1..n};
const distributedMesh: domain(2) dmapped Block(boundingBox=mesh) = mesh;
var A: [distributedMesh] string; // block-distributed array mapped to locales
forall a in A { // go in parallel through all n^2 elements in A
  // assign each array element on the locale that stores that index/element
  a = a.locale.id:string + '-' + here.name + '-' + here.maxTaskPar:string + ' ';
}
writeln(A);

for loc in Locales {
  on loc {
    writeln(A.localSubdomain());
  }
}

var counter = 0;
forall a in A with (+ reduce counter) { // go in parallel through all n^2 
                                        // elements
  counter = 1;
}
writeln("Actual number of tasks = ", counter);

for loc in Locales {
  on loc {
    writeln(A2.localSubdomain());
  }
}
