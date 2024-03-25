//Check whether vectorization occurs when loop contains if and reduction variable
//In some cases vectorizer is able to see if loop is vectorizable
//Although it contains branch and reduction variable, we want to make sure
//that we compile loop in a way that it allows for such vectorization
//this uses attributes to force larger vectorization widths than the optimizer might normally use
proc loop (A, B) {
  var sum : int(32) = 0;

  // interleave by 4 will load each array 4 times per loop iteration
  @llvm.metadata(("llvm.loop.vectorize.width", 8), ("llvm.loop.interleave.count", 4))
  foreach i in 1..511 with (ref sum) {
    // CHECK: load <8 x i32>
    // CHECK: load <8 x i32>
    // CHECK: load <8 x i32>
    // CHECK: load <8 x i32>
    // CHECK: load <8 x i32>
    // CHECK: load <8 x i32>
    // CHECK: load <8 x i32>
    // CHECK: load <8 x i32>
    if(A[i] <= B[i])
    {
      sum += A[i]+5;
    }
  }
  return sum;
}

var A : [0..511] int(32);
var B : [0..511] int(32);

var res = loop(A, B);
writeln("Result is ", res);
