use BlockDist;
use BlockCycDist;

config const N = 4;

const space = {0..#N, 0..#N, 0..#N};
const ParentDom = space dmapped BlockCyclic(space.low, blocksize=(1,2,3));
// const ParentDom = space dmapped Block(space);
var arr: [ParentDom] int;

for (i,j,k) in ParentDom {
  arr[i,j,k] = i*(N**2) + j*(N**1) + k*(N**0);
}

for i in ParentDom.dim(0) {
  for j in ParentDom.dim(1) {
    for k in ParentDom.dim(2) {
      writeln((i,j,k), " expect: ", 16*i + 4*j + k, " got ", arr[i,j,k]);
    }
  }
}
