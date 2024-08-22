config const correctness = true;

// code in the slide starts here

const nProcs = here.gpus.size,
      nRows = nProcs,
      nCols = 5;
var A: [1..nRows, 1..nCols] real;




   coforall (gpu, gRow) in zip(here.gpus, 1..) {
    on gpu do setRowToTwo(gRow);
   }



if !correctness then writeln(A);
proc setRowToTwo(row) {
 var B: [1..nCols] real;
 B = 2;
 A[row, ..] = B;
}

// code in the slide ends here


if correctness {
  var sum = 0.0;
  for a in A do sum += a;
  assert(sum == nRows*nCols*2.0);
}

