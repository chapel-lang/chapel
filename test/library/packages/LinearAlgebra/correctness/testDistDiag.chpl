use LinearAlgebra;
use BlockDist;
use CyclicDist;

config type dtype = real;
config const dim1 = 25, dim2 = 50;
config const k = 0;
const Space = {1..#dim1, 1..#dim2};

const Adom: domain(2) dmapped Block(boundingBox=Space) = Space;

for k in -min(dim1, dim2)+1..min(dim1, dim2)-1 {

  var arr : [Adom] dtype;
  var arrLocal : [Space] dtype;

  forall (i,j) in Adom {
    arr[i,j] = i+j;
  }

  forall (i,j) in Space {
    arrLocal[i,j] = i+j;
  }

  var distDiag = diag(arr, k);

  var localDiag = diag(arrLocal, k);

  const OutDom: domain(1) dmapped Cyclic(startIdx=0) = {0..#localDiag.size};
  var arrOutput : [OutDom] dtype;
  ref outArray = diag(arr, k, arrOutput);

  var check1 = && reduce (distDiag == localDiag);
  var check2 = && reduce (arrOutput == localDiag);

  if check1 && check2 then writeln("Passed");
  else writeln("Failed!");
}
