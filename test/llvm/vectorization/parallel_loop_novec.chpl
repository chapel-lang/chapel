//This loop shouldn't be vectorized because no parallel_loop_access
//metadata was added. For more details look into parallel_loop.chpl
proc loop (A, B, C, D) {
  // CHECK-NOT: <4 x i32>
  var sum : int(32) = 0;
  for i in vectorizeOnly(0..511) {
     if(C[i] < D[i]) {
       sum += C[i]+5;
     }
     A[A[i]] = B[i];
     A[i] = B[i+1];
  }
  return sum;
}

var A : [0..511] int(32);
var B : [0..511] int(32);
var C : [0..511] int(32);
var D : [0..511] int(32);

var res = loop(A, B, C, D);
writeln("Sum of A is ", + reduce A);
writeln("Res is ", res);
