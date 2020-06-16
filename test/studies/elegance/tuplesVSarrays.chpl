/**
  * This test demonstrates the performance difference between accessing arrays
  * vs. accessing tuples.
  *
 **/
use Time;

config param subOrder: int = 5;
config const order: int = 100,
             iterations: int = 5,
             correctness: bool = true;


proc main() {

  var elegantTime = elegant(order, subOrder, iterations);

  var prfmnt_T = performant(order, subOrder, iterations);

  if correctness {
    writeln("correctness execution complete");
  } else {
    writeln("Elegant:    ", elegantTime);
    writeln("Performant: ", prfmnt_T);
  }
}


proc elegant(order, param subOrder, iterations): real {

  var Dom = {1..order, 1..order};
  var Array: [Dom] real = 1.0;
  const subDom = {1..subOrder, 1..subOrder};
  var subArray: [subDom] real = 2.0;

  var t = new Timer();

  t.start();

  var total: real = 0;

  for 1..iterations do
    for (i,j) in Dom do
      for k in 1..subOrder do
        Array[i, j] += Array[i, j] * subArray[k, k];

  return t.elapsed()/iterations;
}


proc performant(order, param subOrder, iterations): real {

  var Dom = {1..order, 1..order};
  var Array: [Dom] real = 1.0;
  const subDom = {0..#subOrder, 0..#subOrder};
  var subArray: subOrder*(subOrder*(real));

  for (i,j) in subDom do
    subArray[i][j] = 2.0;


  var t = new Timer();

  t.start();

  var total: real = 0;

  for 1..iterations do
    for (i,j) in Dom do
      for k in 0..#subOrder do
        Array[i, j] += Array[i, j] * subArray[k][k];

  return t.elapsed()/iterations;
}
