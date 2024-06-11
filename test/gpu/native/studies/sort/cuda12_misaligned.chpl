on here.gpus[0] {
  var a: [1..10] uint(32);
  @assertOnGpu
  foreach chunk in 0..1 {
    serialScan(a);   // Exclusive scan in serial
  }
}

 proc serialScan(ref arr : [] uint(32)) {
  // Calculate the prefix sum
  var sum : uint(32);
  var temp : uint(32) = arr[1];
  arr[1] = sum;
  sum += temp;

}

