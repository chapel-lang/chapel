/**
  * This test demonstrates the performance difference between the promoted
  * operation ``Array += 1.0``, and the manual loop that the promoted operation
  * should be reproducing: ``for (i,j) in Dom do Array[i, j] += 1.0``.
 **/
use Time;

config const order: int = 100,
             iterations: int = 5,
             correctness: bool = true;

proc main() {

  var elegantTime = elegant(order, iterations);

  var prfmnt_T = performant(order, iterations);

  if correctness {
    writeln("correctness execution complete");
  } else {
    writeln("Elegant:    ", elegantTime);
    writeln("Performant: ", prfmnt_T);
  }
}


proc elegant(order, iterations): real {

  var Dom = {1..order, 1..order};
  var Array : [Dom] real;

  var t = new Timer();

  t.start();

  for iteration in 1..iterations {
    Array += 1.0;
  }

  return t.elapsed()/iterations;
}


proc performant(order, iterations): real {

  var Dom = {1..order, 1..order};
  var Array : [Dom] real;

  var t = new Timer();

  t.start();

  for 1..iterations {
    forall (i,j) in Dom {
      Array[i, j] += 1.0;
    }
  }

  return t.elapsed()/iterations;
}
