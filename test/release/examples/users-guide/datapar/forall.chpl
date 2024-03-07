config param runSerial = false,
             runConcurrent = false,
             runRace = false;

config 
  const n = 1000000;
  var A: [1..n] real;

if runSerial {

  for a in A do
    a += 1.0;

} else if runConcurrent {

  coforall a in A do
    a += 1.0;

} else {

  forall a in A do
    a += 1.0;

}

writeln(A);
writeln();

if runRace {

  forall i in 2..n-1 do
    A[i] = A[i-1] + A[i+1];
  writeln(A);
  writeln();

} else {

  forall i in 2..n-1 by 2 do
    A[i] = A[i-1] + A[i+1];
  writeln("A is: ", A);
  writeln();
  
  var B: [1..n] real;
  forall i in 2..n-1 do
    B[i] = A[i-1] + A[i+1];
  writeln("B is: ", B);
  writeln();

}
